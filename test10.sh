#!/bin/bash

#	Runs every matrix/kernel combo ten times

echo "" > results10

for i in matrices/inputCHW*
do
for k in matrices/kernelCHW*
do

./conv10 $i $k 1 0 >> results10

done
done

for i in matrices/inputHWC*
do
for k in matrices/kernelHWC*
do

./conv10 $i $k 1 0 >> results10

done
done

