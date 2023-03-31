//
// Created by HanHaocheng on 2023/04/05.
//

#ifndef THINKSYSTEM_THINK_CORE_H
#define THINKSYSTEM_THINK_CORE_H
#include "data_manage.h"
#include "think_io.h"

namespace think {
constexpr const static size_t COUNT_MAX_ID_SIZE = 0x1000000000000U;

class ThinkCore {
  NodeIDAllocator m_id_alloc;
  NodeFileDatabase m_node_db_;
public:
  ThinkCore()  {
  }
};
}// namespace think
#endif//THINKSYSTEM_THINK_CORE_H
