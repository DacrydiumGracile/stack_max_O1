В один из вечеров, когда я читал различные статьи на Хабре, я наткнулся на размышления о задаче ["стек с поиском максимума"](https://habr.com/ru/articles/843596/) от [Ивана Бессонова](https://habr.com/ru/users/ibessonov/). Автор рассматривает проблемы, которые имеют "стандартные" решения в сети. А также задаётся вопросом, как избежать проблем с переполнением и суметь решить поставленную задачу, не вводя дополнительные ограничения на данные, которые мы добавляем в стек. В этом небольшой черновике я приведу одно из возможных решений, которое основывается на модификации одного из вариантов решений из статьи автора.  

### Различные части проекта

1. [Поиск решения и математические выкладки](doc/latex/main.pdf)
    - файл с основной реализацией [solvers/solver_3/solution.cpp](solvers/solver_3/solution.cpp)
2. [Описание процедуры проверки корректности](doc/tests.md)

### Сборба проекта

Для сборки проекта потребуется: [cmake](https://cmake.org/), [googletest](https://github.com/google/googletest) и [google/benchmark](https://github.com/google/benchmark).

```bash
$ cmake -S . -B ./build -DCMAKE_BUILD_TYPE=Release
$ cmake --build ./build
```

Запуск тестов, например:
```bash
$ ctest -V --test-dir ./build/solver_3/tests/
или
$ ./build/solver_3/tests/user_test
```

Запуск замеров
```bash
$ ./build/benchmarks/benchmark_1 --benchmark_enable_random_interleaving=true  --benchmark_repetitions=25 > ./results/benchmark_1.txt 2>&1

$ ./build/benchmarks/benchmark_2 --benchmark_enable_random_interleaving=true --benchmark_repetitions=25  > ./results/benchmark_2.txt 2>&1

$ ./build/benchmarks/benchmark_3 --benchmark_enable_random_interleaving=true --benchmark_repetitions=10  > ./results/benchmark_3.txt 2>&1
```
или
```bash
$ ./run_solver1_benchmark.sh
```

Также смотреть `./run_valgrind.sh` для запуска `massif` и `ms_print`.