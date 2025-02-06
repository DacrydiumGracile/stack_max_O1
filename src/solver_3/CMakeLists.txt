add_library(solver_3 OBJECT solution.cpp)
target_include_directories(solver_3 PUBLIC . "../base_stack")

add_subdirectory(tests)
add_subdirectory(benchmarks)