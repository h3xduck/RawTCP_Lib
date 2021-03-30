#include "../include/segment.h"
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netinet/tcp.h>
#include <netinet/in.h>


struct tcphdr* generate_tcp_header(
    u_int16_t source, //source port
    u_int16_t destination, //destination port
    u_int32_t seq_num, //sequence number
    u_int32_t ack_num, //acknowledgment number
    u_int16_t window //congestion window size
    ){
        struct tcphdr *tcpheader = malloc(sizeof(struct tcphdr));
        bzero(tcpheader, sizeof(struct tcphdr));
        //Set the tcp header with the parameters and all flags to 0 for now.
        //Also checksum is generated later
        tcpheader->ack = 0; //ACK flag
        tcpheader->ack_seq = ack_num;   //ACK sequence number
        tcpheader->check = 0; //Checksum 0, set later
        tcpheader->dest = htons(destination); //Dest port
        tcpheader->doff = 5; //5 bytes, no options
        tcpheader->fin = 0; //FIN flag
        tcpheader->psh = 0; //PSH flag
        tcpheader->rst = 0; //RST flag
        tcpheader->seq = seq_num; //Sequence number
        tcpheader->source = htons(source); //Source port
        tcpheader->syn = 0; //SYN flag
        tcpheader->urg = 0; //URG flag
        tcpheader->urg_ptr = 0; //URG pointer
        tcpheader->window = window; //window size
        

        return tcpheader;
    }

/**
 * Generates header with data needed for TCP checksum computation
 * 
 */
struct pseudo_header* generatePseudoHeader(
    u_int16_t payload_length,
    const char *source_address,
    const char *dest_address
    ){
        struct pseudo_header *psh = malloc(sizeof(struct pseudo_header));
        bzero(psh, sizeof(struct pseudo_header));
        inet_pton(AF_INET, dest_address, (void*)&(psh->dest_address));
        psh->protocol_type = IPPROTO_TCP;
        psh->reserved = 0;
        psh->segment_length = htons(payload_length+sizeof(struct tcphdr));
        inet_pton(AF_INET, source_address, (void*)&(psh->source_address));

        return psh;    
}

/**
 * TCP checksum calculation.
 * Following RFC 1071.
 * In essence 1's complement of 16-bit groups.
 */ 
unsigned short tcp_checksum(unsigned short *addr, int nbytes){
    long sum = 0;
    unsigned short checksum;
    while(nbytes>1){
        sum += (unsigned short) *addr++;
        printf("Sum: %ld \n",sum);
        nbytes -= 2;
    }
    if(nbytes>0){
        sum += htons((unsigned char)*addr);
        printf("Sum: %ld \n",sum);
    }
            
    while (sum>>16){
        sum = (sum & 0xffff) + (sum >> 16);
        printf("Sum: %ld \n",sum);
    }

    checksum = ~sum;
    printf("Calculated checksum %hu\n", checksum);
    return checksum;
}


void compute_tcp_checksum(struct tcphdr *tcpheader, unsigned short *addr, int nbytes){
    u_int16_t res =tcp_checksum(addr, nbytes);
    tcpheader->check = res;
}

