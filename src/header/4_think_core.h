#pragma once

#include "3_think_manage.h"

namespace think {

static double NUM_DY_SAVE_STANDARD = 0.5;
static double NUM_ST_SAVE_STANDARD = 0.99;

class CThinkCore {
  struct act_node_info : SP_NODE_INFO {
    ACTIVE_VAL actWeight;
  };
  std::unordered_map<NODE_ID, act_node_info> m_cMapActiveNode_;

  CNodeManage m_cNodeManage_;

public:
  CThinkCore() {
  }

  ~CThinkCore() {
  }

  void longRun() {
    shortRun(0x120);
    m_cNodeManage_.saveAllNode(0.5, 0x10);
  }

  bool shortRun(size_t actNodeMaxSize) {
    do {

    } while (m_cNodeManage_.getBufferSize() < actNodeMaxSize);
  }


  void setActWeight(NODE_ID actId, ACTIVE_VAL actWeight, ACTIVE_VAL actMax, ACTIVE_VAL actMin) {
    if (actMin < actWeight) { return; }
    auto findRes = m_cMapActiveNode_.find(actId);
    if (findRes != m_cMapActiveNode_.end()) {
      findRes->second.actWeight = std::min(actMax, actWeight + findRes->second.actWeight);
    } else {
      m_cMapActiveNode_.emplace(actId, act_node_info{m_cNodeManage_.find(actId)->second, std::min(actMax, actWeight)});
    }
  }

  //目标：获取激活后的信息
  //前提：一级节点希望以 原始链接值（） 连接到激活节点
  std::unordered_map<NODE_ID, act_node_info>* getActInfo() {
    return &m_cMapActiveNode_;
  };
};
}// namespace think