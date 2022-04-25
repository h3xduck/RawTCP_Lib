#include "streamBuilder.h"

stream_t build_standard_packet_stream_empty_payload(
    int stream_length,
    u_int16_t source_port,
    u_int16_t destination_port,
    const char* source_ip_address,
    const char* destination_ip_address
    ){
        stream_t stream_res;
        stream_res.packet_stream = calloc(stream_length, sizeof(packet_t));
        stream_res.stream_length = 0;
        if(stream_length<1){
            perror("Invalid option, requested to build empty packet stream");
            stream_res.packet_stream = NULL;
            stream_res.stream_length = 0;
            return stream_res;
        }

        for(int ii=0; ii<stream_length; ii++){
            packet_t packet = build_standard_packet(source_port, destination_port, source_ip_address, destination_ip_address, 4096, "");
            if(packet.tcpheader == NULL){
                //Means we got a malformed or null packet back
                stream_res.packet_stream = NULL;
                stream_res.stream_length = 0;
                return stream_res;
            }
            memcpy(stream_res.packet_stream + ii*(sizeof(packet_t)), (void*)&packet, sizeof(packet_t));
            stream_res.stream_length++;
        }

        return stream_res;
    }

/**
 * @brief Frees stream and, internally, all packets contained
 * DO NOT FREE PACKETS IN AN STREAM
 * 
 * @param stream 
 */
void stream_destroy(stream_t stream){
    if(stream.stream_length<1){
        return;
    }
    for(int ii = 0; ii<stream.stream_length; ii++){
        packet_t packet;
        memcpy(&packet, stream.packet_stream+ ii*sizeof(packet_t), sizeof(packet_t));
        packet_destroy(packet);
    }
    stream.stream_length = 0;
    free(stream.packet_stream);
}