#!/bin/bash

#	Runs every matrix/kernel combo one thousand times

echo "" > results1000

for i in matrices/inputCHW*
do
for k in matrices/kernelCHW*
do

./conv1000 $i $k 1 0 >> results1000

done
done

for i in matrices/inputHWC*
do
for k in matrices/kernelHWC*
do

./conv1000 $i $k 1 0 >> results1000

done
done

