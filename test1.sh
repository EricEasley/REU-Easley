#!/bin/bash

#	Runs every matrix/kernel combo once

echo "" > results1

for i in matrices/inputCHW*
do
for k in matrices/kernelCHW*
do

./conv1 $i $k 1 0 >> results1

done
done

for i in matrices/inputHWC*
do
for k in matrices/kernelHWC*
do

./conv1 $i $k 1 0 >> results1

done
done

