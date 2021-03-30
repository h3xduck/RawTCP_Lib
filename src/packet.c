#include "../include/packet.h"


struct iphdr* generate_ip_header(
    const char *source_address,
    const char *dest_address,
    u_int16_t payload_length
    ){
        struct iphdr* ipheader = malloc(sizeof(struct iphdr));
        bzero(ipheader, sizeof(struct iphdr));

        ipheader->check = 0;
        inet_pton(AF_INET, dest_address, (void*)&(ipheader->daddr));
        ipheader->frag_off = 0;
        ipheader->id = htonl(54321);
        ipheader->ihl = 5; //Header length, no options
        ipheader->protocol = 6; //TCP
        inet_pton(AF_INET, source_address, (void*)&(ipheader->saddr));
        ipheader->tos = 0;
        ipheader->tot_len = sizeof(struct iphdr)+sizeof(struct tcphdr)+payload_length;
        ipheader->ttl = 255; //Time to live
        ipheader->version = 4; //Ipv4

        return ipheader;
    }

/**
 * IP checksum calculation.
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
        sum +=htons((unsigned char)*addr);
    }
            
    while (sum>>16){
        sum = (sum & 0xffff) + (sum >> 16);
    }

    checksum = ~sum;
    return checksum;
}

void compute_ip_checksum(struct iphdr *ipheader, unsigned short *packet, int nbytes){
    ipheader->check = checksum(packet, nbytes);
}
