//
// Created by HanHaocheng on 2024/4/1.
//

#ifndef MY_PROJECT_MYAI_MEMORY_IO_H
#define MY_PROJECT_MYAI_MEMORY_IO_H
#include "myai_io_interface.h"

MYAI_SPACE_BEGIN
class MyaiMemoryIO : public MyaiIOBase {

  MyaiMemoryIO() : MyaiIOBase(Node::ND_MEMORY) {}
};

MYAI_SPACE_END
#endif//MY_PROJECT_MYAI_MEMORY_IO_H
