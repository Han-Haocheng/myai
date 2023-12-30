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

add_executable(myai_test ${MYAI_TEST_LIST})
force_redefine_file_macro_for_sources(myai_test)
target_link_libraries(myai_test myai mylib yaml-cpp ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

enable_testing()
add_test(NAME MyAiTest COMMAND myai_test)