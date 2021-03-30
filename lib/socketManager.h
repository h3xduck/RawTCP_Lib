#ifndef HEADER_SOCKETS
#define HEADER_SOCKETS

#include "packetForger.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int rawsocket_send(packet_t packet);

#endif