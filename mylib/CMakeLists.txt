set(MODEL_NAME mylib)
set(MODEL_LINK_LIB yaml-cpp boost_filesystem )
message(STATUS ">>>>>model '${MODEL_NAME}' link lib '${MODEL_LINK_LIB}'<<<<")

file(GLOB_RECURSE MODEL_HEADERS  ${CMAKE_CURRENT_SOURCE_DIR}/*/*.h )
file(GLOB_RECURSE MODEL_SOURCES  ${CMAKE_CURRENT_SOURCE_DIR}/*/*.cpp )

source_group("header files" FILES ${MODEL_HEADERS}) 
source_group("source files" FILES ${MODEL_SOURCES})
add_library(${MODEL_NAME} STATIC ${MODEL_HEADERS} ${MODEL_SOURCES})
force_redefine_file_macro_for_sources(${MODEL_NAME})
target_link_libraries(${MODEL_NAME} PRIVATE ${MODEL_LINK_LIB})
