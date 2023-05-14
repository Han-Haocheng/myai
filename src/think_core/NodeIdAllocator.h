//
// Created by HanHaocheng on 2023/05/12.
//

#ifndef THINKSYSTEM_NODEIDALLOCATOR_H
#define THINKSYSTEM_NODEIDALLOCATOR_H
#include "../base_type/range.h"
#include "DefType.h"

namespace think
{

class NodeIdAllocator
{
private:
  using IdRange = Range<node_id>;
  std::forward_list<IdRange> m_cListUnassignedIDs;

  constexpr static const char *STR_UNASSIGNED_PATH = "id_allocator.dat";

public:
  explicit NodeIdAllocator(size_t nodeCount)
  {
    if (fs::exists(STR_UNASSIGNED_PATH)) { _getUnassignedId(STR_UNASSIGNED_PATH); }
    else { m_cListUnassignedIDs.emplace_front(node_id{1}, node_id{nodeCount}); }
  }

  ~NodeIdAllocator() { _saveUnassignedId(STR_UNASSIGNED_PATH); }

  /// 申请节点（时间复杂度O(1)）
  /// \param out
  node_id allocate()
  {
    node_id out = m_cListUnassignedIDs.front().begin()++;
    if (m_cListUnassignedIDs.front().is_empty()) { m_cListUnassignedIDs.pop_front(); }
    return out;
  }

  /// 申请节点（时间复杂度O(n)）
  /// \param out
  /// \param count
  void allocate(std::vector<node_id> &out, size_t count)
  {
    if (!out.empty()) { out.clear(); }
    out.reserve(count);
    auto itBeg = m_cListUnassignedIDs.before_begin();
    while (itBeg != m_cListUnassignedIDs.end())
    {
      do {
        out.emplace_back(itBeg->begin()++);
        --count;
        if (count == 0) { return; }
      } while (itBeg->begin() != itBeg->end());
      m_cListUnassignedIDs.erase_after(itBeg++);
    }
    throw std::overflow_error("Error:空闲节点数量为空，请扩充节点！");
  }

  /// 释放节点id（时间复杂度 O(n)）
  /// \param in
  void release(const node_id &in)
  {
    //判断释放节点是否在最前面
    auto itBeg = m_cListUnassignedIDs.begin();
    IdRange installRange(in, in + 1);
    if (in < itBeg->begin())
    {
      if (installRange.end() == itBeg->begin()) { --itBeg->begin(); }
      else { m_cListUnassignedIDs.emplace_front(installRange); }
    }
    else
    {
      auto itBfBeg = m_cListUnassignedIDs.before_begin();

      //查找第一个begin_大于in的位置
      for (; itBeg != m_cListUnassignedIDs.end(); ++itBeg, ++itBfBeg)
      {
        if (itBeg->begin() > in)
        {
          //in在上一个范围中
          ////抛出异常
          //in在上一个范围结束和这个范围起始的位置
          ////
          if (itBfBeg->contains(in)) { throw std::runtime_error("Error:节点释放异常，"); }
          if (itBfBeg->end() == installRange.begin())
          {
            if (installRange.end() == itBeg->begin())
            {
              itBfBeg->end() = itBeg->end();
              m_cListUnassignedIDs.erase_after(itBfBeg);
            }
            else { itBfBeg->end() = installRange.end(); }
          }
          else if (installRange.end() == itBeg->begin()) { itBeg->begin() = installRange.begin(); }
          else { auto newIter = m_cListUnassignedIDs.insert_after(itBfBeg, installRange); }
        }
      }

      //假如释放位置未找到，说明释放位置为最后一个范围后
      if (itBfBeg->end() == in) { ++itBfBeg->end(); }
      else { m_cListUnassignedIDs.insert_after(itBfBeg, {in, in + 1}); }
    }
  }

  /// 释放节点（时间复杂度 O(n^2)）
  /// \param in
  void release(const std::vector<node_id> &in)
  {
    if (in.empty()) { return; }
    for (const auto item: in) { release(item); }
  }

private:
  void _getUnassignedId(const std::string &path)
  {
    std::ifstream file;
    IdRange tmp;
    file.open(path, std::ios::binary | std::ios::in);
    if (file.is_open())
    {
      while (file.read(reinterpret_cast<char *>(&tmp), sizeof(IdRange))) { m_cListUnassignedIDs.emplace_front(tmp); }
      file.close();
    }
  }
  void _saveUnassignedId(const std::string &path)
  {
    std::ofstream file(path, std::ios::binary | std::ios::out);
    if (file.is_open())
    {
      for (const auto &id: m_cListUnassignedIDs) { file.write(reinterpret_cast<const char *>(&id), sizeof(IdRange)); }
      file.close();
    }
  }
};//! class NodeIDAllocator

}// namespace think

#endif//THINKSYSTEM_NODEIDALLOCATOR_H
