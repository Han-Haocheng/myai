cmake_minimum_required(VERSION 3.1) #最低版本

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(MYSQL_CONNECTOR_DIR "C:/Program Files/MySQL/Connector C++ 8.0")
set(MYSQL_SERVER_DIR "C:/Program Files/MySQL/MySQL Server 8.0")
set(BOOST_DIR "D:/Users/HanHaocheng/Documents/Program Projects/lib/boost_1_79_0")

set(INCLUDE_DIR
    ${MYSQL_CONNECTOR_DIR}/include
    ${MYSQL_SERVER_DIR}/include
    ${BOOST_DIR}/include
    )
set(LINK_DIR
    ${MYSQL_CONNECTOR_DIR}/lib64
    ${MYSQL_SERVER_DIR}/lib
    ${BOOST_DIR}/stage/lib
    )

# #引入头文件

include_directories(${INCLUDE_DIR})

# #引入库文件

link_directories(${LINK_DIR})

set(HEADER_DIRS "src/header/type/type_node.h")
set(SOURCE_DIR "./src/main.cpp")

project(ThinkSystem VERSION 0.23) #项目名称及自定义版本号
add_executable(ThinkSystem ${SOURCE_DIR} ${HEADER_DIRS}) #添加生成可执行文件