#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/tcp.h>
#include <strings.h>
#include <string.h>
#include <arpa/inet.h>
#include "../include/packetForger.h"
#include "../include/socketManager.h"
#include "include/streamBuilder.h"

int main(){
    packet_t packet = build_standard_packet_auto(8000, 9000, "127.0.0.1", "127.0.0.1", "XDP_PoC_0");
    
    rawsocket_send(packet);

    set_TCP_flags(packet, 0x02);
    rawsocket_send(packet);
    
    //packet_destroy(packet);

    rawsocket_sniff_pattern_fast("XDP_PoC_0");

    /*struct sockaddr_in source;
    memset(&source, 0, sizeof(source));
	source.sin_addr.s_addr = packet.ipheader->daddr;

    printf("Packet: %s\n", inet_ntoa(source.sin_addr));*/

    /*stream_t stream = build_standard_packet_stream_empty_payload(4, 8000, 9000, "127.0.1.1", "127.0.0.1");
    printf("STREAM LENGTH: %i\n", stream.stream_length);
    for(int ii=0; ii<stream.stream_length; ii++){
        rawsocket_send(*(stream.packet_stream+ii*(sizeof(packet_t))));
    }

    stream_destroy(stream);
    for(int ii=0; ii<stream.stream_length; ii++){
        set_TCP_flags(*(stream.packet_stream+ii*(sizeof(packet_t))), 0x02);
        rawsocket_send(*(stream.packet_stream+ii*(sizeof(packet_t))));
    }
    stream_inject(stream, TYPE_TCP_SRC_PORT, "AAABBBCC", 8);
    for(int ii=0; ii<stream.stream_length; ii++){
        rawsocket_send(*(stream.packet_stream+ii*(sizeof(packet_t))));
    }*/


    return 0;
}