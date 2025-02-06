#include "standard_solution.hpp"

StandardStack::StandardStack(size_t length_) { stack.reserve(length_); }

void StandardStack::push(size_t value) {
  if (stack.empty()) {
    // если стек пуст, добавляем пару с максимумом и значением
    stack.emplace_back(value, value);
  } else {
    // если стек не пуст, добавляем пару с новым максимумом и значением
    size_t new_max = std::max(stack.back().first, value);
    stack.emplace_back(new_max, value);
  }
}

size_t StandardStack::pop() {
  // Вызов pop для пустого контейнера приводит к неопределенному поведению.
  size_t value = stack.back().second;
  stack.pop_back();
  return value;
}

size_t StandardStack::back() {
  // Вызов back для пустого контейнера приводит к неопределенному поведению.
  return stack.back().second;
}

size_t StandardStack::max() {
  // Вызов max для пустого контейнера приводит к неопределенному поведению.
  return stack.back().first;
}

size_t StandardStack::size() { return stack.size(); }
size_t StandardStack::capacity() { return stack.capacity(); }

bool StandardStack::is_empty() { return stack.empty(); }