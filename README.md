# P1 Mergesort

- Author: Lam Nguyen
- CS453

## Overview

In this project, a multithreaded version of the
mergesort algorithm using the pthread library will be implemented.

## Run the code

$make then $./mytests <# of elements> [<number of thread>] [<seed>] 

## Learning Objectives

- Take a singled threaded algorithm and safely convert it to threaded
  to see a performance gain.
- To gain more experience writing concurrent code.
- Explore the pthread library


## Book references

- [Intro to Threads](http://pages.cs.wisc.edu/~remzi/OSTEP/threads-intro.pdf)
- [Threads API](http://pages.cs.wisc.edu/~remzi/OSTEP/threads-api.pdf)


## Implementation Analysis (ABET outcome 6)

I have created a file named speedup.pdf (in the root directory) that analyses
the two aspects of your mergesort solution: efficiency and effectiveness.
