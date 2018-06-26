#!/bin/bash

PRJ=./$1

for i in $(ls $PRJ/*.input)
do
	j=$(echo $i | sed 's/-[0-9]\+.input//')
	k=$(echo $j | cut -d'/' -f3-)
	qsub -v PRJ_DIR=$1,FNAME=$k -N ""$1"-"$k scriptCorsika.sh
done


