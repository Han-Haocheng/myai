include_directories(AFTER ${INCLUDE_DIRS})
set(MODEL_NAME mytest)
set(MODEL_LINK_LIB mylib boost_unit_test_framework)
message(STATUS ">>>>>model '${MODEL_NAME}' link lib '${MODEL_LINK_LIB}'<<<<")

file(GLOB_RECURSE MODEL_HEADERS  ${CMAKE_CURRENT_SOURCE_DIR}/*.h )
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/. MODEL_SOURCES)
source_group("header files" FILES ${MODEL_HEADERS}) 
source_group("source files" FILES ${MODEL_SOURCES})



foreach(TEST_FILE ${MODEL_SOURCES})
	
	string(REPLACE ".cpp" "" CHILD_MODEL_NAME ${TEST_FILE})
	string(REPLACE "/home/hanhaocheng/myproject/my-ai/mytest/./" "" CHILD_MODEL_NAME ${CHILD_MODEL_NAME})
	# message(${CHILD_MODEL_NAME})

	add_executable(${CHILD_MODEL_NAME} ${TEST_FILE})
	force_redefine_file_macro_for_sources(${CHILD_MODEL_NAME})
	target_link_libraries(${CHILD_MODEL_NAME} PRIVATE ${MODEL_LINK_LIB})

endforeach()

enable_testing()

add_test(
	NAME MyTest
	COMMAND ${MODEL_NAME}
)



