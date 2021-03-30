#include "../include/socketManager.h"

int rawsocket_send(packet_t packet){
    //Create raw socket.
    //This needs ROOT priviliges on the system!
    int sock = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);
    if(sock == -1){
        perror("ERROR opening raw socket. Do you have root priviliges?");
        return -1;
    }
    struct sockaddr_in sock_in;
    sock_in.sin_addr.s_addr = packet.ipheader->daddr;
    sock_in.sin_family = AF_INET;
    sock_in.sin_port = packet.tcpheader->dest;

    //We need to set the flag IP_HDRINCL as an option to specify that we already included all headers in the packet
    int one = 1;
    if(setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one))<0){
        perror("ERROR setting desired flags in socket options");
        return -1;
    }

    int sent = sendto(sock, packet.packet, packet.ipheader->tot_len, 0, (struct sockaddr*)&sock_in, sizeof(sock_in));
    if(sent<0){
        perror("ERROR sending the packet in the socket");
        return -1;
    }

    printf("Packet of length %d sent to %u\n", packet.ipheader->tot_len, packet.ipheader->daddr);

    close(sock);
    return 0;
}


