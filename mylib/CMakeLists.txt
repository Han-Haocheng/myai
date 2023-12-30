find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)
include_directories(include ${Boost_INCLUDE_DIRS}/boost)
add_definitions(-DBOOST_TEST_DYN_LINK -g)

message(STATUS >>>${Boost_FILESYSTEM_LIBRARY})
message(STATUS >>>${Boost_SYSTEM_LIBRARY})
message(STATUS >>>${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

aux_source_directory(../mylib/src MYLIB_SRCS)
aux_source_directory(../mylib/test MYLIB_TEST_LIST)


add_library(mylib SHARED ${MYLIB_SRCS})
force_redefine_file_macro_for_sources(mylib)


foreach (file ${MYLIB_TEST_LIST})
    message(STATUS >>>file=${file})
    string(REGEX REPLACE ".+/(.+)\\..*" "\\1" TEST_NAME ${file})
    message(STATUS >>>TEST_NAME=${TEST_NAME})

    add_executable(${TEST_NAME} ${file})
    force_redefine_file_macro_for_sources(${TEST_NAME})
    target_link_libraries(${TEST_NAME}  mylib yaml-cpp ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach ()




