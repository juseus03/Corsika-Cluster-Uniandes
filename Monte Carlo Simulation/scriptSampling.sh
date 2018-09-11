#!/bin/bash

#PBS -l nodes=1:ppn=2
#PBS -l mem=10gb
#PBS -l vmem=10gb
#PBS -l walltime=02:00:00


TMP_DIR=/state/partition1/$USER/${PBS_JOBID%.clustermaster.uniandes.edu.co}
OUT_DIR=$PBS_O_WORKDIR/results/

mkdir -p $TMP_DIR
mkdir -p $OUT_DIR

cp $PBS_O_WORKDIR/baseCode/* $TMP_DIR/.

module load hep/Root6.04.18-MadAn5_1.4
LOADROOT

cd $TMP_DIR

root -l -b -q 'samplingMuons_v1.C(1000,'${NRUN}')'

cd $HOME
mv $TMP_DIR/*.root  $OUT_DIR/.
rm -rf $TMP_DIR



