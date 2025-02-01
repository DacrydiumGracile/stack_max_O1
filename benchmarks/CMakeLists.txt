find_package(benchmark REQUIRED)

# Random push
add_executable(benchmark_1 "")
target_sources(benchmark_1
    PRIVATE
    benchmark_1.cpp
)
target_link_libraries(benchmark_1
    PRIVATE
    benchmark::benchmark
    ${solver}
    standard_solution
)
# Push последовательных чисел
add_executable(benchmark_2 "")
target_sources(benchmark_2
    PRIVATE
    benchmark_2.cpp
)
target_link_libraries(benchmark_2
    PRIVATE
    benchmark::benchmark
    ${solver}
    standard_solution
)
# Pop back
add_executable(benchmark_3 "")
target_sources(benchmark_3
    PRIVATE
    benchmark_3.cpp
)
target_link_libraries(benchmark_3
    PRIVATE
    benchmark::benchmark
    ${solver}
    standard_solution
)