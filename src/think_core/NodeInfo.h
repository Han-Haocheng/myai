//
// Created by HanHaocheng on 2023/05/12.
//

#ifndef THINKSYSTEM_NODEINFO_H
#define THINKSYSTEM_NODEINFO_H
#include "CoreInclude.h"
#include "LinkList.h"

namespace think
{

class NodeInfo
{
  friend class CIOManage;

private:
  struct ConstFileInfo
  {
    size_t const_count;
    size_t static_count;
  };
  enum node_state : unsigned int
  {
    IS_INITIALIZED = 0b0001,//是初始化的
    IS_STABLE      = 0b0010,//是稳定的
    IS_DUPLICATED  = 0b0100,//是去重的
  };

private:
  unsigned int m_state_  = 0;
  unsigned int m_iActNum = 0;
  ENodeType m_type_      = ENodeType::NULL_TYPE;
  LinkList m_lsConstLink;
  LinkList m_lsStaticLink;
  LinkList m_lsDynamicLink;

public:
  NodeInfo() = default;
  explicit NodeInfo(node_id id);
  explicit NodeInfo(ENodeType nodetype);

  NodeInfo(ENodeType nodetype, LinkList ctLks, LinkList scLks, LinkList dyLks);

  void clear();

  //稳定链接
  void stableLink(const WEIGHT<link_val> &dySaveWeight);

  //削减链接
  void reduceLink(link_val stand);

  bool initNodeInfo(node_id id) noexcept(false);

  bool writeNodeInfo(node_id id);

};// class NodeInfo

using CLINKGROUP   = const NodeInfo;
using UP_NODE_INFO = std::unique_ptr<NodeInfo>;
using SP_NODE_INFO = std::shared_ptr<NodeInfo>;
using WPLINKGROUP  = std::weak_ptr<NodeInfo>;

using node_type    = std::pair<const node_id, std::shared_ptr<NodeInfo>>;//节点
using node_list    = std::vector<node_type>;
}// namespace think

#endif//THINKSYSTEM_NODEINFO_H
