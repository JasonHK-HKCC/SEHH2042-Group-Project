cmake_minimum_required(VERSION 3.10)

project(JetAssign VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY build)

add_executable(JetAssign src/JetAssign.cpp)
