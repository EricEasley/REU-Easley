cat ../unrollerHeader > ../unroll/unrolled1$1$2.c
../unroller $1 $2 1 0 >> ../unroll/unrolled1$1$2.c
cat ../unrollerFooter >> ../unroll/unrolled1$1$2.c

~/../sadavally/RISCV/bin/riscv64-unknown-elf-gcc -o ../unroll/unrolled1$1$2 ../unroll/unrolled1$1$2.c
rm ../unroll/unrolled1$1$2.c
../../spike/riscv-isa-sim/build/spike ../../spike/pk ../unroll/unrolled1$1$2 $1 $2 1 0 | grep "ordering"  >> ../resultsur1
rm ../unroll/unrolled1$1$2
