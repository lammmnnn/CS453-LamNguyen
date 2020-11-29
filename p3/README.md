# P3 Device Driver

- Author: Lam Nguyen
- CS453

## Overview

In this assignment, we will write a simple character driver called booga.

## Run the code

$make 
then $sudo ./booga_load
then $./test-booga  <minor number> <buffersize> <read|write>
then $sudo ./booga_unload 

## Plan for testing

Test functionality with the given test file booga-test1.sh and check for thread-safe with booga-test2.sh.
Here are some additional tests for read/write: 
./test-booga 0 100 read 
./test-booga 1 100 read 
./test-booga 2 100 read 
./test-booga 3 100 read 
./test-booga 0 100 write 
./test-booga 1 100 write 
./test-booga 2 100 write 
./test-booga 3 100 write



## Thread-safety

Protect the updating of structure used to track the statistics using semaphores. Refer to example driver code on the course page for an example on how to use semaphores in Linux kernel.
