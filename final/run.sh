#!/bin/bash

export OMP_NUM_THREADS=4
time ./mandelbox params.dat
export OMP_NUM_THREADS=3
time ./mandelbox params.dat
export OMP_NUM_THREADS=2
time ./mandelbox params.dat
export OMP_NUM_THREADS=1 
time ./mandelbox params.dat
