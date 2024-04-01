# CMakeList.txt: my-ai 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#

set(PROJECT_NAME my-ai)

set(MYAI_SOURCE_FILES
        "my-ai.cpp"
        "my-ai.h"
        "core/myai_controller.cpp"
        "core/myai_controller.h"
        "core/myai_def.h"
        "core/myai_factory.cpp"
        "core/myai_factory.h"
        "core/myai_id_allocator.cpp"
        "core/myai_id_allocator.h"
        "core/myai_io_manager.cpp"
        "core/myai_io_manager.h"
        "core/myai_node.cpp"
        "core/myai_node.h"
        "core/myai_node_dao.cpp"
        "core/myai_node_dao.h"
        "core/myai_thinker.cpp"
        "core/myai_thinker.h"
        "io/myai_io_interface.h"
)


message(STATUS ">>>> created file list:")
foreach (SOURCE_FILE ${MYAI_SOURCE_FILES})
    message(STATUS "\t\t[${SOURCE_FILE}]")
endforeach ()



# 将源代码添加到此项目的可执行文件。
add_executable(${PROJECT_NAME} ${MYAI_SOURCE_FILES})


if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD ${MY_PROJECT_CXX_STANDARD})
endif ()

message(STATUS ">>>> [${PROJECT_NAME}] create finish.\n\n")

# TODO: 如有需要，请添加测试并安装目标。
