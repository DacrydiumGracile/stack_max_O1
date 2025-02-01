#!/bin/bash
set -x
# --benchmark_report_aggregates_only=true --benchmark_time_unit=ms

./build/benchmarks/benchmark_1 --benchmark_enable_random_interleaving=true  --benchmark_repetitions=25 > ./results/benchmark_1.txt 2>&1

./build/benchmarks/benchmark_2 --benchmark_enable_random_interleaving=true --benchmark_repetitions=25  > ./results/benchmark_2.txt 2>&1

./build/benchmarks/benchmark_3 --benchmark_enable_random_interleaving=true --benchmark_repetitions=10  > ./results/benchmark_3.txt 2>&1
