
set(MODEL_NAME myai)

initialize_model(${MODEL_NAME} ".")
add_executable(${MODEL_NAME} ${MODEL_SOURCES} ${MODEL_HEADERS})
install_model(${MODEL_NAME})

target_link_libraries(${MODEL_NAME} Mylib Dbghelp yaml-cpp::yaml-cpp Qt6::Core Qt6::Gui Qt6::Quick Qt6::Widgets)

# qt6_add_resources(${MODEL_NAME} "qml"
#         PREFIX
#         "/"
#         FILES
#         ${QML_FILES}
# )
