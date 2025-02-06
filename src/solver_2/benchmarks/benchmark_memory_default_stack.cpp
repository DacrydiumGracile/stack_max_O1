#include "default_stack_list.hpp"
#include <cassert>
#include <chrono>
#include <random>

elem_t B = {sizeof(elem_t) * elem_t(8)};
elem_t IMAX = {(B == sizeof(elem_t) * elem_t(8))
                   ? ~elem_t(0)
                   : (elem_t(1) << B) - elem_t(1)};

elem_t calculate_m(elem_t id) {
  // Вычисляем m_n -- последний элемент блока, для uint4: m_0 = 0,
  // m_1 = 7, m_2 = 11, m_3 = 13, m_4 = 14, m_5 = 15; см. pdf
  assert((id <= B + elem_t(1)) && "Error. Incorrect B value.");
  if (id == B + elem_t(1)) {
    // Максимальное число мы обрабатываем отдельно
    return IMAX;
  } else if (id == B) {
    return IMAX - 1;
  } else if (id == elem_t(0)) {
    return 0;
  } else {
    // 1 <= id <= B - 1:  m_id = 2^(B-id) * (2^id - 1) - 1
    return (elem_t(1) << (B - id)) * ((elem_t(1) << id) - elem_t(1)) -
           elem_t(1);
  }
}

void test_memory_default_stack_list_push(size_t start, size_t end) {
  auto now = std::chrono::high_resolution_clock::now();
  std::mt19937 generator(now.time_since_epoch().count());
  for (size_t i = start; i <= end; ++i) {
    size_t data_size = (1uz << i); // Количество элементов
    size_t count = 0uz;
    Default_Stack *stack = new Default_Stack();

    // Добавим сперва data_size/(B+1) элементов из первого блока
    // затем из второго и так далее
    for (elem_t id = 1uz; id <= B; ++id) {
      size_t m = {calculate_m(id)};
      std::uniform_int_distribution<elem_t> distribution(0, m);
      for (size_t k = 0uz; k <= data_size / (B + 1uz); ++k) {
        stack->push(distribution(generator));
        count++;
      }
    }
    size_t m = {calculate_m(B + elem_t(1))};
    std::uniform_int_distribution<elem_t> distribution(0, m);
    // последний блок заполняем оставшимся элементами
    while (count <= data_size) {
      stack->push(distribution(generator));
      count++;
    }
    // Уничтожаем stack, тем самым освобождая
    delete stack;
  }
}

int main() { test_memory_default_stack_list_push(10uz, 15uz); }