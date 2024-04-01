# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required(VERSION 3.8)

set(MY_PROJECT_CXX_STANDARD 14)

if (${CMAKE_VERSION} VERSION_LESS 3.21)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else ()
    cmake_policy(VERSION 3.21)
endif ()


set(Boost_ROOT "H:\\Documents\\Program Projects\\lib\\boost_1_84_0")
find_package(Boost 1.84.0 REQUIRED)

if (Boost_FOUND)
    message(Boost\ version: ${Boost_VERSION_STRING})
    message(Boost\ include\ dir: ${Boost_INCLUDE_DIRS})
    message(Boost\ library\ dir: ${Boost_LIBRARY_DIRS})
    message("Found Boost Libraries:${Boost_LIBRARIES}")
    foreach(boost_lib IN LISTS Boost_LIBRARIES)
        message(${boost_lib})
        string(REGEX MATCH ".+/.*boost_([^-]+)-.+\.(lib|a)" boost_lib_name ${boost_lib})
        set(boost_lib_name ${CMAKE_MATCH_1})
        set(boost_target Boost::${boost_lib_name})
        if(TARGET ${boost_target})
            message(STATUS "Boost target found: " ${boost_target})
        endif(TARGET ${boost_target})
    endforeach(boost_lib)

else ()
    message(FATAL_ERROR "Boot not found")
endif ()


# 如果支持，请为 MSVC 编译器启用热重载。
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif ()

project("my-project")

link_directories(${Boost_LIBRARY_DIRS})
include_directories(${Boost_INCLUDE_DIRS})

# 包含子项目。
add_subdirectory("my-ai")
add_subdirectory("my-lib")
