<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
cmake_minimum_required(VERSION 3.22)
#file(TO_CMAKE_PATH "$ENV{VCPKG_ROOT}" VCPKG_ROOT)
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}\\scripts\\buildsystems\\vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
message(STATUS ">>>> CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}")

<<<<<<< HEAD
include(FindPackageHandleStandardArgs)

set(CMAKE_BUILD_TYPE Debug)

set(myai_VERSION 0.2.0.20241126)
project(myai VERSION ${myai_VERSION})

set(mylib_ROOT "F:\\c++\\mylib\\out\\${CMAKE_BUILD_TYPE}")

find_package(mylib REQUIRED)
=======
cmake_minimum_required(VERSION 3.1) #最低版本
project(ThinkSystem)
>>>>>>> 69004fd (v0.33)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")

set(ALL_LIB_DIR D:/Users/HanHaocheng/Documents/lib)
set(OpenCV_DIR ${ALL_LIB_DIR}/opencv3.4.16/build/x64/vc15/lib)

# 引入 OpenCV 库
find_package(OpenCV 3.4.16 REQUIRED)

# 添加 OpenCV 库头文件搜索路径
include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "build")

add_subdirectory(src)
file(GLOB ALL_SRCS src/main.cpp ${CORE_SOURCE})


add_executable(${PROJECT_NAME} ${ALL_SRCS})

# 判断是否找到 OpenCV 库
if (OpenCV_FOUND)
  target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

  message(STATUS "查找到opencv")
  message(STATUS "opencv头文件路径: " ${OpenCV_INCLUDE_DIRS})
  message(STATUS "\t opencv库: " ${OpenCV_LIBS})
  message(STATUS "\t opencv版本: " ${OpenCV_VERSION})
endif ()

=======
cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 17)
project(think)
<<<<<<< HEAD
>>>>>>> 55d935f (v0.40)
=======
cmake_minimum_required(VERSION 3.25)
project(think)
>>>>>>> e7ce4b8 (v0.40)

=======
>>>>>>> 737c593 (v1.0)
add_executable(think src/main.cpp)
=======
cmake_minimum_required(VERSION 3.8)
project(THINK)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED NO)
aux_source_directory("./src" SOURCES_DIRECTORY)
=======
set(CMAKE_VERBOSE_MAKEFILE ON)
=======
=======
#
#set(PROJECT_SOURCE_DIR ./)
#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
>>>>>>> 7809b15 (v0.8.0 完成基本的io)
#cmake_minimum_required(VERSION 3.0)
#project(my_ai)
#
#set(CMAKE_VERBOSE_MAKEFILE ON)
#
<<<<<<< HEAD
#
=======
>>>>>>> 7809b15 (v0.8.0 完成基本的io)
#add_definitions(-rdynamic
#                -O0
#                -ggdb
#                -std=c++11
#                -Wall
#                -Wno-deprecated
#                -Werror
#                -Wno-unused-function
#                -Wno-builtin-macro-redefined)
#include(cmake/utils.cmake)
#
<<<<<<< HEAD
#aux_source_directory("src/log" LOG_LIB)
#aux_source_directory("src/config" CONFIG_LIB)
#aux_source_directory("src/time" TIME_LIB)
#aux_source_directory("src/asyn" THREAD_LIB)
#
#add_library(myai SHARED ${LOG_LIB} ${THREAD_LIB} ${CONFIG_LIB} ${TIME_LIB})
#force_redefine_file_macro_for_sources(myai)
=======
#include_directories(mylib/include)
#aux_source_directory(mylib/src MY_LIB_SRC)
#
#add_library(mylib SHARED ${MY_LIB_SRC})
#
#force_redefine_file_macro_for_sources(mylib)
#
>>>>>>> 7809b15 (v0.8.0 完成基本的io)
##add_library(my_ai SHARED ${LIB_SRC})
##add_library(ai_static STATIC ${LIB_SRC})
##set_target_properties(ai_static PROPERTIES OUTPUT_NAME "myserver")
#
#
#link_directories(AFTER
#                 /usr/local/lib
#                 /usr/lib
#)
#
#
<<<<<<< HEAD
#set(LINK_LIBS myai yaml-cpp)
#aux_source_directory("tests" TESTS)
#
#foreach (test ${TESTS})
#  string(REPLACE ".cpp" "" test_name ${test})
#  string(REPLACE "tests/" "" test_name ${test_name})
#
#  message("==>name = ${test_name} | file = ${test}.")
#
#  add_executable(${test_name} ${test})
#
#  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
#    target_compile_definitions(${test_name} PRIVATE _DEBUG)
#  endif ()
#
#  force_redefine_file_macro_for_sources(${test_name})
#  target_link_libraries(${test_name} ${LINK_LIBS})
#endforeach ()
#
#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
=======
=======
#set(LINK_LIBS mylib yaml-cpp)
#
#add_subdirectory(tests)
#enable_testing()
#add_test(NAME my_test COMMAND Test)
>>>>>>> 7809b15 (v0.8.0 完成基本的io)

