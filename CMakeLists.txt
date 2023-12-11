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

aux_source_directory("src/log" LOG_LIB)
aux_source_directory("src/config" CONFIG_LIB)
aux_source_directory("src/time" TIME_LIB)
aux_source_directory("src/thread" THREAD_LIB)

add_library(mylib SHARED ${LOG_LIB} ${THREAD_LIB} ${CONFIG_LIB}  ${TIME_LIB})

#add_library(my_ai SHARED ${LIB_SRC})
#add_library(ai_static STATIC ${LIB_SRC})
#set_target_properties(ai_static PROPERTIES OUTPUT_NAME "myserver")


link_directories(AFTER
                 /usr/local/lib
                 /usr/lib
)


set(LINK_LIBS mylib yaml-cpp)

aux_source_directory("tests" TESTS)

foreach (test ${TESTS})
  string(REPLACE ".cpp" "" test_name ${test})
  string(REPLACE "tests/" "" test_name ${test_name})
  #message("==>${test_name} is begin created to executable.")
  add_executable(${test_name} ${test})

  force_redefine_file_macro_for_sources(${test_name})
  target_link_libraries(${test_name} ${LINK_LIBS})
  message("==>create <${test_name}> finish")
endforeach ()

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
