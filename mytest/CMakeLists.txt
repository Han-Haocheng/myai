
set(MODEL_NAME mytest)
set(MODEL_LINK_LIB boost_unit_test_framework)

file(GLOB_RECURSE MODEL_HEADERS  ${CMAKE_CURRENT_SOURCE_DIR}/*.h )
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/. MODEL_SOURCES)
source_group("header files" FILES ${MODEL_HEADERS}) 
source_group("source files" FILES ${MODEL_SOURCES})
add_executable(${MODEL_NAME} ${MODEL_HEADERS} ${MODEL_SOURCES})
force_redefine_file_macro_for_sources(${MODEL_NAME})

target_link_libraries(${MODEL_NAME} PRIVATE ${MODEL_LINK_LIB})
enable_testing()

add_test(
	NAME mytest
	COMMAND ${MODEL_NAME}
)



