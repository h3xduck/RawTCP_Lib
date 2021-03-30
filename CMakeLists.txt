#This makefile is in charge of creating the executables and the library
cmake_minimum_required(VERSION 2.8)

# set the project name
project(RawTCP_Lib)

# Add the needed flags for compiler
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -D_DEFAULT_SOURCE")

#Adding executables
include_directories(include src)

add_executable(RawTCP_Exec main.c 
    src/segment.c include/segment.h
    src/packet.c include/packet.h
    src/packetForger.c include/packetForger.h
    src/socketManager.c include/socketManager.h)

#Add files with implementation and INTERFACE library type (header only).
add_library(RawTCP_Lib SHARED 
    src/segment.c src/packet.c src/packetForger.c src/socketManager.c )

# Public API of the library (the header file)
set_target_properties(RawTCP_Lib PROPERTIES PUBLIC_HEADER 
    "include/packetForger.h;include/socketManager.h")

include(GNUInstallDirs)

#We create the executables in the destination folder bin
install(TARGETS RawTCP_Exec RUNTIME DESTINATION bin)
install(TARGETS RawTCP_Lib LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION lib)
