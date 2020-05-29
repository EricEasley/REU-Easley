#!/bin/bash
#	Generates a variety of matrices with different sizes and numbers of channels

#	HWC
cd matrices

for ic in 1 2 4 8
do

for isize in 64 256 1024 2048 4096
do

../synth $ic $isize $isize 100 0 inputHWC$isize$ic

done
done



for kc in 1 2 8 16 32 64
do

for ksize in 1 3 5 7
do

../synth $kc $ksize $ksize 100 0 kernelHWC$ksize$kc

done
done

#	CHW

for ic in 1 2 4 8
do

for isize in 64 256 1024 2048 4096
do

../synth $ic $isize $isize 100 1 inputCHW$isize$ic

done
done



for kc in 1 2 8 16 32 64
do

for ksize in 1 3 5 7
do

../synth $kc $ksize $ksize 100 1 kernelCHW$ksize$kc

done
done

cd ..