set(PROJECT_SOURCE_DIR ./)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
>>>>>>> 24084a7 (v0.7.0)
=======
set(PROJECT_SOURCE_DIR .)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_C_COMPILER gcc)
=======
set(CMAKE_CXX_COMPILER "/usr/bin/g++")
set(CMAKE_C_COMPILER "/usr/bin/gcc")
>>>>>>> dd5c0c4 (添加项目文件。)
=======
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_C_COMPILER gcc)
>>>>>>> 688b3c0 (v1.0.3)

>>>>>>> b392109 (添加项目文件。)
cmake_minimum_required(VERSION 3.0)
project(my_ai)
=======
﻿# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
<<<<<<< HEAD
<<<<<<< HEAD
cmake_minimum_required(VERSION 3.8)

set(MY_PROJECT_CXX_STANDARD 14)
>>>>>>> 371cce7 (x86-v0.0.1)
=======
cmake_minimum_required(VERSION 3.27)
>>>>>>> cf98b81 (v0.02 - x86)
=======
cmake_minimum_required(VERSION 3.22)
>>>>>>> 4c14146 (v1.0.0 系统主体结构完整，测试通过)
=======
cmake_minimum_required(VERSION 3.22)
#file(TO_CMAKE_PATH "$ENV{VCPKG_ROOT}" VCPKG_ROOT)
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}\\scripts\\buildsystems\\vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
message(STATUS ">>>> CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}")

include(FindPackageHandleStandardArgs)

set(CMAKE_BUILD_TYPE Debug)

set(myai_VERSION 0.2.0.20241126)
project(myai VERSION ${myai_VERSION})

set(mylib_ROOT "F:\\c++\\mylib\\out\\${CMAKE_BUILD_TYPE}")

find_package(mylib REQUIRED)
>>>>>>> a3a28bf (2025年2月24日 20:19:05)

<<<<<<< HEAD
=======
if (${CMAKE_VERSION} VERSION_LESS 3.21)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else ()
    cmake_policy(VERSION 3.21)
endif ()

# message(>>>>>>>>>>>>>>>>>>${CMAKE_SOURCE_DIR})
>>>>>>> 41248b3 (v1.0.1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)

# 如果是MSVC编译器，则添加编译器标志以禁用特定警告
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4819")
endif()

check_packages()
check_environment()

message(STATUS ">>mylib_INCLUDE_DIRS：${mylib_INCLUDE_DIRS}")
message(STATUS ">>mylib_LIBRARYS：${mylib_LIBRARIES}")

set(CMAKE_CXX_STANDARD 17)

include("./cmake/utils.cmake")

