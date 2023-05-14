#ifndef THINKSYSTEM_3_THINK_io_H
#define THINKSYSTEM_3_THINK_io_H

#include "../inout/io_include.h"
#include "../think_core/DefType.h"
#include "../think_core/LinkList.h"
#include "2_io_node.h"

namespace think
{

using CActFunc = void (*)(active_t);

class CIOManage : public CNodeIOManage
{
  class ActNodeInfo : public NodeInfo
  {
    active_t actVal = 0;

  public:
    static active_t NUM_ACT_VAL_MAX, NUM_ACT_VAL_MIN;
    void act(NodeInfo &out) {}
    void act(ActFunc actFunc) const { actFunc(actVal); }
    void operator+=(link_val linkVal) { actVal = std::clamp(actVal + linkVal, NUM_ACT_VAL_MIN, NUM_ACT_VAL_MAX); }
  };

private:
  std::unordered_map<node_id, std::shared_ptr<NodeInfo>> m_mapNodeBuffer;//节点缓存

  std::unordered_map<ENodeType, std::vector<node_type>> m_mapConstNode_;//常态节点标识缓存

  std::unordered_map<node_id, CActFunc> m_mapConstAct_;//常态行为

  std::unordered_map<node_id, std::shared_ptr<ActNodeInfo>> m_mapActiveNode;//激活节点缓存

  std::vector<io::ThinkIO *> m_lsIOSystem_;

  think::io::IOMemory *m_ioMemory_;

public:
  CIOManage()
  {
    ActNodeInfo::NUM_ACT_VAL_MAX = 0;
    ActNodeInfo::NUM_ACT_VAL_MIN = 0;
    _initConstId();
  }

  size_t activeNodeSize() { return m_mapActiveNode.size(); }

  void updateBuffer()
  {
    m_cMapNodeBuffer_.clear();
    for (const auto &[type, typeList]: m_mapConstNode_) { m_cMapNodeBuffer_.insert(typeList.begin(), typeList.end(), think::LinkList::const_iterator()); }
  }

  // 输出 - 传入输出行为信息(思维输出，行为输出)

  void clearActiveBuffer() { m_mapActiveNode.clear(); }
  UP_NODE_INFO setOutput(const LinkList &dyLinks, const LinkList &stLinks, const LinkList &csLinks) {}

  // 输入 - 传入输入感知信息(思维输入，感知输入)
  void getInput(const SP_NODE_INFO &out) {}

private:
  void _setActiveInfo(const LinkList &linkList, ENodeType nodetype)
  {
    for (const auto &link: linkList)
    {
      auto fRes = m_mapActiveNode.find(link.id);
      if (fRes != m_mapActiveNode.end()) { fRes->second = std::min(m_iValActMax_, link.linkVal + fRes->second.actVal); }
      else { m_mapActiveNode.emplace(link.id, active_info{nodetype, link.uiValLink}); }
    }
  }

  struct ConstIdFileHeader
  {
    ENodeType constType;
    size_t idSize;
  };
  constexpr static const char *STR_CONST_ID_FOLDER = "./const_id/";

  bool _initConstId()
  {
    if (!fs::exists(STR_CONST_ID_FOLDER)) { return false; }
    for (const auto &[eNodeType, szNodeType]: ARRAY_TYPE_TO_STRING)
    {
      std::ifstream idFile{STR_CONST_ID_FOLDER + szNodeType, std::ios::binary | std::ios::in};
      if (!idFile.is_open())
      {
        std::cerr << "产生未知错误，静态节点id文件读取时打开失败！" << std::endl;
        continue;
      }

      ConstIdFileHeader fileHeader{};
      idFile.read(reinterpret_cast<char *>(&fileHeader), sizeof(ConstIdFileHeader));
      auto vecRes = m_mapConstNode_.emplace(eNodeType, std::vector<std::pair<node_id, SP_NODE_INFO>>{}).first;
      vecRes->second.resize(fileHeader.idSize);
      for (auto &node: vecRes->second)
      {
        idFile.read(reinterpret_cast<char *>(&node.first), sizeof(node_id));
        node.second = std::make_unique<NodeInfo>(node.first);
      }
      idFile.close();
    }
    return true;
  }

  bool _saveConstId()
  {
    if (!fs::exists(STR_CONST_ID_FOLDER)) { fs::create_directories(STR_CONST_ID_FOLDER); }
    for (auto &[constType, fileName]: ARRAY_TYPE_TO_STRING)
    {
      std::ofstream idFile{STR_CONST_ID_FOLDER + fileName, std::ios::binary | std::ios::out};
      if (!idFile.is_open())
      {
        std::cerr << "产生未知错误，静态节点id文件写入时打开失败！" << std::endl;
        continue;
      }

      auto mapIter = m_mapConstNode_.find(constType);
      ConstIdFileHeader cnidHeader{constType, mapIter->second.size()};
      idFile.write(reinterpret_cast<const char *>(&cnidHeader.idSize), sizeof(ConstIdFileHeader));
      for (const auto &[id, spLG]: mapIter->second)
      {
        idFile.write(reinterpret_cast<const char *>(&id), sizeof(node_id));
      }
      idFile.close();
    }
  }
};

}// namespace think
#endif