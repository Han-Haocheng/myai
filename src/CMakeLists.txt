<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> a3a28bf (2025年2月24日 20:19:05)

set(MODEL_NAME myai)

initialize_model(${MODEL_NAME} ".")
add_executable(${MODEL_NAME} ${MODEL_SOURCES})
install_model(${MODEL_NAME})

target_link_libraries(${MODEL_NAME} mylib dbghelp yaml-cpp::yaml-cpp)

<<<<<<< HEAD
=======
=======
>>>>>>> e7ce4b8 (v0.40)
message(STATUS "Enter src folder")
set(MODULE_SRC think_core)

aux_source_directory(. SRC_DIR)
>>>>>>> f77e2f3 (v0.38)
=======
>>>>>>> a3a28bf (2025年2月24日 20:19:05)
=======

set(MODEL_NAME myai)

initialize_module(${MODEL_NAME} exe)

target_link_libraries(${MODEL_NAME} mylib::mylib_core)
<<<<<<< HEAD
>>>>>>> f215332 (2025-1-1)
=======

>>>>>>> b5b5b2a (2025-1-8 09:18:02)
