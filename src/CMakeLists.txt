
set(MODEL_NAME myai)

initialize_module(${MODEL_NAME} exe)

target_link_libraries(${MODEL_NAME} mylib::mylib_core)

