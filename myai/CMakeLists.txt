find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)
include_directories(include ${Boost_INCLUDE_DIRS}/boost)
add_definitions(-DBOOST_TEST_DYN_LINK -g)

message(STATUS >>>${Boost_FILESYSTEM_LIBRARY})
message(STATUS >>>${Boost_SYSTEM_LIBRARY})
message(STATUS >>>${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

aux_source_directory(../myai/src MYAI_SRCS)
aux_source_directory(../myai/test MYAI_TEST_LIST)


add_library(myai SHARED ${MYAI_SRCS})
force_redefine_file_macro_for_sources(myai)

foreach (file ${MYAI_TEST_LIST})
    message(STATUS >>>file=${file})
    string(REGEX REPLACE ".+/(.+)\\..*" "\\1" TEST_NAME ${file})
    message(STATUS >>>TEST_NAME=${TEST_NAME})

    add_executable(${TEST_NAME} ${file})
    force_redefine_file_macro_for_sources(${TEST_NAME})
    target_link_libraries(${TEST_NAME} myai mylib yaml-cpp ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach ()