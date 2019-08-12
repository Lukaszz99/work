#!/bin/bash
# $1 energy typed in vHLLE.C $2 number of simulations

counter = 1
while ((counter < $2))
do
    qsub run_vHLLE.qsub $1
    ((counter++))
done