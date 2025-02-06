#pragma once

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
