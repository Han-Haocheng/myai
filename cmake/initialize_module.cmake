# 定义一个函数来初始化模块
function(initialize_module MODEL_NAME MODEL_TYPE)
    message(STATUS "${MODEL_NAME} initializing.....")

    file(GLOB_RECURSE MODEL_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
    file(GLOB_RECURSE MODEL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
    source_group("header files" FILES ${MODEL_HEADERS})
    source_group("source files" FILES ${MODEL_SOURCES})


    message(STATUS ">>>> Header files:")
    foreach (SOURCE_FILE ${MODEL_HEADERS})
        message(STATUS "\t\t[${SOURCE_FILE}]")
    endforeach ()

    message(STATUS ">>>> Source files:")
    foreach (SOURCE_FILE ${MODEL_SOURCES})
        message(STATUS "\t\t[${SOURCE_FILE}]")
    endforeach ()

    if (${MODEL_TYPE} STREQUAL exe)
        add_executable(${MODEL_NAME} ${MODEL_SOURCES})
    elseif (${MODEL_TYPE} STREQUAL lib)
        add_library(${MODEL_NAME} STATIC ${MODEL_SOURCES})
    elseif (${MODEL_TYPE} STREQUAL dll)
        add_library(${MODEL_NAME} SHARED ${MODEL_SOURCES})
    else ()
        message(WARNING "MODEL_TYPE:${MODEL_TYPE} is unknown.MODEL_TYPE must be one of in <lib>, <exe> , <dll>")
    endif ()

    install(FILES ${MODEL_HEADERS} DESTINATION include/${MODEL_NAME})
    set(PROJECT_MODEL_LIST ${PROJECT_MODEL_LIST} ${MODEL_NAME} CACHE INTERNAL "model list" FORCE)

    message(STATUS "[${MODEL_NAME}] initialized.")
endfunction()