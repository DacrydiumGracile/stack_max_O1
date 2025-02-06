#include "solution.hpp"

size_t log2(size_t x) {
  [[assume(x != 0)]];
  return std::bit_width(x) - 1; // bit_width вернёт int
}

Stack::Stack(size_t B_, size_t length_)
    : B{B_},
      // Максимальное число для нашего типа 2^B  - 1 (для unit4 равное 15)
      M{(B == 64) ? ~0uz : (1uz << B) - 1uz}, count_elements(B_ + 1uz, 0uz) {

  stack.reserve(length_);
}

size_t Stack::calculate_block_id(size_t value) {
  // Вычисляем какому из блоково принадлежит число value:
  if (value == M) {
    // Максимальное число мы обрабатываем отдельно
    return B; // последний из возможных id блока
  } else {
    // Поскольку value < M, то значит M - value != 0, поэтому законно
    // использовать log2 с указанным assume
    return (B - 1uz - log2(M - value));
  }
}

void Stack::push(size_t value) {
  if (value <= max_value) {
    // В данном случае ни max_value, ни block_id не изменятся.
    // Увеличиваем количество элементов для блока, которому принадлежит
    // max_value.
    count_elements[block_id] += 1;
    stack.push_back(value);
  } else {
    // value > max_value, поэтому потребуется обновление max_value и возможно
    // block_id.
    size_t new_max_value = value;
    size_t new_block_id = calculate_block_id(new_max_value);
    if (new_block_id == block_id) {
      // Выполняем стандартное добавление, поскольку гарантировано не будет
      // переполнения.
      stack.push_back((value - max_value) + value); // 2*value - max_value
    } else {
      // Мы добавляем элемент из другого блока сохраняем старый max, как
      // элемент стека, чтобы при pop востановить его.
      stack.push_back(max_value);
    }
    count_elements[new_block_id] += 1;
    // Перезаписываем max_value и block_id
    max_value = new_max_value;
    block_id = new_block_id;
  }
}

size_t Stack::pop() {
  // Вызов pop для пустого контейнера приводит к неопределенному поведению.
  // Мы знаем текущий max_value, его block_id и верхний элемент стека,
  // кототрый может быть: предыдущим max, истинным добавленным значением,
  // значением, для определения предыдущего max.
  size_t element = stack.back();
  count_elements[block_id] -= 1;
  stack.pop_back();
  if (count_elements[block_id] == 0) {
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
      size_t res = max_value;
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
      size_t res = max_value;
      // востановим прошлый максимум по формуле 2 * max_value - element;
      // Чтобы не было переполнения в промежуточных шагах:
      size_t diff = element - max_value;
      // последнее корректно, так как в случае element > max_value

      // (max_value - diff =  max_value - (element - max_value) = 2*max_value -
      // element)
      max_value = max_value - diff;
      return res;
    }
  }
}

// Возвращает истинное значение добавленного элемента
size_t Stack::back() {
  // Вызов back для пустого контейнера вызывает неопределенное поведение.
  size_t element = stack.back();
  if (count_elements[block_id] == 1) {
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

// Возвращает значение последнего элемента в стеке, которое может быть
// закодировано.
size_t Stack::top() {
  // Вызов back для пустого контейнера вызывает неопределенное поведение.
  return stack.back();
}

size_t Stack::max() {
  // Вызов max для пустого контейнера возвращает 0.
  // Но не стоит этого делать!
  return max_value;
}

size_t Stack::size() { return stack.size(); }
size_t Stack::capacity() { return stack.capacity(); }

bool Stack::is_empty() { return stack.empty(); }
