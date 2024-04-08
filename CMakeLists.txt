# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required(VERSION 3.27)

if (${CMAKE_VERSION} VERSION_LESS 3.21)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else ()
    cmake_policy(VERSION 3.21)
endif ()
# message(>>>>>>>>>>>>>>>>>>${CMAKE_SOURCE_DIR})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
project("myproject")

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
# message(yaml-cpp_INCLUDE_DIRS - ${yaml-cpp_INCLUDE_DIRS})
# 如果支持，请为 MSVC 编译器启用热重载。
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif ()
# message(>>>>>>>>>>>>>>>>>>>>>>${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
link_directories(${Boost_LIBRARY_DIRS} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
include_directories(${Boost_INCLUDE_DIRS} ${yaml-cpp_INCLUDE_DIRS})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

# 包含子项目

message("my_ai cmake begin")
add_subdirectory("myai")

message("my-lib cmake begin")
add_subdirectory("mylib")


message("my-test cmake begin")
add_subdirectory("mytest")

target_link_libraries(mylib PRIVATE yaml-cpp::yaml-cpp)
target_link_libraries(mytest PRIVATE yaml-cpp::yaml-cpp mylib ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
