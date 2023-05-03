#ifndef THINKSYSTEM_3_THINK_io_H
#define THINKSYSTEM_3_THINK_io_H

#include "1_think_type.h"

namespace think {
using ACT_WEIGHT = WEIGHT;

class CIOManage {
private:
  std::unordered_map<NODE_ID, ACT_WEIGHT> m_cMapTmpActiveNode;

public:
  void setActWeight(NODE_ID actId, ACT_WEIGHT actWeight, ACT_WEIGHT actMax, ACT_WEIGHT actMin) {
    auto findRes = m_cMapTmpActiveNode.find(actId);
    if (findRes != m_cMapTmpActiveNode.end()) {
      findRes->second = std::max(actMin, std::min(actMax, actWeight + findRes->second));
    } else {
      m_cMapTmpActiveNode.emplace(actId, std::max(actMin, std::min(actMax, actWeight)));
    }
  }

  void actNode(){

  }

  void get
};

}// namespace think
#endif