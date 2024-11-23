#!/bin/bash
# Author: Your Name
# Description : 

SCRIPT_DIR=$(dirname "$(realpath "$0")")
echo $SCRIPT_DIR

# Server compilation
cd "$SCRIPT_DIR/../server"
make clean

make 

echo -e "\n--------------------------------"
echo -e "Server compiled successfully"
echo -e "--------------------------------\n"

# Client compilation
cd "$SCRIPT_DIR/../client"
make clean

make

echo -e "\n--------------------------------"
echo -e "\nClient compiled successfully\n"
echo -e "--------------------------------\n"

cd "$SCRIPT_DIR/../server/bin"
echo $SCRIPT_DIR
./server &

echo -e "\n--------------------------------"
echo -e "\nServer launched\n"
echo -e "--------------------------------\n"

$SCRIPT_DIR/../client/bin/client 127.0.0.1 Hazim
