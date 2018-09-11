#!/bin/bash

for i in {1..100}
do
	qsub -v NRUN=$i -N "SamplingR"$i scriptSampling.sh
done


