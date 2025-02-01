# Если включено тестирование, то ищем googleTest
if(ENABLE_UNIT_TESTS)
    enable_testing()
    find_package(GTest REQUIRED)

    if(GTEST_FOUND)
        message(STATUS "GoogleTest is found!")
    endif()

  # Реализация стека, которая будет выступать в качестве эталонной для тестов.
    add_library(standard_solution OBJECT standard_solution.cpp)
    target_include_directories(standard_solution PUBLIC .)
  # Создадим конфигурационный файл, в котором будет определён путь к тестовым данным и путь куда сохранять выходыне.
    cmake_path(APPEND TEST_DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}" "data")
    message(STATUS "TEST_DATA_DIR = ${TEST_DATA_DIR}")
    configure_file(config.hpp.in "${CMAKE_BINARY_DIR}/include/config.hpp")

    add_executable(user_tests "")
    target_include_directories(user_tests PUBLIC ${CMAKE_BINARY_DIR}/include)
    
    target_sources(user_tests
        PRIVATE
        user_tests.cpp
    )

    target_link_libraries(user_tests
        PRIVATE
        ${solver}
        standard_solution
        GTest::gtest_main
    )

    # set(CMAKE_GTEST_DISCOVER_TESTS_DISCOVERY_MODE PRE_TEST)
    # message(STATUS "CMAKE_CURRENT_BINARY_DIR = ${CMAKE_CURRENT_BINARY_DIR}")
    # message(STATUS "CMAKE_GTEST_DISCOVER_TESTS_DISCOVERY_MODE = ${CMAKE_GTEST_DISCOVER_TESTS_DISCOVERY_MODE}")
    gtest_discover_tests(
        user_tests
        DISCOVERY_MODE "PRE_TEST"
        EXTRA_ARGS "--gtest_color=yes"
    )
endif()