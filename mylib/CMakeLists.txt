
find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)
include_directories(include ${Boost_INCLUDE_DIRS}/boost)
add_definitions(-DBOOST_TEST_DYN_LINK -g)

message(STATUS >>>${Boost_FILESYSTEM_LIBRARY})
message(STATUS >>>${Boost_SYSTEM_LIBRARY})
message(STATUS >>>${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

set(MODEL_NAME mylib)
set(MODEL_LINK_LIB yaml-cpp boost_filesystem boost_unit_test_framework)

file(GLOB_RECURSE MODEL_HEADERS  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h )
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/. MODEL_SOURCES)
source_group("header files" FILES ${MODEL_HEADERS}) 
source_group("source files" FILES ${MODEL_SOURCES})
add_library(${MODEL_NAME} STATIC ${MODEL_HEADERS} ${MODEL_SOURCES})
force_redefine_file_macro_for_sources(${MODEL_NAME})
target_link_libraries(${MODEL_NAME} PRIVATE ${MODEL_LINK_LIB})
