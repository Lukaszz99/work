#!/bin/bash
#$1 energy $2 number of simulations
programs/makeinputfile $1
#programs/commands $1 $2

counter = 1
while ((counter < $2))
do
    qsub run_UrQMD.qsub $1
    ((counter++))
done
