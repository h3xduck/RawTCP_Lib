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

void reforge_TCP_checksum(packet_t packet);

int packet_destroy(packet_t packet);


#endif