<<<<<<< HEAD
# link_directories(${mylib_LIBRARIES})
# include_directories(${mylib_INCLUDE_DIRS})
# add_definitions(-DYAML_CPP_BUILD_SHARED_LIBS)
add_subdirectory(src)
=======
IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
    MESSAGE(STATUS "current platform: Linux ")
    set(yaml-cpp_ROOT "/home/hanhaocheng/libraries/yaml-cpp-master")
    find_package(yaml-cpp 0.8.0 REQUIRED)
    if (yaml-cpp_FOUND)
        message(">>>> yaml-cpp is found")
        message("\t\t YamlCpp version: ${yaml-cpp_VERSION}")
        message("\t\t yaml-cpp include path is : ${YAML_CPP_INCLUDE_DIR}")
        message("\t\t yaml-cpp library path is : ${YAML_CPP_LIBRARY_DIR}")
        message("\t\t yaml-cpp libraries is :")
        foreach (Yaml_Cpp_lib ${YAML_CPP_LIBRARIES})
            message("\t\t\t\t ${Yaml_Cpp_lib}")
        endforeach ()
        # message("\t\t\t\t Boost unit_test_framework is : ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}")
    else ()
        message(WARNING "yaml-cpp not found.")
    endif ()

    set(Boost_ROOT "/home/hanhaocheng/libraries/boost_1_83_0")
    find_package(Boost 1.83.0 REQUIRED COMPONENTS unit_test_framework)
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
    MESSAGE(STATUS "current platform: Windows")

    if (MSVC)
        # warning level 4 and all warnings as errors
        add_compile_options(/W4)
        set(Boost_COMPILER -vc143)
        set(Boost_ROOT "H:/Documents/Program Projects/lib/boost_1_84_0/lib-msvc-14.3")
        set(Boost_USE_STATIC_LIBS ON)  # only find static libs
        set(Boost_USE_DEBUG_LIBS ON)  # ignore debug libs and
        set(Boost_USE_RELEASE_LIBS OFF)  # only find release libs
        set(Boost_USE_MULTITHREADED ON)
        set(Boost_USE_STATIC_RUNTIME OFF)
    else ()
        # lots of warnings and all warnings as errors
        add_compile_options(-Wall -Wextra -pedantic)
        set(Boost_COMPILER -mgw8)
        set(Boost_ROOT "H:/Documents/Program Projects/lib/boost_1_84_0/lib-gcc")
        set(Boost_USE_STATIC_LIBS ON)  # only find static libs
        set(Boost_USE_DEBUG_LIBS ON)  # ignore debug libs and
        set(Boost_USE_RELEASE_LIBS OFF)  # only find release libs
        set(Boost_USE_MULTITHREADED ON)
        set(Boost_USE_STATIC_RUNTIME ON)
    endif ()
    find_package(Boost 1.84.0 REQUIRED COMPONENTS unit_test_framework)

    include(FetchContent)
    FetchContent_Declare(
            yaml-cpp
            GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
            GIT_TAG master # Can be a tag (yaml-cpp-x.x.x), a commit hash, or a branch name (master)
    )
    FetchContent_GetProperties(yaml-cpp)
    if (NOT yaml-cpp_POPULATED)
        message(STATUS "Fetching yaml-cpp...")
        FetchContent_Populate(yaml-cpp)
        add_subdirectory(${yaml-cpp_SOURCE_DIR} ${yaml-cpp_BINARY_DIR})
    endif ()
    set(yaml-cpp_INCLUDE_DIRS "${yaml-cpp_SOURCE_DIR}/include")
    # message(>>>>>>>>>>>>>>>>>>>>>>${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
    MESSAGE(STATUS "current platform: FreeBSD")
ELSE ()
    MESSAGE(STATUS "other platform: ${CMAKE_SYSTEM_NAME}")
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")

if (Boost_FOUND)
    message(">>>> Boost_FOUND is found")
    message("\t\t YamlCpp version: ${Boost_VERSION}")
    message("\t\t Boost include path is : ${Boost_INCLUDE_DIRS}")
    message("\t\t Boost library path is : ${Boost_LIBRARY_DIRS}")
    message("\t\t Boost libraries is :")
    foreach (Boost_lib ${Boost_LIBRARIES})
        message("\t\t\t\t ${Boost_lib}")
    endforeach ()
    # message("\t\t\t\t Boost unit_test_framework is : ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}")
else ()
    message(WARNING "boost not found.")
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_SOURCE_DIR}/lib)

