
### Реализация в коде

Чтобы иметь возможность задавать `B`, тем самым "выбирать" тип `uintB`, давайте просто считать, что типы элементов и счётчиков в нашей программе -- это `size_t`, причём будем предпологать, что `size_t` -- 64 битный. Конечно, НЕ следует использовать данные реализации где-либо кроме каких-то демонстрационных проектов. 

Рассмотрим `./solver_1/solution.hpp`:
```cpp
#include <vector>

size_t log2(size_t x);

class Stack {
private:
  // Количество бит для чисел в типе. Для unit4 равно4
  // для обычного unsigned int равно 32 может быть 64
  size_t B = 0;
  size_t M = 0;
  // Создаём вспомогательный вектор с счётчиками
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
  // Возвращает значение последнего элемента в стеке, которое может быть закодировано.
  size_t top();

  size_t max(); // Вызов max для пустого контейнера возвращает 0.
  size_t size();
  size_t capacity();
  bool is_empty();
};
```
Из особенностей -- это `block_id`, чтобы не подсчитывать `id` блока для текущего максимума, который может часто повторяться, мы его запоминаем. (Поскольку отсчёт начинается `0`, то мы будем работать с `id`, а не номерами блока.) Рассмотрим `./solver_1/solution.cpp`
```cpp
Stack::Stack(size_t B_, size_t length_)
    : B{B_},
      // Максимальное число для нашего типа 2^B  - 1 (для unit4 равное 15)
      M{(B == 64) ? ~0uz : (1uz << B) - 1uz}, count_elements(B_ + 1uz, 0uz) {

  stack.reserve(length_);
}
```
В конструктуре по заданному `B_` создаём нужное количество счётчиков и инициализируем их нулями, а также вычисляем `M`, причём, чтобы избежать переполнения в `1uz << B` при $B = 64$ мы рассматриваем этот случай отдельно. Очевидно, что при передаче `B` большего чем размер `size_t` мы получим UB. Чтобы при замерах не тратить время на увеличение вектора, мы можем сразу передать заданную длину `length_` и выполнить `reserve`.

```cpp
size_t log2(size_t x) {
  [[assume(x != 0)]];
  return std::bit_width(x) - 1; // bit_width вернёт int
  // см. https://en.cppreference.com/w/cpp/numeric/bit_width
}

size_t Stack::calculate_block_id(size_t value) {
  // Вычисляем какому из блоково принадлежит число value:
  if (value == M) {
    // Максимальное число мы обрабатываем отдельно
    return B; // последний из возможных id блока
  } else {
    // Поскольку value < M, то значит M - value != 0, поэтому законно использовать log2 с указанным assume
    return (B - 1 - log2(M - value));
  }
}
```

Реализация основных двух функций:
```cpp
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
      stack.push_back(2 * value - max_value);
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
```
Заметим, поскольку изначально при пустом стеке `max_value = 0`, то случая добавления первого элемента корректен.
```cpp
size_t Stack::pop() {
  // Вызов pop для пустого контейнера приводит к неопределенному поведению.
  // Мы знаем текущий max_value, его block_id и верхний элемент стека,
  // кототрый может быть: 1) предыдущим max, 2) истинным добавленным 
  // значением, 3) значением, для определения предыдущего max.
  size_t element = stack.back(); // <- stack.back() -- здесь это vector.back() не очень понятное наименование, поскольку можно спутать с Stack::back()...
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
      max_value = 2 * max_value - element;
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
```
Остальные методы -- это просто вызовы методов `vector`.
```cpp
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
```