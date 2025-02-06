#! /bin/bash

DIR_OUT="./results"
DIR_IN="./build/src/solver_2/benchmarks"
# FILE="benchmark_memory_default_stack"
# valgrind --read-var-info=yes --read-inline-info=yes \
#     --tool=massif --stacks=yes --time-unit=B \
#     --threshold=0.000 --peak-inaccuracy=0.0 --detailed-freq=1 \
#     --max-snapshots=1000 \
#     --massif-out-file="${DIR_OUT}/massif_solver2_${FILE}.out" "${DIR_IN}/${FILE}" 

# ms_print --threshold=0.000 "${DIR_OUT}/massif_solver2_${FILE}.out" > "${DIR_OUT}/massif_solver2_${FILE}.result"

# FILE="benchmark_memory_stack"
# valgrind --read-var-info=yes --read-inline-info=yes \
#     --tool=massif --stacks=yes --time-unit=B \
#     --threshold=0.000 --peak-inaccuracy=0.0 --detailed-freq=1 \
#     --max-snapshots=1000 \
#     --massif-out-file="${DIR_OUT}/massif_solver2_${FILE}.out" "${DIR_IN}/${FILE}" 

# ms_print --threshold=0.000 "${DIR_OUT}/massif_solver2_${FILE}.out" > "${DIR_OUT}/massif_solver2_${FILE}.result"

# Для solver_3
DIR_IN="./build/src/solver_3/benchmarks"

FILE="benchmark_memory_base_stack"
valgrind --read-var-info=yes --read-inline-info=yes \
    --tool=massif --stacks=yes --time-unit=B \
    --threshold=0.000 --peak-inaccuracy=0.0 --detailed-freq=1 \
    --max-snapshots=1000 \
    --massif-out-file="${DIR_OUT}/massif_solver3_${FILE}.out" "${DIR_IN}/${FILE}" 

ms_print --threshold=0.000 "${DIR_OUT}/massif_solver3_${FILE}.out" > "${DIR_OUT}/massif_solver3_${FILE}.result"

FILE="benchmark_memory_stack"
valgrind --read-var-info=yes --read-inline-info=yes \
    --tool=massif --stacks=yes --time-unit=B \
    --threshold=0.000 --peak-inaccuracy=0.0 --detailed-freq=1 \
    --max-snapshots=1000 \
    --massif-out-file="${DIR_OUT}/massif_solver3_${FILE}.out" "${DIR_IN}/${FILE}" 

ms_print --threshold=0.000 "${DIR_OUT}/massif_solver3_${FILE}.out" > "${DIR_OUT}/massif_solver3_${FILE}.result"
