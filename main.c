#define _DEFAULT_SOURCE
#include <stdio.h>
#include "include/segment.h"
#include <unistd.h>
#include <stdlib.h>
#include <netinet/tcp.h>
#include <strings.h>
#include <string.h>
#include <arpa/inet.h>

int main(){
    u_int8_t va = 1;
    struct tcphdr *tcp =  generate_tcp_header(1,va,1,2,3);
    printf("ack: %hu\n", ntohs(tcp->dest));

    char *payload = (char*)malloc(sizeof(char)*200); //200 bytes for the data
    strcpy(payload, "PAYLOAD");
    printf("payload: %s\n", payload);

    struct pseudo_header* psh = generatePseudoHeader(strlen(payload), inet_addr("1.2.3.4"), inet_addr("1.2.3.4"));
    printf("segment_length: %hu\n", psh->segment_length);

    int to_checksum_size = sizeof(struct pseudo_header) + sizeof(struct tcphdr) + strlen(payload);
    char *to_checksum = malloc(to_checksum_size);

    memcpy(to_checksum, psh, sizeof(struct pseudo_header));
    memcpy(to_checksum+sizeof(struct pseudo_header), tcp, sizeof(struct tcphdr));
    memcpy(to_checksum+sizeof(struct pseudo_header)+sizeof(struct tcphdr), payload, strlen(payload));

    compute_checksum(tcp,(unsigned short*) to_checksum, to_checksum_size);

    printf("Checksum: %hu\n", tcp->check);

    return 0;
}