cat ../unrollerHeader > ../unroll/unrolled3$1$2.c
../unroller3 $1 $2 1 0 >> ../unroll/unrolled3$1$2.c
cat ../unrollerFooter >> ../unroll/unrolled3$1$2.c

~/../sadavally/RISCV/bin/riscv64-unknown-elf-gcc -o ../unroll/unrolled3$1$2 ../unroll/unrolled3$1$2.c
rm ../unroll/unrolled3$1$2.c
../../spike/riscv-isa-sim/build/spike ../../spike/pk ../unroll/unrolled3$1$2 $1 $2 1 0 | grep "ordering"  >> ../resultsur3
rm ../unroll/unrolled3$1$2
