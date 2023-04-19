//
// Created by HanHaocheng on 2023/04/05.
//

#ifndef THINKSYSTEM_INOUT_H
#define THINKSYSTEM_INOUT_H
#include "../type/type_node.h"
#include "manage.h"

#include <list>
#include <map>

namespace think {
using ActFun = bool (*)(LinkInfo::link_type);

class in_type {
  virtual void setInputData() = 0;
  virtual void input()        = 0;
};

class out_type {
  virtual void getOutputData() = 0;
  virtual void output()        = 0;
};

class MemorySystem {

public:
  void getActionInfo(std::vector<LinkInfo> &out) {}
  void getSenseInfo(std::vector<LinkInfo> &out) {}
  void runAct(const std::vector<LinkInfo> &in) {}

  void setMemoryNode() {
  }
};

class IOManageSystem{

  void addNewInput(NodeType type,in_type*){

  }

  void addNewOutput(NodeType type,out_type*){

  }

  void refreshInputInfo(){}

  void getInputInfo(){}

  void refreshOutputInfo(){}

  void setOutputInfo(){}

  // 刷新感知信息
  // 获取感知信息
  //

  // 生成一级感知信息

  // 生成一级行为信息

  // 二级感知节点指向二级行为节点

  // 执行一级行为信息
};

}// namespace think

#endif//THINKSYSTEM_INOUT_H
