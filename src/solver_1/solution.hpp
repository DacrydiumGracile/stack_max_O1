#pragma once

#include <vector>

size_t log2(size_t x);

class Stack {
private:
  // Количество бит для чисел в типе. Для unit4 равно4
  // для обычного unsigned int равно 32 может быть 64
  size_t B = 0;
  size_t M = 0;
  // Создаём вспомогательный вектор с счётчиками Подробнее читать в pdf.
  std::vector<size_t> count_elements;
  std::vector<size_t> stack;
  size_t max_value = 0; // текущий максимум стека
  // текущий id блока, которому принадлежит max_value
  size_t block_id = 0;

  size_t calculate_block_id(size_t value);

public:
  Stack(size_t B_, size_t length_);

  void push(size_t value);
  size_t pop();

  // Возвращает истинное значение добавленного элемента
  size_t back();
  // Возвращает значение последнего элемента в стеке, которое может быть
  // закодировано.
  size_t top();

  size_t max();
  size_t size();
  size_t capacity();
  bool is_empty();
};
