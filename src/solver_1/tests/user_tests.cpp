#include <filesystem>
#include <fstream>
#include <memory>

#include "config.hpp"
#include "solution.hpp"
#include "standard_solution.hpp"
#include "gtest/gtest.h"

std::vector<std::string> getTestFiles() {
  std::vector<std::string> files;
  for (const auto &entry : std::filesystem::directory_iterator(TEST_DATA_DIR)) {
    if (entry.path().extension() == ".dat") {
      files.emplace_back(entry.path().string());
    }
  }
  return files;
}

class StackTest : public testing::Test,
                  public ::testing::WithParamInterface<std::string> {
protected:
  void SetUp() override {
    path = {GetParam()};
    file.open(path);
    if (!file.is_open()) {
      FAIL() << "Failed to open the file '" + path + "'.";
    }
    size_t count_elements;
    size_t B;
    if (!(file >> B)) {
      FAIL() << "Error reading from file '" + path +
                    "'. The 'B' value is not correct.";
    }

    if (!(file >> count_elements)) {
      FAIL() << "Error reading from file '" + path +
                    "'. The 'count_elements' value is not correct.";
    }

    Stack_ptr = std::make_unique<Stack>(B, count_elements);
    StandardStack_ptr = std::make_unique<StandardStack>(count_elements);
  };

  std::unique_ptr<Stack> Stack_ptr;
  std::unique_ptr<StandardStack> StandardStack_ptr;
  std::ifstream file;
  std::string path;
};

TEST_P(StackTest, Try1) {
  // Мы создали пустые стеки. Далее выполняем чтение команд из file.
  // В файле могут быть всего две команды: 'a' число (= push число) и 'p' (=
  // pop). После выполнения любой из этих команд max и элемент на вершине должны
  // быть одинаковыми в обоих стеках.
  // Соответственно тест будет пройден после корректной обработки всех команд из
  // файла.
  size_t Stack_size = Stack_ptr->size();
  size_t StandardStack_size = StandardStack_ptr->size();
  ASSERT_EQ(Stack_size, 0);
  ASSERT_EQ(StandardStack_size, 0);
  char command;
  while (file >> command) {
    if (command == 'p') {
      // pop. После выполнения мы должны проверить, что мы извлекли одинаковые
      // элементы
      size_t Stack_value = Stack_ptr->pop();
      size_t StandardStack_value = StandardStack_ptr->pop();

      ASSERT_EQ(Stack_value, StandardStack_value);
      // Если стек не пуст, то мы также должны убедиться, что max правильно
      // изменился.
      Stack_size = Stack_ptr->size();
      StandardStack_size = StandardStack_ptr->size();
      ASSERT_EQ(Stack_size, StandardStack_size);
      if (Stack_size != 0) {
        size_t Stack_max = Stack_ptr->max();
        size_t StandardStack_max = StandardStack_ptr->max();
        ASSERT_EQ(Stack_max, StandardStack_max);
      }
    } else if (command == 'a') {
      // push number. Читаем число
      size_t number;
      if (!(file >> number)) {
        FAIL() << "Error reading from file '" + path + "'.";
      }
      Stack_ptr->push(number);
      StandardStack_ptr->push(number);
      // Проверяем, что после добавления max и значение на вершине одинаковые
      size_t Stack_value = Stack_ptr->back();
      size_t Stack_max = Stack_ptr->max();
      size_t StandardStack_value = StandardStack_ptr->back();
      size_t StandardStack_max = StandardStack_ptr->max();

      ASSERT_EQ(Stack_value, StandardStack_value);
      ASSERT_EQ(Stack_max, StandardStack_max);
    } else {
      // Задана не известная команда, убедитесь в корректности тестовых данных
      // при помощи 'validate_test_data'
      FAIL() << "Error. Incorrect test data from file '" + path + "'.";
    }
  }
  // Если мы вышли из цикла, то либо достигли конца файла, либо произошла ошибка
  if (!file.eof()) {
    // Произошла ошибка
    FAIL() << "Error reading from file '" + path + "'.";
  }
  // Достигли конца файла. Убедимся, что после выполнения всех команд размеры
  // стеков одинаковые. (Правильность max и вершины были проверены в цикле)
  Stack_size = Stack_ptr->size();
  StandardStack_size = StandardStack_ptr->size();
  ASSERT_EQ(Stack_size, StandardStack_size);
}

INSTANTIATE_TEST_CASE_P(StackTests, StackTest,
                        ::testing::ValuesIn(getTestFiles()));

int main(int argc, char **argv) {
  std::cerr << "Value TEST_DATA_DIR = '" << TEST_DATA_DIR << "'." << std::endl;
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}