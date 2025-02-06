#include "base_stack.hpp"

class Stack {
private:
  elem_t B = {sizeof(elem_t) * elem_t(8)};
  elem_t M = {~elem_t(0)};

  elem_t max_value = 0; // текущий максимум стека
  // текущий id блока, которому принадлежит max_value
  elem_t block_id = 0;

  Base_Stack *stacks; // B + 1 будующих стеков

  elem_t calculate_block_id(elem_t value);
  bool block_is_empty(elem_t id);

public:
  // Конструктор и деструктор
  Stack();
  ~Stack();

  // Проверка на пустоту стека
  bool empty();

  elem_t max();

  // Добавление элемента в стек
  void push(elem_t value);

  // Удаление элемента из стека
  elem_t pop();

  // Получение верхнего элемента стека без удаления
  elem_t top();
};