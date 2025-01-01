cmake_minimum_required(VERSION 3.22)

#file(TO_CMAKE_PATH "$ENV{VCPKG_ROOT}" VCPKG_ROOT)
set(CMAKE_TOOLCHAIN_FILE "F:/lib_vcpkg/scripts/buildsystems/vcpkg.cmake")
message(STATUS "CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}")

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

set(mylib_ROOT "F:\\c++\\mylib\\out")
find_package(mylib REQUIRED)
message(STATUS ">>mylib_INCLUDE_DIRS：${mylib_INCLUDE_DIRS}")
message(STATUS ">>mylib_LIBRARYS：${mylib_LIBRARIES}")

add_subdirectory(src)


