set(PROJECT_NAME mylib)

file(GLOB_RECURSE MODEL_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*/*.h mylib.h)
file(GLOB_RECURSE MODEL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*/*.cpp main.cpp)
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

add_library(${PROJECT_NAME} STATIC ${MODEL_HEADERS} ${MODEL_SOURCES})


if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD ${MY_PROJECT_CXX_STANDARD})
endif ()

message(STATUS ">>>> [${PROJECT_NAME}] create finish.\n\n")