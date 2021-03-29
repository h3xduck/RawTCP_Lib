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

struct tcphdr* generate_tcp_header(
    u_int16_t source,
    u_int16_t destination,
    u_int32_t seq_num,
    u_int32_t ack_num,
    u_int16_t window
);

struct pseudo_header* generatePseudoHeader(
    u_int16_t payload_length,
    u_int32_t source_address,
    u_int32_t dest_address
);

int compute_checksum(struct tcphdr *tcpheader, unsigned short *addr, int nbytes);

#endif
