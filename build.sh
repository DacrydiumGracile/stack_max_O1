#!/bin/bash

cmake -S . -B ./build -DCMAKE_BUILD_TYPE=Valgrind -Dsolver=solver_3
cmake --build ./build 