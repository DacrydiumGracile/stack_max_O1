### Тесты
Сравнивать результаты мы будем с реализацией требующей `O(n)` дополнительной памяти. Мы просто будем хранить пары, в которых перевый элемент -- это текущий максимум, а второй элемент -- добавленный элемент. `./tests/standard_solution.hpp`:
```cpp
#include <vector>

class StandardStack {
private:
  std::vector<std::pair<size_t, size_t>> stack;

public:
  StandardStack(size_t length_);

  void push(size_t value);
  size_t pop();

  size_t back();

  size_t max();
  size_t size();
  size_t capacity();
  bool is_empty();
};
```
`./tests/standard_solution.cpp`:
```cpp
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
```

Входными данными для тестов будут файлы `.dat`, которые должны быть помещены в `./tests/data/`. Они должны быть сформированы определённым образом. Например, для примера, который мы рассматривали руками выше:
```txt
4 8
a 0
a 3
a 10
a 12
a 15
a 15
a 1
p 
p
p
p
p
p
p
```
В первой строке указаны два числа: `B` и `length`, которые будут переданы в конструктор `Stack`. Затем идут струки следующих типов:
1. `a число`, что означает `push число`;
2. `p`, что означает `pop`. 

Очевидно, что следующий входной файл будет некорректным:
```
4 8
a 0
p
p
```
поскольку после первого `p` стек будет пустой. 

Чтобы проверить входные данные можно воспользоваться небольшим скриптом `validate_test_data.py`:
```txt
$ python ./validate_test_data.py -h
usage: validate_test_data.py [-h] [-d path] [-f path]
                             [-r path]

options:
  -h, --help       show this help message and exit
  -d, --dir path   Проверить все файлы из указанной
                   директории. Путь указывается
                   относительно данного скрипта.
  -f, --file path  Проверить указанный файл. Путь
                   указывается относительно данного
                   скрипта.
  -r path          Проверить файлы, пути к которым указаны
                   в path-файле. Путь указывается
                   относительно данного скрипта.
```
Например:
```txt
$ python ./validate_test_data.py -f ./tests/data/example_1.dat 
INFO: Тестовые данные файла '.../tests/data/example_1.dat' корректны.
```
Для генерации различных тестовых данных следует использовать `generate_data.py`:
```
$ python ./generate_data.py -h
usage: generate_data.py [-h] [-n number] [-B number]
                        [-s number] [-o path]

options:
  -h, --help         show this help message and exit
  -n number          Количество повторных запусков.
  -B number          Количество бит. Указывает, что
                     генерируемые числа не могут быть
                     больше чем 2^B
  -s, --size number  Размер стека.
  -o, --output path  Файл сохранения. Путь указывается
                     относительно данного скрипта.
```
Например, мы хотим сгенерировать данные для `uint4`, а именно, пусть в стек кладётся `20` произвольных чисел, а затем все $20$ чисел извлекаются, и так повторяется $50000$ раз. После каждого добавления и после каждого извлечения (то есть $20\cdot50000$ раз) проверяется максимальные элеменет и извлечённый элемент. Тога мы генерируем данные следующим образом:
```bash
$ python ./generate_data.py -o ./tests/data/test_7.dat -n 50000 -s 20 -B 4
```
при этом в `./tests/data/test_7.dat` будет:
```
4 20
a 8
a 14
...
a 11
a 6
p
p
...
p
p
a 10
a 15
...
a 9
a 11
a 7
p
...
p
p
...
```
ровно $200001$ строк. 

Реализацию самих тестов можно посмотреть в `./tests/user_test.cpp` с подробными комментариями. 

Примечание. Если после сборки проекта, вы добавляете новый файл с тестовыми данными, то не требуется повторной пересборки `user_tests`, достаточно выполнить конфигурирование `cmake`: `cmake -S . -B ./build`, для корректной работы `ctest`. Либо просто запускать напрямую `user_test`. 
