#include "packetForger.h"


packet_t build_standard_packet(
    u_int16_t source_port,
    u_int16_t destination_port,
    u_int32_t max_payload_length,
    const char* source_ip_address,
    const char* destination_ip_address,
    u_int32_t packet_length,
    const char* payload
    ){
        //First we build a TCP header
        struct tcphdr *tcpheader =  generate_tcp_header(source_port,destination_port,0,0,htons(5840));
        int payload_length = strlen(payload)+1;
        printf("Payload length: %i\n", payload_length);fflush(stdout);
        //We copy the payload we were given, just in case they free memory on the other side

        char *packet_payload = (char*)malloc(sizeof(char)*payload_length);
        memcpy(packet_payload, payload, payload_length*sizeof(char));

        //We now compute the TCP checksum
        struct pseudo_header* psh = generatePseudoHeader(payload_length, source_ip_address, destination_ip_address);
        int tcp_checksum_size = sizeof(struct pseudo_header) + sizeof(struct tcphdr) + payload_length;
        unsigned short *tcp_checksum = malloc(tcp_checksum_size*sizeof(unsigned short));
        printf("tcp checksum data length: %i\n", tcp_checksum_size);fflush(stdout);
        printf("PS %i, TCPH %i, PP %i\n", sizeof(struct pseudo_header), sizeof(struct tcphdr), payload_length);fflush(stdout);

        memcpy(tcp_checksum, psh, sizeof(struct pseudo_header));
        memcpy(tcp_checksum+ (unsigned short) sizeof(struct pseudo_header), tcpheader, sizeof(struct tcphdr)+payload_length);
        memcpy(tcp_checksum+ (unsigned short) (sizeof(struct pseudo_header)+sizeof(struct tcphdr)), packet_payload, payload_length);
        compute_tcp_checksum(tcpheader, tcp_checksum, tcp_checksum_size);

        //Now we build the whole packet and incorporate the previous tcpheader + payload
        char packet[packet_length];
        bzero(packet, packet_length);

        //First we incorporate the IP header
        struct iphdr *ipheader = generate_ip_header(destination_ip_address, source_ip_address, payload_length);
        //The IP header is the first element in the packet
        memcpy(packet, ipheader, sizeof(struct iphdr));
        free(ipheader);
        ipheader = (struct iphdr*) packet;
        //We incorporate the payload, goes after the tcpheader but we need it already for the checksum computation (the tcpheader does not take part)
        memcpy(packet+sizeof(struct iphdr)+sizeof(struct tcphdr), packet_payload, payload_length);
        free(packet_payload);
        packet_payload = packet+sizeof(struct iphdr)+sizeof(struct tcphdr);
        compute_ip_checksum(ipheader, (unsigned short*) packet, ipheader->tot_len);
        //Now we incorporate the tcpheader
        memcpy(packet+sizeof(struct iphdr), tcpheader, sizeof(struct tcphdr));
        free(tcpheader);
        tcpheader = (struct tcphdr*)(packet+sizeof(struct iphdr));

        //We build the returning data structure
        packet_t result;
        result.ipheader = ipheader;
        result.tcpheader = tcpheader;
        result.payload = packet_payload;
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