cmake_minimum_required(VERSION 3.16)
project(libcdf C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_C_FLAGS_RELEASE "-O2")
set(CMAKE_C_FLAGS_DEBUG "-O0 -g")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

include(cmake/CompilerWarnings.cmake)

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)
