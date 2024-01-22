set(CMAKE_CXX_COMPILER "/usr/bin/g++")
set(CMAKE_C_COMPILER "/usr/bin/gcc")

cmake_minimum_required(VERSION 3.0)
project(my_ai)

message(STATUS "PROJECT_SOURCE_DIR>>>>>>${PROJECT_SOURCE_DIR}")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 17)
add_compile_options(
    -std=c++17
    -O0 
    -ggdb
    -rdynamic 
    -Wall 
    -Werror
    # -Wno-deprecated 
    -Wno-unused-function 
    -Wno-builtin-macro-redefined
)


find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)
add_definitions(-DBOOST_TEST_DYN_LINK -g)

message(STATUS >>>${Boost_INCLUDE_DIRS})
message(STATUS >>>${Boost_SYSTEM_LIBRARY})
message(STATUS >>>${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

set(INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include")
message(STATUS >>>>${INCLUDE_DIRS})

include(cmake/utils.cmake)
link_directories(AFTER "/usr/local/lib" "/usr/lib")
include_directories(AFTER "/usr/local/include" "/usr/include")
link_directories(AFTER
        /usr/local/lib
        /usr/lib
)

add_subdirectory(mylib)


add_subdirectory(myai)
add_subdirectory(mytest)

