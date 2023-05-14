//
// Created by HanHaocheng on 2023/05/12.
//

#include "NodeInfo.h"

namespace think
{
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
  std::ifstream file{path, std::ios ::binary | std::ios ::in};
  if (!file.is_open())
  {

    if (!fs::exists(path))
    {
      throw think_error("初始化节点信息错误，文件不存在", std::make_error_code(std::errc::no_such_file_or_directory));
    }
    else
    {
      throw think_error{"初始化节点信息错误，错误原因未知，文件打开失败",
                        std::make_error_code(std::errc::no_such_file_or_directory)};
    }
  }
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
  m_lsStaticLink.insert(m_lsDynamicLink, think::LinkList::const_iterator(), think::LinkList::const_iterator());
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
NodeInfo::NodeInfo(node_id id) { initNodeInfo(id); }

}// namespace think