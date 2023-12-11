//
// Created by HanHaocheng on 2023/11/29.
//

#include "NodeIdAllocator.h"

namespace myai
{
NodeIdAllocator::NodeIdAllocator() = default;
NodeIdAllocator::NodeIdAllocator(uint64_t minUnusedId, const std::vector<uint64_t> &unusedIds) : m_minUnusedId(minUnusedId), m_unusedIds(unusedIds.begin(), unusedIds.end()) {}
uint64_t NodeIdAllocator::allocate()
{
  if (!m_unusedIds.empty()) {
    size_t res = m_unusedIds.front();
    m_unusedIds.pop_front();
    return res;
  }
  return m_minUnusedId++;
}

void NodeIdAllocator::deallocate(uint64_t id)
{
  m_unusedIds.emplace_front(id);
}

}// namespace myai