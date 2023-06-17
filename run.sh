#!/bin/bash
gcc P1.c -o a -pthread
gcc P2.c -o b -pthread
gcc Scheduler.c -o c
time ./c 1000 1000 1000 in1.txt in2.txt out.txt
#time ./b 1000 1000 1000 in1.txt in2.txt out.txt 96
#meld matrixres.txt out.txt
