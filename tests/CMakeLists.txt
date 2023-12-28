find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)

include_directories(include ${Boost_INCLUDE_DIRS}/boost)

add_definitions(-DBOOST_TEST_DYN_LINK -g)

add_executable(Test test_boost_test.cpp test_config.cpp)

target_link_libraries(Test mylib yaml-cpp ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

message(STATUS >>>${Boost_FILESYSTEM_LIBRARY})
message(STATUS >>>${Boost_SYSTEM_LIBRARY})
message(STATUS >>>${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})