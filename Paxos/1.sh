#!/bin/bash
 
make clean   >/dev/null 
make  -j4  2>&1   >/dev/null 

cd bin

./Paxos_Test  