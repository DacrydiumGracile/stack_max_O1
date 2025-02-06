
// Для наглядности, чтобы понимать по коду, где подразумевается типа
// элемента структуры (какой-то конечный безнаковый). Мы положим
// size_t, но аргументом B будем регулировать.
#include <cstddef>
using elem_t = size_t;

// Определение структуры узла односвязного списка
struct Node {
  elem_t data;
  Node *next;
  Node(elem_t data_, Node *next_ = nullptr) : data{data_}, next{next_} {}
};

class Default_Stack {
private:
  Node *top; // Верхний элемент стека

public:
  // Конструктор
  Default_Stack() : top(nullptr) {}

  // Деструктор
  ~Default_Stack();

  // Проверка на пустоту стека
  bool is_empty() const;

  // Добавление элемента в стек
  void push(elem_t value);

  // Удаление элемента из стека
  elem_t pop();

  // Получение верхнего элемента стека без удаления
  elem_t back() const;
};
