#! /bin/bash

TRAIN_ROOT=$1
CONFIG=$2
OUTPUT=$3
USER=$4
FILELIST=$5
NFILES=$6

export NERSC_HOST=pdsf

counter=$(echo "$SGE_TASK_ID - 1" | bc)
MIN=$(echo "$counter * $NFILES" | bc)
MAX=$(echo "($counter+1) * $NFILES" | bc)
echo MIN: $MIN
echo MAX: $MAX

starttime=$(date +%s)
outputdir=$(printf "%s/job%d" $OUTPUT $SGE_TASK_ID)
cd $outputdir
echo $PWD

source $TRAIN_ROOT/train/config/env

export TRAIN_ROOT=$TRAIN_ROOT
cmd=$(printf "python %s/train/steer/runAnalysis.py %s %s %s/train/filelists/%s %s %s &> analysis.log" $TRAIN_ROOT $USER $CONFIG $TRAIN_ROOT $FILELIST $MIN $MAX)
echo $cmd
eval $cmd 
endtime=$(date +%s)
jobtime=$(echo "$endtime-$starttime" | bc)
echo Done after $jobtime sec
