cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(Stack_max LANGUAGES CXX)

message(STATUS "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
# require c++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(THREADS_PREFER_PTHREAD_FLAG ON)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g3 -Wall -Wextra")

  set(SANITIZERS_COMMON
    -fsanitize=address
    -fsanitize=undefined
    -fsanitize=leak
  )
  list(JOIN SANITIZERS_COMMON " " SANITIZERS_COMMON)

  set(SANITIZERS_CLANG
    -fsanitize=memory
    -fsanitize=type
  )
  list(JOIN SANITIZERS_CLANG " " SANITIZERS_CLANG)

  if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SANITIZERS_COMMON}")
  elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SANITIZERS_COMMON} ${SANITIZERS_CLANG}")
  endif()
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -Wextra")
elseif (CMAKE_BUILD_TYPE STREQUAL "Valgrind")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -O0 -g3 -Wall -Wextra")
endif()
message(STATUS "CMAKE_CXX_FLAGS = '${CMAKE_CXX_FLAGS}'")

set(solver solver_2 CACHE STRING "")
message(STATUS "solver = ${solver}")

option(ENABLE_UNIT_TESTS "Enable unit tests" ON)
message(STATUS "Enable testing: ${ENABLE_UNIT_TESTS}")


add_subdirectory(src)
