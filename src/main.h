//
// Created by HanHaocheng on 2023/04/05.
//

#ifndef THINKSYSTEM_MAIN_H
#define THINKSYSTEM_MAIN_H
#include "header/system/allocator.h"
#include "header/system/inout.h"
#include "header/system/manage.h"
#include <map>

namespace think {

class ThinkCore {
  NodeIDAllocator *m_id_alloc_;
  std::unordered_map<NMSType, NodeManageSystem *> m_nms_;

public:
  ThinkCore() {
    m_id_alloc_                  = new NodeIDAllocator(COUNT_MAX_ID_SIZE);
    m_nms_[NMSType::CONST_NMS]   = new ConstManageSystem(*m_id_alloc_);
    m_nms_[NMSType::STATIC_NMS]  = new StaticManageSystem(*m_id_alloc_);
    m_nms_[NMSType::DYNAMIC_NMS] = new DynamicManageSystem(*m_id_alloc_);
    auto a = dynamic_cast<ConstManageSystem*>(m_nms_[NMSType::CONST_NMS]);

  }

  ~ThinkCore() {
    for (auto& item: m_nms_){
      delete item.second;
      item.second = nullptr;
    }
    delete m_id_alloc_;
  }
};
}// namespace think
#endif//THINKSYSTEM_MAIN_H
