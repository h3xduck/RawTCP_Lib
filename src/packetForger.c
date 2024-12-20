#include "packetForger.h"

void forge_TCP_checksum(int payload_length, const char* source_ip_address, const char* destination_ip_address, struct tcphdr* tcpheader, char* payload){
    //We now compute the TCP checksum
    struct pseudo_header* psh = generatePseudoHeader(payload_length, source_ip_address, destination_ip_address);
    if(!psh){
        perror("Could not allocate memory for pseudo header");
        exit(1);
    }
    unsigned short tcp_checksum_size = (sizeof(struct pseudo_header) + sizeof(struct tcphdr)) + payload_length;
    unsigned short *tcp_checksum = malloc(tcp_checksum_size);
    bzero(tcp_checksum, tcp_checksum_size);
    if(!tcp_checksum){
        perror("Could not allocate memory for tcp checksum");
        exit(1);
    }
    memcpy(tcp_checksum, psh, sizeof(struct pseudo_header));
    memcpy(tcp_checksum+ (unsigned short) (sizeof(struct pseudo_header)/sizeof(unsigned short)), tcpheader, sizeof(struct tcphdr));
    memcpy(tcp_checksum+ (unsigned short) ((sizeof(struct pseudo_header)+sizeof(struct tcphdr))/sizeof(unsigned short)), payload, payload_length+1);
    compute_segment_checksum(tcpheader, tcp_checksum, tcp_checksum_size);
    free(psh);
    free(tcp_checksum);
}

void reforge_TCP_checksum(packet_t packet){
    char* source_addr = malloc(sizeof(char)*32);
    inet_ntop(AF_INET, (void*)&(packet.ipheader->saddr), source_addr, INET_ADDRSTRLEN);
    char* dest_addr = malloc(sizeof(char)*32);
    inet_ntop(AF_INET, (void*)&(packet.ipheader->daddr), dest_addr, INET_ADDRSTRLEN);
    packet.tcpheader->check = 0;
    forge_TCP_checksum(packet.payload_length, source_addr, dest_addr, packet.tcpheader, packet.payload);
    free(source_addr);
    free(dest_addr);
}

packet_t build_standard_packet(
    u_int16_t source_port,
    u_int16_t destination_port,
    const char* source_ip_address,
    const char* destination_ip_address,
    u_int32_t packet_length,
    char* payload
    ){
        //First we build a TCP header
        struct tcphdr *tcpheader =  generate_tcp_header(source_port,destination_port,0,0,htons(5840));
        if(!tcpheader){
            perror("Could not allocate memory for tcp header");
            exit(1);
        }
        int payload_length = strlen((const char*)payload);
        //We copy the payload we were given, just in case they free memory on the other side
        forge_TCP_checksum(payload_length, source_ip_address, destination_ip_address, tcpheader, payload);
        
        //Now we build the whole packet and incorporate the previous tcpheader + payload
        char *packet = malloc(sizeof(char)*packet_length);
        bzero(packet, packet_length);

        //First we incorporate the IP header
        struct iphdr *ipheader = generate_ip_header(source_ip_address, destination_ip_address, payload_length);
        //The IP header is the first element in the packet
        memcpy(packet, ipheader, sizeof(struct iphdr));
        free(ipheader);
        ipheader = (struct iphdr*) packet;
        //We incorporate the payload, goes after the tcpheader but we need it already for the checksum computation (the tcpheader does not take part)
        memcpy(packet+sizeof(struct iphdr)+sizeof(struct tcphdr), payload, payload_length);
        //free(payload);
        payload = packet+sizeof(struct iphdr)+sizeof(struct tcphdr);
        compute_ip_checksum(ipheader, (unsigned short*) packet, ipheader->tot_len);
        //Now we incorporate the tcpheader
        memcpy(packet+sizeof(struct iphdr), tcpheader, sizeof(struct tcphdr));
        free(tcpheader);
        tcpheader = (struct tcphdr*)(packet+sizeof(struct iphdr));
        
        //We build the returning data structure
        packet_t result;
        result.ipheader = ipheader;
        result.tcpheader = tcpheader;
        result.payload = payload;
        result.packet = packet;
        result.payload_length = payload_length;
        
        return result;

}

packet_t build_standard_packet_auto(
    u_int16_t source_port,
    u_int16_t destination_port,
    const char* source_ip_address,
    const char* destination_ip_address,
    char* payload
    ){
        //First we build a TCP header
        struct tcphdr *tcpheader =  generate_tcp_header(source_port,destination_port,0,0,htons(5840));
        if(!tcpheader){
            perror("Could not allocate memory for tcp header");
            exit(1);
        }
        int payload_length = strlen((const char*)payload);
        //We copy the payload we were given, just in case they free memory on the other side
        forge_TCP_checksum(payload_length, source_ip_address, destination_ip_address, tcpheader, payload);
        
        //We can calculate the length of the packet taking into account the size of the headers
        int packet_length = sizeof(struct iphdr)+sizeof(struct tcphdr) + payload_length;

        //Now we build the whole packet and incorporate the previous tcpheader + payload
        char *packet = malloc(sizeof(char)*packet_length);
        bzero(packet, packet_length);

        //First we incorporate the IP header
        struct iphdr *ipheader = generate_ip_header(source_ip_address, destination_ip_address, payload_length);
        //The IP header is the first element in the packet
        memcpy(packet, ipheader, sizeof(struct iphdr));
        free(ipheader);
        ipheader = (struct iphdr*) packet;
        //We incorporate the payload, goes after the tcpheader but we need it already for the checksum computation (the tcpheader does not take part)
        memcpy(packet+sizeof(struct iphdr)+sizeof(struct tcphdr), payload, payload_length);
        //free(payload);
        payload = packet+sizeof(struct iphdr)+sizeof(struct tcphdr);
        compute_ip_checksum(ipheader, (unsigned short*) packet, ipheader->tot_len);
        //Now we incorporate the tcpheader
        memcpy(packet+sizeof(struct iphdr), tcpheader, sizeof(struct tcphdr));
        free(tcpheader);
        tcpheader = (struct tcphdr*)(packet+sizeof(struct iphdr));
        
        //We build the returning data structure
        packet_t result;
        result.ipheader = ipheader;
        result.tcpheader = tcpheader;
        result.payload = payload;
        result.packet = packet;
        result.payload_length = payload_length;
        
        return result;

}



int set_TCP_flags(packet_t packet, int hex_flags){
    if(hex_flags>0x200){
        perror("Invalid flags set");
        return -1;
    }
    set_segment_flags(packet.tcpheader, hex_flags);
    reforge_TCP_checksum(packet);
    return 0;
}

int set_TCP_seq_num(packet_t packet, u_int32_t bytes){
    set_segment_seq_num(packet.tcpheader, bytes);
    reforge_TCP_checksum(packet);
    return 0;
}

int set_TCP_src_port(packet_t packet, u_int16_t bytes){
    set_segment_src_port(packet.tcpheader, bytes);
    reforge_TCP_checksum(packet);
    return 0;
}

packet_t build_null_packet(packet_t packet){
    packet.ipheader = NULL;
    packet.packet = NULL;
    packet.payload = NULL;
    packet.payload_length = 0;
    packet.tcpheader = NULL;
    return packet;
}


int packet_destroy(packet_t packet){
    free(packet.packet);
    packet.payload = NULL;
    packet.packet = NULL;
    return 0;
}