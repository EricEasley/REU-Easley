#!/bin/bash

#	Runs every matrix/kernel combo once

cd matrices

for i in inputCHW*
do
for k in kernelCHW*
do

cat unrollerHeader > ../unroll/unrolled10$i$k
../unroller10 $i $k 1 0 >> ../unroll/unrolled10$i$k
cat unrollerFooter >> ../unroll/unrolled10$i$k

done
done

for i in inputHWC*
do
for k in kernelHWC*
do

cat unrollerHeader > ../unroll/unrolled10$i$k
../unroller10 $i $k 1 0 >> ../unroll/unrolled10$i$k
cat unrollerFooter >> ../unroll/unrolled10$i$k

done
done

