#include "solution.hpp"
#include "standard_solution.hpp"

#include <benchmark/benchmark.h>
#include <iostream>
#include <vector>

size_t B = 32uz;
size_t IMAX = (1uz << B) - 1uz;

static void BM_StandardStack(benchmark::State &state) {
  size_t data_size = state.range(0);
  StandardStack stack(data_size);
  size_t d = static_cast<size_t>(IMAX / (data_size + 1));
  for (auto _ : state) {
    size_t value = 0;
    for (size_t i = 0; i < data_size; ++i) {
      value += d;
      stack.push(value);
      benchmark::DoNotOptimize(value);
      benchmark::ClobberMemory();
    }
  }
}

static void BM_Stack(benchmark::State &state) {
  size_t data_size = state.range(0);
  Stack stack(B, data_size);
  size_t d = static_cast<size_t>(IMAX / (data_size + 1));
  for (auto _ : state) {
    size_t value = 0;
    for (size_t i = 0; i < data_size; ++i) {
      value += d;
      stack.push(value);
      benchmark::DoNotOptimize(value);
      benchmark::ClobberMemory();
    }
  }
}

static void BM_Vector(benchmark::State &state) {
  size_t data_size = state.range(0);
  std::vector<size_t> vec;
  vec.reserve(data_size);
  for (auto _ : state) {
    for (size_t i = 0; i < data_size; ++i) {
      size_t value = i;
      vec.push_back(value);
      benchmark::DoNotOptimize(value);
      benchmark::ClobberMemory();
    }
  }
}

BENCHMARK(BM_StandardStack)
    ->RangeMultiplier(2)
    ->Range(1uz << 19uz, 1uz << 25uz)
    // ->Args({1uz << 22uz})
    ->Complexity();

BENCHMARK(BM_Stack)
    ->RangeMultiplier(2)
    ->Range(1uz << 19uz, 1uz << 25uz)
    // ->Args({1uz << 22uz})
    ->Complexity();
BENCHMARK(BM_Vector)
    ->RangeMultiplier(2)
    ->Range(1uz << 19uz, 1uz << 25uz)
    // ->Args({1uz << 22uz})
    ->Complexity();

BENCHMARK_MAIN();
