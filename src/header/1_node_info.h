//
// Created by HanHaocheng on 2023/05/05.
//

#ifndef THINKSYSTEM_1_NODE_INFO_H
#define THINKSYSTEM_1_NODE_INFO_H
#include "1_link_list.h"

namespace think {

class CNodeInfo {
  friend class CNodeIOManage;
  unsigned int m_iNumFind_ = 0;
  NODETYPE nodetype        = NODETYPE::NULL_TYPE;
  CLinkList ctLks;
  CLinkList scLks;
  CLinkList dyLks;

public:
  CNodeInfo() = default;
  explicit CNodeInfo(NODE_ID id) { readNodeInfo(id); }
  explicit CNodeInfo(NODETYPE nodetype) : nodetype(nodetype), ctLks(), scLks(), dyLks() {}


  CNodeInfo(NODETYPE nodetype, CLinkList ctLks, CLinkList scLks, CLinkList dyLks)
      : nodetype(nodetype), ctLks(std::forward<CLinkList>(ctLks)), scLks(std::forward<CLinkList>(scLks)), dyLks(std::forward<CLinkList>(dyLks)) {}


  void getActGroup(active_t actWeight, const std::shared_ptr<CNodeInfo> &out) {

    out->ctLks.insert(this->ctLks * (static_cast<WEIGHT>(actWeight) / (NUM_LINK_VAL_MAX + 1)));
    out->dyLks.insert(this->dyLks * (static_cast<WEIGHT>(actWeight) / (NUM_LINK_VAL_MAX + 1)));
    out->scLks.insert(this->scLks * (static_cast<WEIGHT>(actWeight) / (NUM_LINK_VAL_MAX + 1)));
  }

  void actNode(active_t iValAct, std::unordered_map<std::pair<node_t, CNodeInfo> *, active_t> *cMapAct) {
    cMapAct->insert();
  }

  //ÎÈ¶¨½Úµã
  void stableNode(NODE_ID id, WEIGHT dySaveWeight) {
    dyLks *= dySaveWeight;
    scLks.insert(dyLks);
    scLks.deduplicate();
    ctLks.deduplicate();
    initNodeInfo(id);
  }

  struct FileInfo {
    size_t const_count;
    size_t static_count;
  };

  bool readNodeInfo(NODE_ID id) {
    std::string path = GetNodeParentPath(id);
    if (!fs::exists(path + STR_NODE_FILE_NAME)) {
      return false;
    }
    std::ifstream file{path + STR_NODE_FILE_NAME, std::ios ::binary | std::ios ::in};
    if (!file.is_open()) {
      return false;
    }

    FileInfo tmpInfo{};
    file.read(reinterpret_cast<char *>(&tmpInfo), sizeof(FileInfo));
    ctLks.resize(tmpInfo.const_count);
    scLks.resize(tmpInfo.static_count);
    file.read(reinterpret_cast<char *>(ctLks.data()),
              static_cast<std::streamsize>(sizeof(slink_t) * tmpInfo.const_count));
    file.read(reinterpret_cast<char *>(scLks.data()),
              static_cast<std::streamsize>(sizeof(slink_t) * tmpInfo.static_count));
    file.close();
    return true;
  }


private:
  bool initNodeInfo(NODE_ID id) {
    std::string path = GetNodeParentPath(id);
    if (!fs::exists(path)) {
      fs::create_directories(path);
    }

    std::ofstream file{path + STR_NODE_FILE_NAME, std::ios ::binary | std::ios ::out};
    if (!file.is_open()) {
      return false;
    }

    FileInfo tmpInfo{
            ctLks.size(),
            scLks.size(),
    };

    file.write(reinterpret_cast<const char *>(&tmpInfo), sizeof(FileInfo));
    file.write(reinterpret_cast<const char *>(ctLks.data()),
               static_cast<std::streamsize>(sizeof(slink_t) * tmpInfo.const_count));
    file.write(reinterpret_cast<const char *>(scLks.data()),
               static_cast<std::streamsize>(sizeof(slink_t) * tmpInfo.static_count));
    file.close();
    return true;
  }

};// struct CNodeInfo

using CLINKGROUP   = const CNodeInfo;
using UP_NODE_INFO = std::unique_ptr<CNodeInfo>;
using SP_NODE_INFO = std::shared_ptr<CNodeInfo>;
using WPLINKGROUP  = std::weak_ptr<CNodeInfo>;


}// namespace think
#endif//THINKSYSTEM_1_NODE_INFO_H
