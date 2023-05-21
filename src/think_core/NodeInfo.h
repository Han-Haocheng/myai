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
  friend class IOManage;

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

  void emplace(LinkList&other) { return; }

  //稳定链接
  void stableLink(const WEIGHT<link_val> &dySaveWeight);

  //削减链接
  void reduceLink(link_val stand);

  /// 从文件中初始化节点信息
  /// \param id 使用的初始化id
  /// \return 返回是否初始化成功
  bool initNodeInfo(node_id id) noexcept(false);

  /// 将信息保存入文件
  /// \param id 要保存的节点id
  /// \return 是否保存成功
  bool writeNodeInfo(node_id id);

};// class NodeInfo

using CLINKGROUP   = const NodeInfo;
using UP_NODE_INFO = std::unique_ptr<NodeInfo>;
using SP_NODE_INFO = std::shared_ptr<NodeInfo>;
using WPLINKGROUP  = std::weak_ptr<NodeInfo>;

using node_type    = std::pair<const node_id, std::shared_ptr<NodeInfo>>;//节点
using node_list    = std::vector<node_type>;

NodeInfo::NodeInfo(ENodeType nodetype, LinkList ctLks, LinkList scLks, LinkList dyLks)
: m_type_(nodetype),
  m_lsConstLink(std::forward<LinkList>(ctLks)),
  m_lsStaticLink(std::forward<LinkList>(scLks)),
  m_lsDynamicLink(std::forward<LinkList>(dyLks))
{
}

NodeInfo::NodeInfo(ENodeType nodetype) : m_type_(nodetype), m_lsConstLink(), m_lsStaticLink(), m_lsDynamicLink() {}

bool NodeInfo::writeNodeInfo(node_id id)
{
  if (m_state_ ^ IS_INITIALIZED) { return false; }
  std::string path = GetNodeParentPath(id);
  if (!fs::exists(path)) { fs::create_directories(path); }

  std::ofstream file{path + STR_NODE_FILE_NAME, std::ios ::binary | std::ios ::out};
  if (!file.is_open())
  {
    throw think_error{"写入节点信息错误，错误原因未知，文件打开失败",
                      std::make_error_code(std::errc::no_such_file_or_directory)};
  }

  ConstFileInfo tmpInfo{
          m_lsConstLink.size(),
          m_lsStaticLink.size(),
  };

  file.write(reinterpret_cast<const char *>(&tmpInfo), sizeof(ConstFileInfo));
  file.write(reinterpret_cast<const char *>(m_lsConstLink.data()),
             static_cast<std::streamsize>(sizeof(link_t) * tmpInfo.const_count));
  file.write(reinterpret_cast<const char *>(m_lsStaticLink.data()),
             static_cast<std::streamsize>(sizeof(link_t) * tmpInfo.static_count));
  file.close();
  return true;
}
bool NodeInfo::initNodeInfo(node_id id) noexcept(false)
{
  if (m_state_ & IS_INITIALIZED) { return false; }
  std::string path = GetNodeParentPath(id) + STR_NODE_FILE_NAME;
  if (!fs::exists(path)) return false;

  std::ifstream file{path, std::ios ::binary | std::ios ::in};
  if (!file.is_open())
    throw think_error{"初始化节点信息错误，错误原因未知，文件打开失败",
                      std::make_error_code(std::errc::no_such_file_or_directory)};
  ConstFileInfo tmpInfo{};
  file.read(reinterpret_cast<char *>(&tmpInfo), sizeof(ConstFileInfo));
  m_lsConstLink.resize(tmpInfo.const_count);
  m_lsStaticLink.resize(tmpInfo.static_count);
  file.read(reinterpret_cast<char *>(m_lsConstLink.data()),
            static_cast<std::streamsize>(sizeof(link_t) * m_lsConstLink.size()));
  file.read(reinterpret_cast<char *>(m_lsStaticLink.data()),
            static_cast<std::streamsize>(sizeof(link_t) * m_lsStaticLink.size()));
  file.close();
  m_state_ |= IS_INITIALIZED;
  return true;
}

void NodeInfo::reduceLink(link_val stand)
{
  if (m_state_ ^ IS_DUPLICATED)
  {
    m_lsDynamicLink.deduplicate();
    m_lsStaticLink.deduplicate();
    m_lsConstLink.deduplicate();
    m_state_ |= IS_DUPLICATED;
  }
  m_lsDynamicLink.erase(stand);
  m_lsStaticLink.erase(stand);
  m_lsConstLink.erase(stand);
}
void NodeInfo::stableLink(const WEIGHT<link_val> &dySaveWeight)
{
  if (m_state_ & IS_STABLE) { return; }
  m_lsDynamicLink *= dySaveWeight;
  m_lsStaticLink.insert(m_lsDynamicLink);
  m_lsStaticLink.deduplicate();
  if (m_state_ ^ IS_DUPLICATED)
  {
    m_lsConstLink.deduplicate();
    m_state_ |= IS_DUPLICATED;
  }
  m_state_ |= IS_STABLE;
}
void NodeInfo::clear()
{
  if (m_state_ ^ IS_INITIALIZED) { return; }
  m_lsConstLink.clear();
  m_lsStaticLink.clear();
  m_lsDynamicLink.clear();
  m_state_ &= ~IS_INITIALIZED;
}
NodeInfo::NodeInfo(node_id id)
{
  if (!initNodeInfo(id)) {

  }
}

}// namespace think

#endif//THINKSYSTEM_NODEINFO_H
