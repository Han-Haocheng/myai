cmake_minimum_required(VERSION 3.1) #最低版本
project(ThinkSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")

set(ALL_LIB_DIR D:/Users/HanHaocheng/Documents/lib)
set(OpenCV_DIR ${ALL_LIB_DIR}/opencv3.4.16/build/x64/vc15/lib)

# 引入 OpenCV 库
find_package(OpenCV 3.4.16 REQUIRED)

# 添加 OpenCV 库头文件搜索路径
include_directories(${OpenCV_INCLUDE_DIRS})

file(GLOB ALL_SRCS src/main.cpp)

add_executable(${PROJECT_NAME} ${ALL_SRCS})

# 判断是否找到 OpenCV 库
if (OpenCV_FOUND)
  target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
  message(STATUS "查找到opencv")

  message(STATUS "opencv头文件路径: " ${OpenCV_INCLUDE_DIRS})
  message(STATUS "\t opencv库: " ${OpenCV_LIBS})
  message(STATUS "\t opencv版本: " ${OpenCV_VERSION})
endif ()



