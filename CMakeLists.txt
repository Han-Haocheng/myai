set(PROJECT_SOURCE_DIR .)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_C_COMPILER gcc)

cmake_minimum_required(VERSION 3.0)
project(my_ai)


set(CMAKE_VERBOSE_MAKEFILE ON)

add_definitions(-rdynamic
        -O0
        -ggdb
        -std=c++11
        -Wall
        -Wno-deprecated
        -Werror
        -Wno-unused-function
        -Wno-builtin-macro-redefined)

include(cmake/utils.cmake)
link_directories(AFTER /usr/local/lib /usr/lib)

set(INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include")
message(STATUS >>>>${INCLUDE_DIRS})

link_directories(AFTER
        /usr/local/lib
        /usr/lib
)
add_subdirectory(myai)
add_subdirectory(mylib)
add_subdirectory(myai)
