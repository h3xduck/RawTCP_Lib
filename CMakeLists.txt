#This makefile is in charge of creating the executables and the library
cmake_minimum_required(VERSION 2.8)

# set the project name
project(RawTCP_Lib)

# Add the needed flags for compiler
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -D_DEFAULT_SOURCE")

#Adding executables
include_directories(include src)

#This is to let cmake know that the library used later is not under /usr/bin, but rather in our custom dir
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

add_executable(RawTCP_Exec main.c)

#Add files with implementation and INTERFACE library type (header only).
add_library(RawTCP_Lib STATIC
    src/segment.c src/packet.c src/packetForger.c src/socketManager.c src/packetInterpreter.c src/streamBuilder.c)

include(GNUInstallDirs)

#Include library
include_directories(lib) 

#We link the client target with the library built under project keys
target_link_libraries(RawTCP_Exec PUBLIC RawTCP_Lib)

#We create the executables in the destination folder bin
install(TARGETS RawTCP_Exec RUNTIME DESTINATION bin)
install(TARGETS RawTCP_Lib ARCHIVE DESTINATION lib)
