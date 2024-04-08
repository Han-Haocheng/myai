set(PROJECT_NAME mytest)

file(GLOB_RECURSE MODEL_HEADERS  ${CMAKE_CURRENT_SOURCE_DIR}/*/*.h)
file(GLOB_RECURSE MODEL_SOURCES  ${CMAKE_CURRENT_SOURCE_DIR}/*/*.cpp)
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

add_executable(${PROJECT_NAME} ${MODEL_SOURCES} ${MODEL_HEADERS})


enable_testing()

add_test(
	NAME MyTest
	COMMAND ${PROJECT_NAME}
)



