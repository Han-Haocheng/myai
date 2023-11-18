cmake_minimum_required(VERSION 3.0)
project(my_ai)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O0 -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function")


aux_source_directory("src/log" LOG_LIB)
aux_source_directory("src/config" CONFIG_LIB)
aux_source_directory("src/time" TIME_LIB)


#add_library(my_ai SHARED ${LIB_SRC})
#add_library(ai_static STATIC ${LIB_SRC})
#set_target_properties(ai_static PROPERTIES OUTPUT_NAME "myserver")


link_directories(AFTER
        /usr/local/lib
        /usr/lib
)

add_library(myaibase SHARED ${LOG_LIB} ${CONFIG_LIB} ${TIME_LIB} )

add_executable(log_test "tests/test_log.cpp")
#add_dependencies(test my_ai)
target_link_libraries(log_test myaibase yaml-cpp)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
