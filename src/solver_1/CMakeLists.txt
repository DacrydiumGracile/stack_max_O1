
add_library(solver_1 OBJECT solution.cpp)
target_include_directories(solver_1 PUBLIC .)

    
add_subdirectory(tests)
add_subdirectory(benchmarks)
