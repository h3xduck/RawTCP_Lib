#ifndef HEADER_PACKET_STREAM
#define HEADER_PACKET_STREAM

#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <strings.h>
#include "packet.h"

typedef struct stream_t{
    packet_t* packet_stream;
    int stream_length;
}stream_t;

#endif