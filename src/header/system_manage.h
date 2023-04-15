//
// Created by HanHaocheng on 2023/03/31.
//

#ifndef DATA_MANAGE_H_
#define DATA_MANAGE_H_
#include "type/type_range.h"
#include "type_node.h"
#include <forward_list>
#include <list>
#include <memory>
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

//节点管理
//申请节点，查找节点，删除节点，修改节点
class NodeManageSystem {
protected:
  NodeIDAllocator *m_id_alloc_ = nullptr;
  std::unordered_map<NodeID, NodePtr> m_node_data_;

public:
  static const ::size_t COUNT_ID_MAX_NUM = 0x1000000000000;

public:
  virtual NodePtr createNode(NodeType type) {
    NodeID id               = m_id_alloc_->allocate();
    return m_node_data_[id] = std::make_unique<Node>(type, id);
  }

  virtual void createType(NodePtrList &out, NodeType type, size_t num) {
    if (!out.empty()) { out.resize(0); }
    out.reserve(num);
    std::vector<NodeID> tmpNodeList;
    m_id_alloc_->allocate(tmpNodeList, num);
    for (const auto &item: tmpNodeList) {
      out.emplace_back(m_node_data_[item] = std::make_unique<Node>(type, item));
    }
  }

  virtual void remove(const NodeID &id) {
    m_id_alloc_->release(id);
    m_node_data_.erase(id);
  }

  virtual void remove(const std::vector<NodeID> &ids) {
    for (const auto &id: ids) {
      m_node_data_.erase(id);
    }
    m_id_alloc_->release(ids);
  }

  virtual NodePtr getNode(NodeType type, NodeID &id) {
  }
  virtual const NodePtrList &getNode(NodeType type)                = 0;
  virtual void setNode(NodeID &id, Node *newNode)                  = 0;
  virtual void setNode(NodeType type, const NodePtrList &newNodes) = 0;
  virtual void saveData()                                          = 0;

protected:
  struct nodeFileData {
    Node::LinkList const_link;
    Node::LinkList static_link;
  };
  static bool read_node(const NodeID &id);
  //{
  //    if (node->isNull()) {
  //      throw std::logic_error("读取节点为空位置");
  //    }
  //
  //    std::string path = node->m_id_.id_path();
  //    if (!fs::exists(path)) {
  //      return false;
  //    }
  //
  //    std::ifstream file(path, std::ios::binary | std::ios::in);
  //    if (file.is_open()) {
  //      Node::Info tmpInfo;
  //      file.read(reinterpret_cast<char *>(&tmpInfo), sizeof(Node::Info));
  //      node->node_type = tmpInfo.type;
  //
  //      // node->m_links_.static_link.link_val = tmpInfo.static_link_val;
  //      // node->m_links_.const_link.link_val  = tmpInfo.const_link_val;
  //      // decltype(node->m_links_.const_link.link) tmpLink(tmpInfo.const_link_count + tmpInfo.static_link_count);
  //      // file.read(reinterpret_cast<char *>(tmpLink.data()), tmpLink.size() * sizeof(LinkInfo));
  //      // node->m_links_.const_link.link  = {tmpLink.begin(), tmpLink.begin() + tmpInfo.const_link_count};
  //      // node->m_links_.static_link.link = {tmpLink.begin() + tmpInfo.const_link_count, tmpLink.end()};
  //
  //      node->m_links_.static_link.link_val = tmpInfo.static_link_val;
  //      node->m_links_.const_link.link_val  = tmpInfo.const_link_val;
  //      node->m_links_.static_link.link.reserve(tmpInfo.static_link_count);
  //      node->m_links_.const_link.link.reserve(tmpInfo.const_link_count);
  //      LinkInfo tmpLI;
  //      for (int i = 0; i < tmpInfo.static_link_count; ++i) {
  //        file.read(reinterpret_cast<char *>(&tmpLI), sizeof(LinkInfo));
  //        node->m_links_.static_link.link.emplace_back(tmpLI);
  //      }
  //      for (int i = 0; i < tmpInfo.const_link_count; ++i) {
  //        file.read(reinterpret_cast<char *>(&tmpLI), sizeof(LinkInfo));
  //        node->m_links_.const_link.link.emplace_back(tmpLI);
  //      }
  //      file.close();
  //      return true;
  //    }
  //    return false;
  //  }

  static bool write_node(const Node *const node);
  //  {
  //    if (node->isNull()) {
  //      throw std::logic_error("读取节点为空位置");
  //    }
  //    std::string path = node->m_id_.id_path();
  //    if (!fs::exists(path)) {
  //      fs::create_directories(path);
  //    }
  //    std::ofstream file(path, std::ios::binary | std::ios::out);
  //    if (file.is_open()) {
  //      Node::Info tmpInfo{node->node_type,
  //                         node->m_links_.const_link.link.size(),
  //                         node->m_links_.static_link.link.size(),
  //                         node->m_links_.const_link.link_val,
  //                         node->m_links_.static_link.link_val};
  //      file.write(reinterpret_cast<const char *>(&tmpInfo), sizeof(Node::Info));
  //
  //      file.write(reinterpret_cast<const char *>(node->m_links_.const_link.link.data()),
  //                 static_cast<std::streamsize>(sizeof(LinkInfo) * tmpInfo.const_link_count));
  //      file.write(reinterpret_cast<const char *>(node->m_links_.static_link.link.data()),
  //                 static_cast<std::streamsize>(sizeof(LinkInfo) * tmpInfo.static_link_count));
  //
  //      file.close();
  //      return true;
  //    }
  //    return false;
  //  }
};//! virtual class NodeManageSystem


class DynamicManageSystem : public NodeManageSystem {
  friend class StaticManageSystem;
  std::map<NodeID, NodePtr> m_dynamics_nodes_;

public:
};//! class DynamicManageSystem

class StaticManageSystem : public NodeManageSystem {

};//! class DynamicManageSystem;


class ConstManageSystem : public NodeManageSystem {

};//! class ConstManageSystem;


}// namespace think

#endif//DATA_MANAGE_H_
