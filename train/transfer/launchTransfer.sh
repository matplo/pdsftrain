#! /bin/bash

SCRIPTNAME=`readlink -f $0`
BASE=`dirname $SCRIPTNAME`

INPUTFILE=$1
OUTPUTDIR=$2
LOGFILE=$3

source /usr/share/Modules/init/bash
module use /project/projectdirs/alice/hpc/dtnmodulefiles
module load dtn/alien/default

source /tmp/gclient_env_$UID

$BASE/copy_files.py $INPUTFILE &> $LOGFILE
mv $INPUTFILE $OUTPUTDIR
