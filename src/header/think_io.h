//
// Created by HanHaocheng on 2023/04/05.
//

#ifndef THINKSYSTEM_THINK_IO_H
#define THINKSYSTEM_THINK_IO_H
#include "data_manage.h"
#include "data_type.h"
#include <list>
#include <map>


namespace think {
using ActFun = bool (*)(EleInfo::link_type);

class MemorySystem {
  std::vector<NodeInfo>m_tmp_nodes_;
public:
  void getActionInfo(std::vector<EleInfo> &out) {
  }
  void getSenseInfo(std::vector<EleInfo> &out) {
  }
  void runAct(const std::vector<EleInfo> &in) {

  }

  void setMemoryNode(){

  }
};


// 生成一级感知信息

// 生成一级行为信息

// 二级感知节点指向二级行为节点

// 执行一级行为信息

}// namespace think

#endif//THINKSYSTEM_THINK_IO_H
