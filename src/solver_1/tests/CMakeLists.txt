# Если включено тестирование, то ищем googleTest
if(ENABLE_UNIT_TESTS)
    enable_testing()
    find_package(GTest REQUIRED)

    if(GTEST_FOUND)
        message(STATUS "GoogleTest is found!")
    endif()

    add_executable(user_tests "")
    target_include_directories(user_tests PUBLIC ${CMAKE_BINARY_DIR}/include)
    
    target_sources(user_tests
        PRIVATE
        user_tests.cpp
    )

    target_link_libraries(user_tests
        PRIVATE
        solver_1
        standard_solution
        GTest::gtest_main
    )

    gtest_discover_tests(
        user_tests
        DISCOVERY_MODE "PRE_TEST"
        EXTRA_ARGS "--gtest_color=yes"
    )
endif()