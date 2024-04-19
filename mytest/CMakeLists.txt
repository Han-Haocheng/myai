set(MODEL_NAME mytest)

file(GLOB_RECURSE MODEL_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB_RECURSE MODEL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
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

add_executable(${MODEL_NAME} ${MODEL_SOURCES})

target_link_libraries(${MODEL_NAME} PRIVATE ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} yaml-cpp mylib)
force_redefine_file_macro_for_sources(${MODEL_NAME})

enable_testing()

add_test(
        NAME MyTest
        COMMAND ${PROJECT_NAME} --run_test=test_log_base
)


message(STATUS ">>>> [${MODEL_NAME}] create finish.\n\n")
