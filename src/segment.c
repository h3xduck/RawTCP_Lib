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
        nbytes -= 2;
    }
    if(nbytes>0){
        sum += (unsigned char *) *addr;
    }
            
    while (sum>>16){
        sum = (sum & 0xffff) + (sum >> 16);
    }

    checksum = ~sum;
    return checksum;
}


void compute_segment_checksum(struct tcphdr *tcpheader, unsigned short *addr, int nbytes){
    u_int16_t res =tcp_checksum(addr, nbytes);
    tcpheader->check = res;
}

void set_segment_flags(struct tcphdr *tcphdr, int flags){
    int iterator = 1;
    int *result = malloc(sizeof(int)*8);
    int counter = 0;
    while (iterator <= flags) {
        if (iterator & flags){
            result[counter] = iterator;
            
        } 
        counter++;
        iterator <<= 1;
    }
    
    
    for(int ii=0; ii<8; ii++){
        if((result[ii] - CWR) == 0) tcphdr->res1 = 1;
        if((result[ii] - ECE) == 0) tcphdr->res2 = 1;
        if((result[ii] - URG) == 0) tcphdr->urg = 1;
        if((result[ii] - ACK) == 0) tcphdr->ack = 1;
        if((result[ii] - PSH) == 0) tcphdr->psh = 1;
        if((result[ii] - RST) == 0) tcphdr->rst = 1;
        if((result[ii] - SYN) == 0) tcphdr->syn = 1;
        if((result[ii] - FIN) == 0) tcphdr->fin = 1;
    }
    
}


void set_segment_seq_num(struct tcphdr *tcphdr, u_int32_t bytes){
    tcphdr->seq = bytes;
}

void set_segment_src_port(struct tcphdr *tcphdr, u_int16_t bytes){
    tcphdr->source = bytes;
}
