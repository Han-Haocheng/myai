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


# link_directories(${mylib_LIBRARIES})
# include_directories(${mylib_INCLUDE_DIRS})
# add_definitions(-DYAML_CPP_BUILD_SHARED_LIBS)
add_subdirectory(src)


