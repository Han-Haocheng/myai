#ifndef MYAI_ID_ALLOCATOR_H_
#define MYAI_ID_ALLOCATOR_H_

#include "myai_def.h"
#include <memory>

MYAI_SPACE_BEGIN
class MyaiIdAllocator {
public:
  using ptr = std::shared_ptr<MyaiIdAllocator>;


  MyaiIdAllocator();
  ~MyaiIdAllocator();

private:
};

MYAI_SPACE_END

#endif// !MYAI_ID_ALLOCATOR_H_
