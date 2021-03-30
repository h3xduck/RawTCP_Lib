# RawTCP_Lib

RawTCP is a library to build custom TCP/IP packets from the ground and send them through raw sockets. It also generates TCP/IP checksums automatically.

RawTCP uses the AF_INET address family so it supports both customization of the Network Layer (IP) and the Transport Layer (TCP) headers. Support for the Link Layer will come in future updates.

## Purpose
* Send spoofed network packets (fake source IP/port).
* Build specific packets for network attacks (e.g SYN flooding).
* OS fingerprinting.
* Fast integration in your network application.

## Installation
RawTCP is built as an static library to facilitate integration with your project. 
1.  [Download](https://github.com/marsan27/RawTCP_Lib/releases/latest) RawTCP and put it in your project directory.
2.  Integrate it in your project
    *  METHOD 1: With gcc compiler
    ```sh
    gcc <your-files> libRawTCP_lib.a
    ```

    * METHOD 2 With cmake
    ```cmake
    target_link_libraries(<your-target> ${CMAKE_CURRENT_SOURCE_DIR}/libRawTCP_Lib.a)
    ```
    Note that you will need to specify the path to the library in both cases if you stored it under a project folder.
3. Import "RawTCP.h" from a source file whenever you need RawTCP.

## Examples
### Create a TCP/IP packet
```c
//Packet indicates source=192.168.1.10:8000, dest=192.168.1.20:8080
packet_t packet = build_standard_packet(8000, 8080, "192.168.1.10", "192.168.1.20", 4096, "MyMessage");
```

### Set TCP flag active
RawTCP uses [hexadecimal notation](https://synfinner.blog/tcp-flags-hex-values) for the TCP flags. Multiple flags can be set at the same time by adding their values.
```c
//Setting both SYN and FIN flags
set_TCP_flags(packet, 0x03);
```
RawTCP manages the recomputation of the checksums automatically.

### Sending packet to destination
```c
//Destination port and destination IP are those specified when creating the packet.
rawsocket_send(packet);
```
IMPORTANT: Raw sockets need root priviliges, so you will need them before you can run this specific function.
### Continously sending packets
```c
//Sequence number & checksum automatically updated by OS
packet_t packet = build_standard_packet(...)
while(1){
    rawsocket_send(packet);
}
```

## Issues?
Whether it is a bug, question or suggestion, please [open a ticket](https://github.com/marsan27/RawTCP_Lib/issues/new) and I will have a look at it as fast as I possibly can.
## LICENSE
This software is licensed under the MIT license. See [LICENSE.md](https://github.com/marsan27/RawTCP_Lib/blob/master/LICENSE.md)






