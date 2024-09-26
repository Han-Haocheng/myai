
aux_source_directory("." MYAI_SOURCES)

file(GLOB_RECURSE MYAI_SOURCES "*.cpp")
file(GLOB_RECURSE MYAI_HEADERS "*.h")


add_executable(myai ${MYAI_SOURCES})

target_link_libraries(myai mylib)
