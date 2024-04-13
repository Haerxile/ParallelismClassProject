#!/usr/bin/bash

make

make clean

mpirun -np 4 ./helloworld
