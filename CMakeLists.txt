#This makefile is in charge of creating the executables
cmake_minimum_required(VERSION 2.8)

# set the project name
project(TCPcannon)

# Add the needed flags for our compiler
SET(CMAKE_C_FLAGS "-Wall -D_DEFAULT_SOURCE")

#Adding executables

include_directories(include src)
add_executable(cannon main.c 
    src/segment.c include/segment.h
    src/packet.c include/packet.h
    src/packetForger.c include/packetForger.h
    src/socketManager.c include/socketManager.h)


#We create the executables in the destination folder bin
install(TARGETS cannon RUNTIME DESTINATION bin)
