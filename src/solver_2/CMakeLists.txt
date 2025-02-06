add_library(solver_2 OBJECT solution.cpp)
target_include_directories(solver_2 PUBLIC .)

add_subdirectory(tests)
add_subdirectory(benchmarks)