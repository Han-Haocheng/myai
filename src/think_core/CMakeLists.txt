message(STATUS "Enter ThinkCore folder")

set(MODULE_CORE think_core)

aux_source_directory(. CORE_SOURCE)   #将当前目录下所有源文件赋给CORE_SOURCE变量，当源文件数量比较多时会比较方便

include_directories(inout)
