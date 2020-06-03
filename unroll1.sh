#!/bin/bash

#	Runs every matrix/kernel combo once

cd matrices

for i in inputCHW*
do
for k in kernelCHW*
do

../iunroll1.sh $i $k

done
done

for i in inputHWC*
do
for k in kernelHWC*
do

../iunroll1.sh $i $k

done
done

