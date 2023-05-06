#ifndef THINKSYSTEM_3_THINK_io_H
#define THINKSYSTEM_3_THINK_io_H

#include "0_think_type.h"
#include "2_io_node.h"

namespace think {
using ACT_WEIGHT = WEIGHT;
struct Sentiment {
};

using ACT_FUNC = void (*)(active_t);

class CIOManage : public CNodeIOManage {
  struct active_info {
    NODETYPE nodetype;
    active_t actVal;
  };

private:
  std::unordered_map<NODE_ID, ACT_FUNC> m_cMapConstAct_;
  std::unordered_map<CONSTTYPE, std::vector<std::pair<NODE_ID, SP_NODE_INFO>>> m_cMapConstNode_;

  std::unordered_map<NODE_ID, active_info> m_cMapTmpActiveNode;
  active_t m_iValActMax_;

public:
  CIOManage() {
    _initConstId();
  }

  size_t activeNodeSize() { return m_cMapTmpActiveNode.size(); }

  void updateBuffer() {
    m_cMapNodeBuffer_.clear();
    for (const auto &[type, typeList]: m_cMapConstNode_) {
      m_cMapNodeBuffer_.insert(typeList.begin(), typeList.end());
    }
  }

  // 输出 - 传入输出行为信息(思维输出，行为输出)

  void clearActiveBuffer() {
    m_cMapTmpActiveNode.clear();
  }

  UP_NODE_INFO setOutput(const CLinkList &dyLinks, const CLinkList &stLinks, const CLinkList &csLinks) {
    _setActiveInfo(dyLinks, ENodeType::DYNAMIC_TYPE);
    _setActiveInfo(stLinks, ENodeType::STATIC_TYPE);
    _setActiveInfo(csLinks, ENodeType::CONST_TYPE);
    return std::make_unique<CNodeInfo>(ENodeType::DYNAMIC_TYPE, dyLinks, stLinks, csLinks);
  }

  // 输入 - 传入输入感知信息(思维输入，感知输入)
  void getInput(const SP_NODE_INFO &out) {
    for (const auto &[id,actInfo]: m_cMapTmpActiveNode) {
      if (!!((int) actInfo.nodetype & (int) ENodeType::CONST_TYPE)) {
        m_cMapConstAct_.at(id)(actInfo.actVal);
      } else {
        find(id)->second->getActGroup(actInfo.actVal, out);
      }
    }
  }

private:
  void _setActiveInfo(const CLinkList &linkList, NODETYPE nodetype) {
    for (const auto &link: linkList) {
      auto fRes = m_cMapTmpActiveNode.find(link.pNode);
      if (fRes != m_cMapTmpActiveNode.end()) {
        fRes->second.actVal = std::min(m_iValActMax_, link.uiValLink + fRes->second.actVal);
      } else {
        m_cMapTmpActiveNode.emplace(link.pNode, active_info{nodetype, link.uiValLink});
      }
    }
  }

  struct ConstIdFileHeader {
    CONSTTYPE constType;
    size_t idSize;
  };
  constexpr static const char *STR_CONST_ID_FOLDER = "./const_id/";

  bool _initConstId() {
    if (!fs::exists(STR_CONST_ID_FOLDER)) { return false; }
    for (const auto &[eNodeType, szNodeType]: MAP_CONST_TYPE_TO_STRING) {
      std::ifstream idFile{STR_CONST_ID_FOLDER + szNodeType, std::ios::binary | std::ios::in};
      if (!idFile.is_open()) {
        std::cerr << "产生未知错误，静态节点id文件读取时打开失败！" << std::endl;
        continue;
      }

      ConstIdFileHeader fileHeader{};
      idFile.read(reinterpret_cast<char *>(&fileHeader), sizeof(ConstIdFileHeader));
      auto vecRes = m_cMapConstNode_.emplace(eNodeType,
                                             std::vector<std::pair<NODE_ID, SP_NODE_INFO>>{})
                            .first;
      vecRes->second.resize(fileHeader.idSize);
      for (auto &node: vecRes->second) {
        idFile.read(reinterpret_cast<char *>(&node.first), sizeof(NODE_ID));
        node.second = std::make_unique<CNodeInfo>(node.first);
      }
      idFile.close();
    }
    return true;
  }

  bool _saveConstId() {
    if (!fs::exists(STR_CONST_ID_FOLDER)) { fs::create_directories(STR_CONST_ID_FOLDER); }
    for (auto &[constType, fileName]: MAP_CONST_TYPE_TO_STRING) {
      std::ofstream idFile{STR_CONST_ID_FOLDER + fileName, std::ios::binary | std::ios::out};
      if (!idFile.is_open()) {
        std::cerr << "产生未知错误，静态节点id文件写入时打开失败！" << std::endl;
        continue;
      }

      auto mapIter = m_cMapConstNode_.find(constType);
      ConstIdFileHeader cnidHeader{constType, mapIter->second.size()};
      idFile.write(reinterpret_cast<const char *>(&cnidHeader.idSize), sizeof(ConstIdFileHeader));
      for (const auto &[id, spLG]: mapIter->second) {
        idFile.write(reinterpret_cast<const char *>(&id), sizeof(NODE_ID));
      }
      idFile.close();
    }
  };
};

}// namespace think
#endif