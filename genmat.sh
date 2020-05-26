#!/bin/bash
#	Generates a variety of matrices with differen sizes and numbers of channels

#	HWC
cd matrices

for ic in 1 2 4
do

for isize in 64 128 256
do

../synth $ic $isize $isize 100 0 inputHWC$isize$ic

done
done



for kc in 1 2 4 8
do

for ksize in 2 4 8
do

../synth $kc $ksize $ksize 100 0 kernelHWC$ksize$kc

done
done

#	CHW

for ic in 1 2 4
do

for isize in 64 128 256
do

../synth $ic $isize $isize 100 1 inputCHW$isize$ic

done
done



for kc in 1 2 4 8
do

for ksize in 2 4 8
do

../synth $kc $ksize $ksize 100 1 kernelCHW$ksize$kc

done
done

cd ..
