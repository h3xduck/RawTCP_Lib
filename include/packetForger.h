#ifndef HEADER_FORGER
#define HEADER_FORGER

#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

#include "packet.h"
#include "segment.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

packet_t build_standard_packet(
    u_int16_t source_port,
    u_int16_t destination_port,
    const char* source_ip_address,
    const char* destination_ip_address,
    u_int32_t packet_length,
    char* payload
    );

packet_t build_standard_packet_auto(
    u_int16_t source_port,
    u_int16_t destination_port,
    const char* source_ip_address,
    const char* destination_ip_address,
    char* payload
    );

int packet_destroy(packet_t packet);

int set_TCP_flags(packet_t packet, int hex_flags);

int set_TCP_seq_num(packet_t packet, u_int32_t bytes);

int set_TCP_src_port(packet_t packet, u_int16_t bytes);

packet_t build_null_packet(packet_t packet);

#endif