#include "solution.hpp"
#include <bit> // std::bit_width

Stack::Stack() : stacks(new Base_Stack[B + 1]) {}
Stack::~Stack() { delete[] stacks; }
// Stack::~Stack() {}
elem_t log2(elem_t x) {
  [[assume(x != elem_t(0))]];
  return std::bit_width(x) - 1; // bit_width вернёт int
}

elem_t Stack::calculate_block_id(elem_t value) {
  // Вычисляем какому из блоково принадлежит число value:
  if (value == M) {
    // Максимальное число мы обрабатываем отдельно
    return B; // последний из возможных id блока
  } else {
    // Поскольку value < M, то значит M - value != 0, поэтому законно
    // использовать log2 с указанным assume
    return (B - elem_t(1) - log2(M - value));
  }
}

bool Stack::block_is_empty(elem_t id) { return (stacks[id].empty()); }

bool Stack::empty() { return ((block_id == 0) && (stacks[0].empty())); }

elem_t Stack::max() { return max_value; }

void Stack::push(elem_t value) {
  if (value <= max_value) {
    // В данном случае ни max_value, ни block_id не изменятся.
    stacks[block_id].push(value); // вызываем Base_Stack::push
  } else {
    // value > max_value, поэтому потребуется обновление max_value и возможно
    // block_id.
    elem_t new_max_value = value;
    elem_t new_block_id = calculate_block_id(new_max_value);
    if (new_block_id == block_id) {
      // Выполняем стандартное добавление (по формуле 2*value - max_value),
      // поскольку гарантировано не будет переполнения.
      // вызываем Base_Stack::push
      stacks[block_id].push(((value - max_value) + value));
    } else {
      // Мы добавляем элемент из другого блока сохраняем старый max, как
      // последний элемент (new_block_id - 1), чтобы при pop востановить его.
      // (new_block_id - 1) >= 0, так как new_block_id >= 1, в данной ветке.
      // вызываем Base_Stack::push
      stacks[new_block_id - elem_t(1)].push(max_value);
    }
    // Перезаписываем max_value и block_id
    max_value = new_max_value;
    block_id = new_block_id;
  }
}

elem_t Stack::pop() {
  // Мы знаем текущий max_value, его block_id и верхний элемент стека,
  // кототрый может быть: предыдущим max, истинным добавленным значением,
  // значением, для определения предыдущего max.
  bool block_empty = stacks[block_id].empty();
  if (block_empty && (block_id != 0)) {
    // Блок пустой и block_id != 0, значит текущий max -- это последний элемент,
    // который необходимо вернуть, а в предыдущем блоке на вершине лежит старый
    // max.
    elem_t res = max_value;
    elem_t prev_max =
        stacks[block_id - elem_t(1)].pop(); // Вызываем Base_Stack::pop
    block_id = calculate_block_id(prev_max);
    max_value = prev_max;
    return res;
  }
  // Далее block_empty = false, поскольку если здесь он равен true, то значит
  // block_empty && (block_id == 0) == true, но тогда это означает, что мы
  // пытаемся извлечь из пустого стека. Здесь можно либо выбросить исключение,
  // либо как-то иначе обрабатывать данную ошибку. Мы же просто скажем, что это
  // UB

  // возвращаем не последний элемент из блока
  elem_t element = stacks[block_id].pop(); // Вызываем Base_Stack::pop
  if (element <= max_value) {
    // element -- истинное добавленное значение
    return element;
  } else {
    // element -- значение, которое будет использоваться, для вычисления
    // прошлого max_value, а текущее max_value -- это истинное добавленное
    // значение,
    elem_t res = max_value;
    // востановим прошлый максимум по формуле 2 * max_value - element;
    // Чтобы не было переполнения в промежуточных шагах:
    elem_t diff = element - max_value;
    // последнее корректно, так как в случае element > max_value

    // (max_value - diff =  max_value - (element - max_value) = 2*max_value
    // - element)
    max_value = max_value - diff;
    return res;
  }
}

elem_t Stack::top() {
  bool block_empty = stacks[block_id].empty();
  if (block_empty && (block_id != 0)) {
    // Блок пустой и block_id != 0, значит текущий max -- это последний элемент,
    // который необходимо вернуть
    return max_value;
  }
  // Опять же, если block_empty == true и block_id == 0, то тогда мы пытаемся
  // узнать элемент пустого стека. Это можно обработать в ветви else. Но мы
  // скажем, что это просто UB.
  // Далее block_empty == false
  elem_t element = stacks[block_id].top(); // Вызываем Base_Stack::top

  if (element <= max_value) {
    // element -- истинное добавленное значение
    return element;
  } else {
    // element -- значение, которое будет использоваться, для вычисления
    // прошлого max_value, а текущее max_value -- это истинное добавленное
    // значение,
    return max_value;
  }
}