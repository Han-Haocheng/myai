# CMakeList.txt: myai 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#

set(PROJECT_NAME myai)

file(GLOB_RECURSE MODEL_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*/*.h")
file(GLOB_RECURSE MODEL_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*/*.cpp")
source_group("header files" FILES ${MODEL_HEADERS})
source_group("source files" FILES ${MODEL_SOURCES})

message(STATUS ">>>> Header files:")
foreach(SOURCE_FILE ${MODEL_HEADERS})
    message(STATUS "\t\t[${SOURCE_FILE}]")
endforeach()

message(STATUS ">>>> Source files:")
foreach(SOURCE_FILE ${MODEL_SOURCES})
    message(STATUS "\t\t[${SOURCE_FILE}]")
endforeach()

# 将源代码添加到此项目的可执行文件。
add_executable(${PROJECT_NAME} ${MODEL_HEADERS} ${MODEL_SOURCES})

if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD ${MY_PROJECT_CXX_STANDARD})
endif ()

message(STATUS ">>>> [${PROJECT_NAME}] create finish.\n\n")

# TODO: 如有需要，请添加测试并安装目标。
