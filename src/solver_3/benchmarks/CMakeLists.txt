find_package(benchmark REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g3 -Wall -Wextra")
add_executable(benchmark_memory_base_stack "")
target_sources(benchmark_memory_base_stack
    PRIVATE
    benchmark_memory_base_stack.cpp
)
target_link_libraries(benchmark_memory_base_stack
    PRIVATE
    solver_3
)


add_executable(benchmark_memory_stack "")
target_sources(benchmark_memory_stack
    PRIVATE
    benchmark_memory_stack.cpp
)
target_link_libraries(benchmark_memory_stack
    PRIVATE
    solver_3
)

