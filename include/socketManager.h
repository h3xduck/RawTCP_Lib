#ifndef HEADER_SOCKETS
#define HEADER_SOCKETS

#include "packetForger.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "packetInterpreter.h"

int rawsocket_send(packet_t packet);

packet_t rawsocket_sniff();

#endif