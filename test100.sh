#!/bin/bash

#	Runs every matrix/kernel combo 100 times

echo "" > results100

for i in matrices/inputCHW*
do
for k in matrices/kernelCHW*
do

./conv100 $i $k 1 0 >> results100

done
done

for i in matrices/inputHWC*
do
for k in matrices/kernelHWC*
do

./conv100 $i $k 1 0 >> results100

done
done

