#define _DEFAULT_SOURCE
#include "../include/segment.h"
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netinet/tcp.h>
#include <netinet/in.h>

/* modern glibc will complain about the above if it doesn't see this. */


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


struct pseudo_header* generatePseudoHeader(
    u_int16_t payload_length,
    u_int32_t source_address,
    u_int32_t dest_address
    ){
        struct pseudo_header *psh = malloc(sizeof(struct pseudo_header));
        bzero(psh, sizeof(struct pseudo_header));
        psh->dest_address = dest_address;
        psh->protocol_type = IPPROTO_TCP;
        psh->reserved = 0;
        psh->segment_length = payload_length+sizeof(struct tcphdr);
        psh->source_address = source_address;

        return psh;    
}

/**
 * TCP checksum calculation.
 * Following RFC 1071.
 * In essence 1's complement of 16-bit groups.
 */ 
unsigned short checksum(unsigned short *addr, int nbytes){
    long sum = 0;
    unsigned short checksum;
    while(nbytes>1){
        sum += (unsigned short) *addr++;
        nbytes -= 2;
    }
    if(nbytes>0){
        sum +=(unsigned char)*addr;
    }
            
    while (sum>>16){
        sum = (sum & 0xffff) + (sum >> 16);
    }

    checksum = ~sum;
    return checksum;
}


int compute_checksum(struct tcphdr *tcpheader, unsigned short *addr, int nbytes){
    tcpheader->check = checksum(addr, nbytes);
    return 0;
}
