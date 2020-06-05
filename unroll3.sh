#!/bin/bash

#	Runs every matrix/kernel combo once

cd matrices

for i in inputHWC1024* inputHWC256* inputHWC64*
do
for k in kernelHWC*
do

../iunroll3.sh $i $k

done
done

