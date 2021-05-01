#ifndef HEADER_PACKET
#define HEADER_PACKET

#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <strings.h>

/**
 * IP Packet definition
 * 
 * @author h3xduck
 * @version 1.0
 * @see
 */ 

typedef struct packet_t{
    struct iphdr *ipheader;
    struct tcphdr *tcpheader;
    char *payload;
    int payload_length;
    char* packet;
}packet_t;


struct iphdr* generate_ip_header(
    const char *source_address,
    const char *dest_address,
    u_int16_t payload_length
);

void compute_ip_checksum(struct iphdr *iphdr, unsigned short *packet, int nbytes);



#endif