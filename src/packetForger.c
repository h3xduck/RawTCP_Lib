#include "packetForger.h"


packet_t build_standard_packet(
    u_int16_t source_port,
    u_int16_t destination_port,
    u_int32_t max_payload_length,
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
        int payload_length = strlen((const char*)payload)+1;
        printf("Payload length: %i\n", payload_length);fflush(stdout);
        //We copy the payload we were given, just in case they free memory on the other side

        //We now compute the TCP checksum
        struct pseudo_header* psh = generatePseudoHeader(max_payload_length, source_ip_address, destination_ip_address);
        if(!psh){
            perror("Could not allocate memory for pseudo header");
            exit(1);
        }
        unsigned short tcp_checksum_size = (sizeof(struct pseudo_header) + sizeof(struct tcphdr) + max_payload_length)*sizeof(unsigned short);
        unsigned short *tcp_checksum = malloc(tcp_checksum_size);
        if(!tcp_checksum){
            perror("Could not allocate memory for tcp checksum");
            exit(1);
        }
        printf("tcp checksum data length: %i\n", tcp_checksum_size);fflush(stdout);
        printf("PS %i, TCPH %i, PP %i\n", sizeof(struct pseudo_header), sizeof(struct tcphdr), payload_length);fflush(stdout);
        printf("tcpcksum; %hu\n", tcp_checksum);
        memcpy(tcp_checksum, psh, sizeof(struct pseudo_header));
        printf("Start: %hu , Memcopying at %hu\n", tcp_checksum, (unsigned short)(tcp_checksum+ sizeof(struct pseudo_header)));fflush(stdout);
        memcpy(tcp_checksum+ (unsigned short) sizeof(struct pseudo_header), tcpheader, sizeof(struct tcphdr));
        memcpy(tcp_checksum+ (unsigned short) (sizeof(struct pseudo_header)+sizeof(struct tcphdr)), payload, payload_length);
        compute_tcp_checksum(tcpheader, tcp_checksum, tcp_checksum_size);

        //Now we build the whole packet and incorporate the previous tcpheader + payload
        char packet[packet_length];
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

        return result;

}

int packet_destroy(packet_t packet){
    free(packet.packet);
    packet.ipheader = NULL;
    packet.tcpheader = NULL;
    packet.payload = NULL;
    packet.packet = NULL;
    return 0;
}