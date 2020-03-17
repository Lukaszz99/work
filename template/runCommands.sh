#!/bin/bash
#x exiting variable
NEV=500 #number of events in one file (preferred 500)
N_JOBS=300 #number of jobs to run (max 300 per person)
MODEL=UrQMD #model used for simulation (only UrQMD 3.4 available right now)
NUC_1=Bi #first nucleus (symbol: Au,Bi)
NUC_2=Bi #secodnf nucleus (symbol: Au,Bi)
ECM=9 #energy (in GeV)
IMP=3 #max impact parameter (in fm)

echo "Configuration:"
    echo "model=$MODEL"
    echo "$NUC_1-$NUC_2"
    echo "ecm=$ECM GeV"
    echo "imp=$IMP fm"
    echo "nev=$NEV"
    echo "n_jobs=$N_JOBS"
    echo "correct? (y/n)"
    read x

while [ $x != y ]
do
#    echo "Which model? (UrQMD)"
#   read MODEL
    
#    echo "What is first nucleus to collide? (Au, Bi)"
#    read NUC_1
    
#    echo "What is second nucleus to collide? (Au, Bi)"
#    read NUC_2
    
    echo "At what energy? (preferred is 9GeV)"
    read ECM

    echo "With what maximum impact parameter?"
    read IMP

    echo "How many evets in one job? (preferred 500)"
    read NEV

    echo "How many jobs? (maximum is 300 per person)"
    read N_JOBS
    
    echo "Configuration:"
    echo "model=$MODEL"
    echo "$NUC_1-$NUC_2"
    echo "ecm=$ECM GeV"
    echo "imp=$IMP fm"
    echo "nev=$NEV"
    echo "n_jobs=$N_JOBS"
    echo "correct? (y/n)"
    read x   
done


if [ $x = y ]
then
    counter=0
    while [ $counter -lt $N_JOBS ]
    do
	qsub run_$MODEL.qsub $NUC_1 $ECM $IMP $NEV
	counter=$(( $counter + 1 ))
    done
else
    echo "Error: wrong configuration"
fi