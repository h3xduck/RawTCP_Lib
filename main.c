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
    packet_t packet = build_standard_packet(8000, 7000, 3, "192.168.1.46", "192.168.1.1", 4096, "AB");
    printf("Packet payload: %s\n", packet.payload);
    printf("Packet TCP header checksum %hu\n", packet.tcpheader->check);
    printf("Packet IP header checksum %hu\n", packet.ipheader->check);

    rawsocket_send(packet);

    return 0;
}