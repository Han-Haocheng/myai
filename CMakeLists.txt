
set(PROJECT_SOURCE_DIR ./)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
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

include_directories(include)
aux_source_directory(include/src MY_LIB_SRC)

add_library(mylib SHARED ${MY_LIB_SRC})

force_redefine_file_macro_for_sources(mylib)

#add_library(my_ai SHARED ${LIB_SRC})
#add_library(ai_static STATIC ${LIB_SRC})
#set_target_properties(ai_static PROPERTIES OUTPUT_NAME "myserver")


link_directories(AFTER
                 /usr/local/lib
                 /usr/lib
)


set(LINK_LIBS mylib yaml-cpp)

add_subdirectory(tests)
enable_testing()
add_test(NAME my_test COMMAND Test)