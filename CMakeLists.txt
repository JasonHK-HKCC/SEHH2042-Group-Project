cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(SEHH2042-Group-Project LANGUAGES CXX VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY build/${CMAKE_BUILD_TYPE})

add_subdirectory(src)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_subdirectory(test)
endif()
# add_subdirectory(extern/googletest/googletest)
