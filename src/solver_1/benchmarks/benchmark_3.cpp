#include "solution.hpp"
#include "standard_solution.hpp"

#include <algorithm>
#include <benchmark/benchmark.h>
#include <chrono>
#include <cstring>
#include <random>
#include <vector>

size_t B = 64uz;
size_t IMAX = (1uz << B) - 1uz;

static void BM_standard_stack_random_back_pop(benchmark::State &state) {
  size_t data_size = state.range(0);

  auto now = std::chrono::high_resolution_clock::now();
  std::mt19937 generator(now.time_since_epoch().count());
  std::uniform_int_distribution<size_t> distribution(0, IMAX);

  StandardStack stack(data_size);
  for (auto _ : state) {
    state.PauseTiming();
    for (size_t i = 0; i < data_size; ++i) {
      stack.push(distribution(generator));
    }
    state.ResumeTiming();
    // Этот код замеряем
    size_t value_back = 0;
    size_t value_pop = 0;
    for (size_t i = 0; i < data_size; ++i) {
      value_back = stack.back();
      benchmark::DoNotOptimize(value_back);
      value_pop = stack.pop();
      benchmark::DoNotOptimize(value_pop);
      benchmark::ClobberMemory();
    }
  }
}

static void BM_stack_random_back_pop(benchmark::State &state) {
  size_t data_size = state.range(0);

  auto now = std::chrono::high_resolution_clock::now();
  std::mt19937 generator(now.time_since_epoch().count());
  std::uniform_int_distribution<size_t> distribution(0, IMAX);

  Stack stack(B, data_size);
  for (auto _ : state) {
    state.PauseTiming();
    for (size_t i = 0; i < data_size; ++i) {
      stack.push(distribution(generator));
    }
    state.ResumeTiming();
    // Этот код замеряем
    size_t value_back = 0;
    size_t value_pop = 0;
    for (size_t i = 0; i < data_size; ++i) {
      value_back = stack.back();
      benchmark::DoNotOptimize(value_back);
      value_pop = stack.pop();
      benchmark::DoNotOptimize(value_pop);
      benchmark::ClobberMemory();
    }
  }
}

static void BM_vector_random_back_pop(benchmark::State &state) {
  size_t data_size = state.range(0);

  auto now = std::chrono::high_resolution_clock::now();
  std::mt19937 generator(now.time_since_epoch().count());
  std::uniform_int_distribution<size_t> distribution(0, IMAX);

  std::vector<size_t> vec;
  vec.reserve(data_size);
  for (auto _ : state) {
    state.PauseTiming();
    for (size_t i = 0; i < data_size; ++i) {
      vec.emplace_back(distribution(generator));
    }
    state.ResumeTiming();
    // Этот код замеряем
    size_t value_back = 0;
    for (size_t i = 0; i < data_size; ++i) {
      value_back = vec.back();
      benchmark::DoNotOptimize(value_back);
      vec.pop_back();
      benchmark::ClobberMemory();
    }
  }
}

// Регистрируем функции для анализа
BENCHMARK(BM_standard_stack_random_back_pop)
    ->RangeMultiplier(2)
    ->Range(1uz << 19uz, 1uz << 24uz)
    // ->Args({1uz << 22uz})
    ->Complexity();
BENCHMARK(BM_stack_random_back_pop)
    ->RangeMultiplier(2)
    ->Range(1uz << 19uz, 1uz << 24uz)
    // ->Args({1uz << 22uz})
    ->Complexity();
BENCHMARK(BM_vector_random_back_pop)
    ->RangeMultiplier(2)
    ->Range(1uz << 19uz, 1uz << 24uz)
    // ->Args({1uz << 22uz})
    ->Complexity();

BENCHMARK_MAIN();