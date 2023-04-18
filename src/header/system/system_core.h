//
// Created by HanHaocheng on 2023/04/05.
//

#ifndef THINKSYSTEM_SYSTEM_CORE_H
#define THINKSYSTEM_SYSTEM_CORE_H
#include "system_io.h"
#include "system_manage.h"

namespace think {
constexpr const static size_t COUNT_MAX_ID_SIZE = 0x1000000000000U;

class ThinkCore {
  NodeManageSystem* m_const_nms_ = new ConstManageSystem();
  NodeManageSystem* m_static_nms_ = new StaticManageSystem();
  NodeManageSystem* m_dynamic_nms_ = new DynamicManageSystem();
public:
  ThinkCore()  {
  }
};
}// namespace think
#endif//THINKSYSTEM_SYSTEM_CORE_H
