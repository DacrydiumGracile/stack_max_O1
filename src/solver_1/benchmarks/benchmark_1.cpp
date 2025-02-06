#include "solution.hpp"
#include "standard_solution.hpp"

#include <algorithm>
#include <benchmark/benchmark.h>
#include <chrono>
#include <cstring>
#include <random>
#include <vector>

size_t B = 32uz;
size_t IMAX = (1uz << B) - 1uz;

static void BM_standard_stack_random_push(benchmark::State &state) {
  // Создадим и заполним вектор случайными целыми числами.
  // чтобы при замерах push в стек, брать числа максимально быстро и вектора.
  size_t data_size = state.range(0);
  std::vector<size_t> data;
  data.reserve(data_size);

  auto now = std::chrono::high_resolution_clock::now();
  std::mt19937 generator(now.time_since_epoch().count());
  std::uniform_int_distribution<size_t> distribution(0, IMAX);
  for (size_t i = 0; i < data_size; ++i) {
    data.emplace_back(distribution(generator));
  }

  StandardStack stack(data_size);
  for (auto _ : state) {
    // Этот код замеряем
    for (size_t i = 0; i < data_size; ++i) {
      size_t value = data[i];
      stack.push(value);
      benchmark::DoNotOptimize(value);
      benchmark::ClobberMemory();
    }
  }
}

static void BM_stack_random_push(benchmark::State &state) {
  size_t data_size = state.range(0);
  std::vector<size_t> data;
  data.reserve(data_size);

  auto now = std::chrono::high_resolution_clock::now();
  std::mt19937 generator(now.time_since_epoch().count());
  std::uniform_int_distribution<size_t> distribution(0, IMAX);
  for (size_t i = 0; i < data_size; ++i) {
    data.emplace_back(distribution(generator));
  }

  Stack stack(B, data_size);
  for (auto _ : state) {
    // Этот код замеряем
    for (size_t i = 0; i < data_size; ++i) {
      size_t value = data[i];
      stack.push(value);
      benchmark::DoNotOptimize(value);
      benchmark::ClobberMemory();
    }
  }
}

static void BM_vector_random_push(benchmark::State &state) {
  size_t data_size = state.range(0);
  std::vector<size_t> data;
  data.reserve(data_size);

  auto now = std::chrono::high_resolution_clock::now();
  std::mt19937 generator(now.time_since_epoch().count());
  std::uniform_int_distribution<size_t> distribution(0, IMAX);
  for (size_t i = 0; i < data_size; ++i) {
    data.emplace_back(distribution(generator));
  }

  std::vector<size_t> vec;
  vec.reserve(data_size);
  for (auto _ : state) {
    // Этот код замеряем
    for (size_t i = 0; i < data_size; ++i) {
      size_t value = data[i];
      vec.push_back(value);
      benchmark::DoNotOptimize(value);
      benchmark::ClobberMemory();
    }
  }
}

// Регистрируем функции для анализа
BENCHMARK(BM_standard_stack_random_push)
    ->RangeMultiplier(2)
    ->Range(1uz << 19uz, 1uz << 25uz)
    // ->Args({1uz << 22uz})
    ->Complexity();
BENCHMARK(BM_stack_random_push)
    ->RangeMultiplier(2)
    ->Range(1uz << 19uz, 1uz << 25uz)
    // ->Args({1uz << 22uz})
    ->Complexity();
BENCHMARK(BM_vector_random_push)
    ->RangeMultiplier(2)
    ->Range(1uz << 19uz, 1uz << 25uz)
    // ->Args({1uz << 22uz})
    ->Complexity();

BENCHMARK_MAIN();