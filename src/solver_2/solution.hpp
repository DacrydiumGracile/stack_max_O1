
#include <cstddef>
using elem_t = size_t; // Аналогично default_stack_list

// Определение структуры узла односвязного списка
// Аналогична, как и для обычного стека
struct Node {
  elem_t data;
  Node *next;

  Node(elem_t data_, Node *next_ = nullptr) : data{data_}, next{next_} {}
};

class Stack {
private:
  // Количество бит для чисел в типе. Для unit4 равно4
  // для обычного unsigned int равно 32 может быть 64
  elem_t B = 0;
  elem_t M = 0;

  Node **stacks; // B + 1 будующих стеков

  elem_t max_value = 0; // текущий максимум стека
  // текущий id блока, которому принадлежит max_value
  elem_t block_id = 0;

  elem_t calculate_block_id(elem_t value);

public:
  // Конструктор
  Stack(elem_t B);

  // Деструктор
  ~Stack();

  // Проверка на пустоту стека
  bool is_empty(elem_t id) const;
  bool is_all_empty() const;

  elem_t max() const;

  // Добавление элемента в стек
  void push(elem_t value);

  // Удаление элемента из стека
  elem_t pop();

  // Получение верхнего элемента стека без удаления
  elem_t back() const;
};
