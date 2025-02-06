#include "solution.hpp"
#include <bit>

elem_t log2(elem_t x) {
  [[assume(x != 0)]];
  return std::bit_width(x) - 1; // bit_width вернёт int
}

Stack::Stack(elem_t B_)
    : B{B_},
      M{(B == sizeof(elem_t) * elem_t(8)) ? ~elem_t(0)
                                          : (elem_t(1) << B) - elem_t(1)},
      stacks(new Node *[B_ + elem_t(1)]) {
  for (elem_t id = 0; id < B + elem_t(1); ++id) {
    stacks[id] = nullptr;
  }
}

Stack::~Stack() {
  for (elem_t id = 0; id <= block_id; ++id) {
    while ((stacks[id] != nullptr)) {
      Node *temp = stacks[id]; // stacks[id] -- это top для stack с id = i.
      stacks[id] = stacks[id]->next;
      delete temp;
    }
  }
  delete[] stacks;
}

elem_t Stack::calculate_block_id(elem_t value) {
  // Вычисляем какому из блоково принадлежит число value:
  if (value == M) {
    // Максимальное число мы обрабатываем отдельно
    return B; // последний из возможных id блока
  } else {
    // Поскольку value < M, то значит M - value != 0, поэтому законно
    // использовать log2 с указанным assume
    return (B - 1 - log2(M - value));
  }
}

bool Stack::is_empty(elem_t id) const { return (stacks[id] == nullptr); }

bool Stack::is_all_empty() const {
  return ((block_id == 0) && (stacks[0] == nullptr));
}

elem_t Stack::max() const { return max_value; }

void Stack::push(elem_t value) {
  if (value <= max_value) {
    // В данном случае ни max_value, ни block_id не изменятся.
    stacks[block_id] = new Node(value, stacks[block_id]);
  } else {
    // value > max_value, поэтому потребуется обновление max_value и возможно
    // block_id.
    elem_t new_max_value = value;
    elem_t new_block_id = calculate_block_id(new_max_value);
    if (new_block_id == block_id) {
      // Выполняем стандартное добавление (по формуле 2*value - max_value),
      // поскольку гарантировано не будет переполнения.
      stacks[block_id] =
          new Node(((value - max_value) + value), stacks[block_id]);
    } else {
      // Мы добавляем элемент из другого блока сохраняем старый max, как первый
      // элемент нового блока, чтобы при pop востановить его.
      stacks[new_block_id] = new Node(max_value, stacks[new_block_id]);
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
  Node *last_node = stacks[block_id];
  elem_t element = last_node->data;
  Node *previous_node = last_node->next;
  stacks[block_id] = previous_node;
  delete last_node;

  if (previous_node == nullptr) {
    // Мы удаляем последний элемент для данного блока, то есть element --
    // это либо прошлый max, либо если block_id = 0, то значит мы удалили
    // последний элемент всего стека.
    if (block_id == 0) {
      // element -- последний элемент всего стека, а значит текущий max_value и
      // есть сам элемент. Поскольку нам надо изменить max_value в изначальное
      // значение, равное 0, то сохраним текущее значение в element
      element = max_value;
      max_value = 0;
      return element;
    } else {
      // element -- прошлый max, а возвращаемый элемент -- это текущий max.
      elem_t res = max_value;
      max_value = element;
      block_id = calculate_block_id(element);
      return res;
    }
  } else {
    // возвращаем не последний элемент из блока
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

      // (max_value - diff =  max_value - (element - max_value) = 2*max_value -
      // element)
      max_value = max_value - diff;
      return res;
    }
  }
}

elem_t Stack::back() const {
  Node *last_node = stacks[block_id];
  elem_t element = last_node->data;
  Node *previous_node = last_node->next;

  if (previous_node == nullptr) {
    // Если в блоке всего одно число, значит текущий max_value
    // и есть последний элемент в массиве
    return max_value;
  } else {
    // в блоке больше чем одно число
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
}