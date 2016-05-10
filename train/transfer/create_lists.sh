#! /bin/bash
RUNLIST=$1
BASEDIR=$2

while read LINE; do 
        echo $LINE
        cmd=$(printf "find %s/%09d -name root_archive.zip | sed -e 's/zip/zip#AliESDs.root/g' >> %09d.txt" $BASEDIR $LINE $LINE)
        echo $cmd
        eval $cmd
done < $RUNLIST
