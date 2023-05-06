#pragma once

#include "3_io_manage.h"

namespace think {

static double NUM_DY_SAVE_STANDARD = 0.5;
static double NUM_ST_SAVE_STANDARD = 0.99;

class CThinkCore {
  struct act_node_info : SP_NODE_INFO {
    active_t actWeight;
  };

  CIOManage m_cIOManage_;
  bool flag = true;
public:
  CThinkCore() {
  }

  ~CThinkCore() {
  }

  void longRun() {
    do {
      shortRun(0x120);
      m_cIOManage_.saveAllNode(0.5, 0x10);
      m_cIOManage_.updateBuffer();
    } while (flag);
  }

  bool shortRun(size_t actNodeMaxSize) {
    do {

    } while (m_cIOManage_.getBufferSize() < actNodeMaxSize);
  }


  //目标：获取激活后的信息
  //前提：一级节点希望以 原始链接值（） 连接到激活节点
  std::unordered_map<NODE_ID, act_node_info>* getActInfo() {
    return &m_cMapActiveNode_;
  };
};
}// namespace think