#ifndef HEADER_PACKET
#define HEADER_PACKET


#include <netinet/tcp.h>
/**
 * TCP Segment definition
 * 
 * @author marsan27
 * @version 1.0
 * @see
 */ 

/**
 * For the calculation of the checksum, TCP uses some data from the network layer. 
 * Thus we must consider the so called pseudo header.
 */
struct pseudo_header{
    u_int32_t source_address;
    u_int32_t dest_address;
    u_int8_t protocol_type;
    u_int16_t segment_length;
    u_int8_t reserved; 
};

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
}

#endif
