# Добавить выбор различных решателей

if(ENABLE_UNIT_TESTS)
    # Реализация стека, которая будет выступать в качестве эталонной для тестов.
    add_subdirectory(standard_solution)
    # Создадим конфигурационный файл, в котором будет определён путь к тестовым данным и путь куда сохранять выходыне.
    cmake_path(APPEND TEST_DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}" "tests_data")
    message(STATUS "TEST_DATA_DIR = ${TEST_DATA_DIR}")
    configure_file(config.hpp.in "${CMAKE_BINARY_DIR}/include/config.hpp")
endif ()

if (${solver} STREQUAL "solver_1")
    add_subdirectory(solver_1)
elseif (${solver} STREQUAL "solver_2")
    add_subdirectory(default_stack_list)
    add_subdirectory(solver_2)
elseif (${solver} STREQUAL "solver_3")
    add_subdirectory(solver_3)
else ()
    message(FATAL_ERROR "Error. Incorrect 'solver' value.")
endif ()