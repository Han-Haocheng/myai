set(MYLIB_SOURCE_FILES
    "mylib_def.h"
    "config/configer.h"
    "config/config_val.h"
    "converter/converter.h" 
    "converter/ymal_converter.h"
    "main.cpp")
set(PROJECT_NAME my-lib)

message(STATUS ">>>> created file list:")
foreach(SOURCE_FILE ${MYLIB_SOURCE_FILES})
	message(STATUS "\t\t[${SOURCE_FILE}]")
endforeach()



add_library (${PROJECT_NAME} STATIC ${MYLIB_SOURCE_FILES})


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD ${MY_PROJECT_CXX_STANDARD})
endif()

message(STATUS ">>>> [${PROJECT_NAME}] create finish.\n\n")