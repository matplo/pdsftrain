#! /bin/bash

TRAINROOT=$1
CONFIG=$2
INPUTDIR=$3
OUTPUTDIR=$4
FILENAME=$5

cd $OUTPUTDIR
export TRAIN_ROOT=$TRAINROOT
source $TRAIN_ROOT/train/config/env

python $TRAIN_ROOT/run.py merge -c $CONFIG -i $INPUTDIR -f $FILENAME
