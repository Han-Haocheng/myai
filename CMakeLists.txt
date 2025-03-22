cmake_minimum_required(VERSION 3.22)
file(TO_CMAKE_PATH "$ENV{VCPKG_ROOT}" VCPKG_ROOT)
set(VCPKG_ROOT "F:/lib_vcpkg")

set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}\\scripts\\buildsystems\\vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
set(VCPKG_TARGET_TRIPLET "x64-mingw-static" CACHE STRING "")
#set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "")

message(STATUS ">>>> CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}")

include(FindPackageHandleStandardArgs)

set(CMAKE_BUILD_TYPE Debug)

set(myai_VERSION 1.0.0.2025303)
project(myai VERSION ${myai_VERSION})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Set the path to your Qt installation
set(QT_DIR "F:/Qt/6.5.3/mingw_64/lib/cmake/Qt6")  # Adjust this path to your Qt installation
set(CMAKE_PREFIX_PATH ${QT_DIR} ${CMAKE_PREFIX_PATH})

set(Mylib_ROOT "F:\\c++\\mylib\\out\\${CMAKE_BUILD_TYPE}")
find_package(Mylib REQUIRED)
find_package(Qt6 COMPONENTS Core Gui Quick Widgets REQUIRED)

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
# file(GLOB QML_FILES qml/*.qml)
add_subdirectory(src)


