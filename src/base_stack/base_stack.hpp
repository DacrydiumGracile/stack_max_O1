#include <forward_list>
#include <stack>
using elem_t = size_t;

class Base_Stack {
public:
  std::forward_list<elem_t> stack;

  void push(const elem_t &value) { stack.push_front(value); }
  bool empty() { return stack.empty(); }

  elem_t pop() {
    elem_t element = stack.front();
    stack.pop_front();
    return element;
  }

  elem_t top() { return stack.front(); }
};
