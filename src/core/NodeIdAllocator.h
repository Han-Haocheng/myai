//
// Created by HanHaocheng on 2023/11/29.
//

#ifndef THINK_NODEIDALLOCATOR_H
#define THINK_NODEIDALLOCATOR_H

#include <cstddef>
#include <cstdint>
#include <forward_list>
#include <memory>
#include <vector>
namespace myai
{

class NodeIdAllocator
{
public:
  using ptr = std::shared_ptr<NodeIdAllocator>;

  NodeIdAllocator();
  NodeIdAllocator(uint64_t minUnusedId, const std::vector<uint64_t> &unusedIds);
  uint64_t allocate();
  void deallocate(uint64_t id);

private:
  uint64_t m_minUnusedId = 0;
  std::forward_list<uint64_t> m_unusedIds = {};
};

}// namespace myai

#endif//THINK_NODEIDALLOCATOR_H
