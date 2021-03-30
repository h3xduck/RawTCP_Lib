#!/bin/bash

#The current directory full path
declare -r DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
#A variable to determine whether to silence output of internal commands
declare silence=$1

#####################################################################
#                                                                   #
#   Script to build the RawTCP_Lib.                                 #
#                                                                   #
#   Running instructions:                                           #
#       chmod +x build.sh                                           #
#       ./build.sh                                                  #
#                                                                   #
#   Option: Avoid getting the full output of the internal commands: #
#       ./build.sh quiet                                            #
#                                                                   # 
#   Once ran, you can run the executables by:                       #
#       bin/cannon                                                  #
#####################################################################

#A simple function to silence output
quiet(){
    if [ "$silence" == "quiet" ]; then
        "$@" > /dev/null
    else
        "$@"
    fi
}


echo "Building the files..."
quiet rm -R build 2> /dev/null
quiet mkdir build
quiet cd build 

echo "Running CMake..."
quiet cmake .. -DCMAKE_INSTALL_PREFIX=.. -DCMAKE_BUILD_TYPE=Debug


echo "Running Make..."
quiet make
quiet make install

echo "Finished!"