link_directories(${Boost_LIBRARY_DIRS} ${YAML_CPP_LIBRARY_DIR} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
include_directories(${Boost_INCLUDE_DIRS} ${YAML_CPP_INCLUDE_DIR})

# message(yaml-cpp_INCLUDE_DIRS - ${yaml-cpp_INCLUDE_DIRS})
# 如果支持，请为 MSVC 编译器启用热重载。
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif ()
>>>>>>> 41248b3 (v1.0.1)


<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
set(INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include")
message(STATUS >>>>INCLUDE_DIRS : ${INCLUDE_DIRS})

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
include_directories(include)
<<<<<<< HEAD
aux_source_directory(include/src MYLIB_SRC)
add_library(mylib STATIC ${MYLIB_SRC})
>>>>>>> da2123e (v0.5.1)
=======
aux_source_directory(include/src MY_LIB_SRC)
>>>>>>> 24084a7 (v0.7.0)

add_library(mylib SHARED ${MY_LIB_SRC})

force_redefine_file_macro_for_sources(mylib)

#add_library(my_ai SHARED ${LIB_SRC})
#add_library(ai_static STATIC ${LIB_SRC})
#set_target_properties(ai_static PROPERTIES OUTPUT_NAME "myserver")


=======
>>>>>>> 7809b15 (v0.8.0 完成基本的io)
=======
include(cmake/utils.cmake)
link_directories(AFTER "/usr/local/lib" "/usr/lib")
include_directories(AFTER "/usr/local/include" "/usr/include")
>>>>>>> dd5c0c4 (添加项目文件。)
=======
>>>>>>> 688b3c0 (v1.0.3)
link_directories(AFTER
        /usr/local/lib
        /usr/lib
)
<<<<<<< HEAD
<<<<<<< HEAD


set(LINK_LIBS mylib yaml-cpp)

<<<<<<< HEAD
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
>>>>>>> 23aebf2 (v0.5.0)

add_library(mylib SHARED ${LOG_LIB} ${THREAD_LIB} ${CONFIG_LIB}  ${TIME_LIB})

if(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))

elseif(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Release"))

<<<<<<< HEAD
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_executable(THINK ${SOURCES_DIRECTORY})
>>>>>>> aab93c0 (v1.3)
=======
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
>>>>>>> 23aebf2 (v0.5.0)
=======
add_subdirectory(tests)
enable_testing()
<<<<<<< HEAD
add_test(NAME mytest COMMAND Test)
>>>>>>> da2123e (v0.5.1)
=======
add_test(NAME my_test COMMAND Test)
>>>>>>> 24084a7 (v0.7.0)
=======
add_subdirectory(myai)
add_subdirectory(mylib)
<<<<<<< HEAD
>>>>>>> 7809b15 (v0.8.0 完成基本的io)
=======
add_subdirectory(myai)
>>>>>>> b392109 (添加项目文件。)
=======
=======
>>>>>>> f4aa444 (v1.0.0 系统主体结构完整，测试通过)

add_subdirectory(mylib)
add_subdirectory(myai)
add_subdirectory(mytest)

>>>>>>> dd5c0c4 (添加项目文件。)
=======
link_directories(${Boost_LIBRARY_DIRS})
include_directories(${Boost_INCLUDE_DIRS})

# 包含子项目。
add_subdirectory("my-ai")
add_subdirectory("my-lib")
>>>>>>> 371cce7 (x86-v0.0.1)
=======
# 包含子项目

message("my_ai cmake begin")
add_subdirectory("myai")

message("my-lib cmake begin")
add_subdirectory("mylib")

message("my-test cmake begin")
add_subdirectory("mytest")

<<<<<<< HEAD
target_link_libraries(mylib PRIVATE yaml-cpp::yaml-cpp)
target_link_libraries(mytest PRIVATE yaml-cpp::yaml-cpp mylib ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
>>>>>>> cf98b81 (v0.02 - x86)
=======
>>>>>>> 4c14146 (v1.0.0 系统主体结构完整，测试通过)
=======
>>>>>>> a3a28bf (2025年2月24日 20:19:05)
=======
cmake_minimum_required(VERSION 3.28)
project(myai)
=======
cmake_minimum_required(VERSION 3.22)
>>>>>>> f215332 (2025-1-1)

#file(TO_CMAKE_PATH "$ENV{VCPKG_ROOT}" VCPKG_ROOT)
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}\\scripts\\buildsystems\\vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
message(STATUS ">>>> CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}")

set(CMAKE_BUILD_TYPE Debug)

set(myai_VERSION 0.2.0.20241126)
project(myai VERSION ${myai_VERSION})

include(cmake/initialize_module.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)

# 如果是MSVC编译器，则添加编译器标志以禁用特定警告
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4819")
endif()

find_package(Boost CONFIG COMPONENTS lexical_cast)
find_package(yaml-cpp CONFIG REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)
include_directories(${YAML_CPP_INCLUDE_DIR})

set(mylib_ROOT "F:\\c++\\mylib\\out\\${CMAKE_BUILD_TYPE}")
find_package(mylib REQUIRED)
message(STATUS ">>mylib_INCLUDE_DIRS：${mylib_INCLUDE_DIRS}")
message(STATUS ">>mylib_LIBRARYS：${mylib_LIBRARIES}")

add_subdirectory(src)


>>>>>>> 3a9d2f1 (v0.0.1_240926)
