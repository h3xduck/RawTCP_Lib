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

int main(){
    /*packet_t packet = build_standard_packet(8000, 7000, "192.168.1.102", "192.168.1.1", 4096, "");
   
    rawsocket_send(packet);
    set_TCP_flags(packet, 0x02);
    
    packet_destroy(packet);*/

    packet_t packet = rawsocket_sniff();

    struct sockaddr_in source;
    memset(&source, 0, sizeof(source));
	source.sin_addr.s_addr = packet.ipheader->saddr;

    printf("Packet: %s\n", inet_ntoa(source.sin_addr));

    return 0;
}