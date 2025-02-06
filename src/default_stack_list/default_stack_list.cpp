#include <default_stack_list.hpp>

Default_Stack::~Default_Stack() {
  while (top) {
    Node *temp = top;
    top = top->next;
    delete temp;
  }
}

bool Default_Stack::is_empty() const { return top == nullptr; }

void Default_Stack::push(elem_t value) { top = new Node(value, top); }

elem_t Default_Stack::pop() {
  elem_t value = top->data;
  Node *temp = top;
  top = top->next;
  delete temp;
  return value;
}

elem_t Default_Stack::back() const { return top->data; }