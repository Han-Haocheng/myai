
set(MODEL_NAME myai)

initialize_model(${MODEL_NAME} ".")
add_executable(${MODEL_NAME} ${MODEL_SOURCES})
install_model(${MODEL_NAME})

target_link_libraries(${MODEL_NAME} mylib dbghelp yaml-cpp::yaml-cpp)

