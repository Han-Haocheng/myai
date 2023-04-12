//
// Created by HanHaocheng on 2023/03/31.
//

#ifndef DATA_MANAGE_H_
#define DATA_MANAGE_H_
#include "data_type.h"
#include "type/type_range.h"
#include <forward_list>
#include <list>
#include <stdexcept>

#include <filesystem>
#include <fstream>
#include <map>

namespace fs = std::filesystem;

namespace think {

class NodeIDAllocator {
private:
  using NodeRange = Range<NodeID>;
  std::forward_list<NodeRange> m_id_datas_;


  constexpr static const char *allocator_path = "id_allocator.dat";

public:
  explicit NodeIDAllocator(size_t nodeCount) {
    NodeRange tmp{NodeID{0}, NodeID{nodeCount}};
    if (fs::exists(allocator_path)) {
      read_null_id(allocator_path);
    } else {
      m_id_datas_.emplace_front(tmp);
    }
  }

  ~NodeIDAllocator() {
    write_null_id(allocator_path);
  }

  /// 申请节点（时间复杂度O(1)）
  /// \param out
  NodeID allocate() {
    NodeID out = m_id_datas_.front().begin()++;
    if (m_id_datas_.front().is_empty()) {
      m_id_datas_.pop_front();
    }
    return out;
  }

  /// 申请节点（时间复杂度O(n)）
  /// \param out
  /// \param count
  void allocate(std::vector<NodeID> &out, size_t count) {
    if (!out.empty()) {
      out.clear();
    }
    out.reserve(count);
    auto itBeg = m_id_datas_.before_begin();
    while (itBeg != m_id_datas_.end()) {
      do {
        out.emplace_back(itBeg->begin()++);
        --count;
        if (count == 0) {
          return;
        }
      } while (itBeg->begin() != itBeg->end());
      m_id_datas_.erase_after(itBeg++);
    }
    throw std::overflow_error("Error:空闲节点数量为空，请扩充节点！");
  }

  /// 释放节点id（时间复杂度 O(n)）
  /// \param in
  void release(const NodeID &in) {

    //判断释放节点是否在最前面
    auto itBeg = m_id_datas_.begin();
    NodeRange installRange(in, in + 1);
    if (in < itBeg->begin()) {
      if (installRange.end() == itBeg->begin()) {
        --itBeg->begin();
      } else {
        m_id_datas_.emplace_front(installRange);
      }
    } else {
      auto itBfBeg = m_id_datas_.before_begin();

      //查找第一个begin_大于in的位置
      for (; itBeg != m_id_datas_.end(); ++itBeg, ++itBfBeg) {
        if (itBeg->begin() > in) {
          //in在上一个范围中
          ////抛出异常
          //in在上一个范围结束和这个范围起始的位置
          ////
          if (itBfBeg->contains(in)) {
            throw std::runtime_error("Error:节点释放异常，");
          }
          if (itBfBeg->end() == installRange.begin()) {
            if (installRange.end() == itBeg->begin()) {
              itBfBeg->end() = itBeg->end();
              m_id_datas_.erase_after(itBfBeg);
            } else {
              itBfBeg->end() = installRange.end();
            }
          } else if (installRange.end() == itBeg->begin()) {
            itBeg->begin() = installRange.begin();
          } else {
            auto newIter = m_id_datas_.insert_after(itBfBeg, installRange);
          }
        }
      }

      //假如释放位置未找到，说明释放位置为最后一个范围后
      if (itBfBeg->end() == in) {
        ++itBfBeg->end();
      } else {
        m_id_datas_.insert_after(itBfBeg, {in, in + 1});
      }
    }
  }

  /// 释放节点（时间复杂度 O(n^2)）
  /// \param in
  void release(const std::vector<NodeID> &in) {
    if (in.empty()) {
      return;
    }
    for (const auto item: in) {
      release(item);
    }
  }

private:
  void read_null_id(const std::string &path) {
    std::ifstream file;
    NodeRange tmp;
    file.open(path, std::ios::binary | std::ios::in);
    if (file.is_open()) {
      while (file.read(reinterpret_cast<char *>(&tmp), sizeof(NodeRange))) {
        m_id_datas_.emplace_front(tmp);
      }
      file.close();
    }
  }
  void write_null_id(const std::string &path) {
    std::ofstream file(path, std::ios::binary | std::ios::out);
    if (file.is_open()) {
      for (const auto &id: m_id_datas_) {
        file.write(reinterpret_cast<const char *>(&id), sizeof(NodeRange));
      }
      file.close();
    }
  }
};//! class NodeIDAllocator

//============================================================================================================


//============================================================================================================

//节点管理
//申请节点，查找节点，删除节点，修改节点
class NodeManageSystem {
  static const ::size_t COUNT_ID_MAX_NUM = 0x1000000000000;
  NodeIDAllocator m_id_alloc_;

  std::map<NodeType, NodeList> m_const_node_;
  std::map<NodeID, Node> m_dynamics_nodes_;
  std::map<NodeID, Node> m_tmp_static_nodes_;

public:
  NodeManageSystem() : m_id_alloc_(COUNT_ID_MAX_NUM) {
    std::string path = "CONST_ID_LIST.dat";
    if (fs::exists(path)) {
      read_const_id_list(path);
    }
    for (auto &item: m_const_node_) {
      for (auto &cNode: item.second) {
        read_node(cNode);
      }
    }
  }

  ~NodeManageSystem() {
    std::string path = "CONST_ID_LIST.dat";
    write_const_id_list(path);
  }

  Node &createDynamicNode() {
    NodeID id                    = m_id_alloc_.allocate();
    return m_dynamics_nodes_[id] = {NodeType::DYNAMIC, id};
  }

  void dynamicCastStatic();

  NodeList &createConstType(NodeType type, size_t num) {
    auto res = m_const_node_.try_emplace(type);
    if (res.second) {
      auto &nodeList = res.first->second;
      nodeList.reserve(num);
      std::vector<NodeID> new_id_list;
      m_id_alloc_.allocate(new_id_list, num);
      for (const auto &id: new_id_list) {
        nodeList.emplace_back(type, id);
      }
      return nodeList;
    } else {
      throw ;
    }
  }

  void removeNotConstNode(NodeType type, NodeID &id);

  void removeConstType(NodeType type);

  const Node &getNotConstNode(NodeType type, NodeID &id);

  const NodeList &getConstNode(NodeType type);

  void setNotConstNode(NodeID &id, const Node &newNode);

  void setConstNode(NodeType type, const NodeList &newNodes);

private:
  void read_const_id_list(const std::string &path);
  void write_const_id_list(const std::string &path);

  bool read_node(Node &node);
  bool write_node(const Node &node);
};

}// namespace think

#endif//DATA_MANAGE_H_
