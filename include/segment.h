#ifndef HEADER_SEGMENT
#define HEADER_SEGMENT

#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

#include <netinet/tcp.h>
/**
 * TCP Segment definition
 * 
 * @author h3xduck
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
    u_int8_t reserved; 
    u_int8_t protocol_type;
    u_int16_t segment_length;
    
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
    const char *source_address,
    const char *dest_address
);

void compute_segment_checksum(struct tcphdr *tcpheader, unsigned short *addr, int nbytes);

#define FIN 0x01
#define SYN 0x02
#define RST 0x04
#define PSH 0x08
#define ACK 0x10
#define URG 0x20
#define ECE 0x40
#define CWR 0x80

void set_segment_flags(struct tcphdr *tcphdr, int flags);

void set_segment_seq_num(struct tcphdr *tcphdr, u_int32_t bytes);

void set_segment_src_port(struct tcphdr *tcphdr, u_int16_t bytes);

#endif
