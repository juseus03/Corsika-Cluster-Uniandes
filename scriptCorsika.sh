#!/bin/bash

#PBS -l nodes=1:ppn=3
#PBS -l mem=26gb
#PBS -l vmem=26gb
#PBS -l walltime=24:00:00


TMP_DIR=/state/partition1/$USER/${PBS_JOBID%.clustermaster.uniandes.edu.co}
OUT_DIR=$PBS_O_WORKDIR/${PRJ_DIR}/results/

mkdir -p $TMP_DIR
mkdir -p $OUT_DIR

cp $PBS_O_WORKDIR/${PRJ_DIR}/*.input $TMP_DIR/.
cp $PBS_O_WORKDIR/setPaths.sh $TMP_DIR/.
cp -r $HOME/corsikaRun/* $TMP_DIR/.

module load corsika/7.5r700_noPARALLEL

cd $TMP_DIR

echo "***************************************************************** " >${FNAME}.log
echo "This is the log file for simulation " >>${FNAME}.log
echo "***************************************************************** " >>${FNAME}.log

START_DATE=$(date +%s.%N)
START_DATE1=$(date)

. ./setPaths.sh

for i in $(ls $TMP_DIR/${FNAME}-*.input)
do
	#echo inputs_Z$i.${MINUTE}>>simulation_${MINUTE}.log
    corsika75700Linux_QGSJET_gheisha_root <$i>${FNAME}.log&
done
wait

FINISH_DATE=$(date +%s.%N)
FINISH_DATE1=$(date)
DIFF=$(echo "$FINISH_DATE- $START_DATE" | bc)
echo "Start Date: $START_DATE1 ">>${FNAME}.log
echo "Finish Date: $FINISH_DATE1" >>${FNAME}.log
echo "Total Time: $DIFF" >>${FNAME}.log

cd $HOME
mv $TMP_DIR/*.root $OUT_DIR/.
mv $TMP_DIR/*.log $OUT_DIR/.
rm -rf $TMP_DIR



