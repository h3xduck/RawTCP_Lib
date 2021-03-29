#This makefile is in charge of creating the executables
cmake_minimum_required(VERSION 2.8)

# set the project name
project(TCPcannon)

# Add the needed flags for our compiler
SET(CMAKE_C_FLAGS "-Wall")

#Adding executables

include_directories(include src)
add_executable(cannon main.c src/segment.c include/segment.h)


#We create the executables in the destination folder bin
install(TARGETS cannon RUNTIME DESTINATION bin)
