//
// Created by HanHaocheng on 2023/11/29.
//

#include "NodeDao.h"
#include <exception>
#include <stdexcept>

namespace myai
{
int NodeDao::insert(const Node::ptr &node)
{
#if DEBUG
  if (node == nullptr) {
    throw std::logic_error{"<error NodeDao::insert>node is nullptr."};
  } else if (node->getId() == 0) {
    throw std::logic_error{"<error NodeDao::insert>node id is 0."};
  } else if (node->getType() == Node::Type::UNKNOWN_NODE) {
    throw std::logic_error{"<error NodeDao::insert>node type is UNKNOW_NODE."};
  }
#endif
  bool success = m_nodeCache.emplace(node->getId(), node).second;
  return success;
}
}// namespace myai