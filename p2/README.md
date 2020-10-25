# Project 2 - Threadsafe Library

- Author: Lam Nguyen
- CS 453

## Overview

In this project, we will create a thread-safe version of an existing list library. We will then test it using a provided solution to the producers-consumers problem.

## Setup for libraries
We are using two libraries in this project. The original linked list library libmylist.so  that is in the folder ./list/lib and the wrapper library libthreadsafe-mylib.so that is in the ./lib directory. You must export both paths in LD_LIBRARY_PATH for everything to work and the paths must be relative to the executable (pc).  For example, use the following command before running the pc executable:

  export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./list/lib:./lib

## Run the code

$export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../list/lib:./lib

Then $make

Then $./pc [poolsize] [items] [# of producers] [# of consumers] [sleeptime]

## Testing
I ran the test with ./pc 100 400 4 8 100 multiple times and it worked correctly.
