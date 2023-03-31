//
// Created by HanHaocheng on 2023/03/31.
//

#ifndef DATA_MANAGE_H_
#define DATA_MANAGE_H_
#include "data_type.h"
#include "type_range.h"
#include <forward_list>
#include <list>
#include <stdexcept>

#include <filesystem>
#include <fstream>
#include <map>

namespace fs = std::filesystem;

namespace think {
using NodeRange = Range<NodeID>;

class NodeIDAllocator {


private:
  std::forward_list<NodeRange> m_id_datas_;

public:
  explicit NodeIDAllocator(size_t nodeCount) {
    std::string path = "node_atto.dat";
    NodeRange tmp{NodeID{0}, NodeID{nodeCount}};
    if (!fs::exists(path)) {
      std::ofstream file;
      m_id_datas_.emplace_front(tmp);
      file.open(path, std::ios::binary | std::ios::out);
      if (file.is_open()) {
        file.write(reinterpret_cast<const char *>(&tmp), sizeof(NodeRange));
        file.close();
      }
    } else {
      std::ifstream file;
      file.open(path, std::ios::binary | std::ios::in);
      if (file.is_open()) {
        while (file.read(reinterpret_cast<char *>(&tmp), sizeof(NodeRange))) {
          m_id_datas_.emplace_front(tmp);
        }
        file.close();
      }
    }
  }

  ~NodeIDAllocator() {
    std::string path = "node_atto.dat";
    std::ofstream file(path, std::ios::binary | std::ios::out);
    if (file.is_open()) {
      for (const auto &id: m_id_datas_) {
        file.write(reinterpret_cast<const char *>(&id), sizeof(NodeRange));
      }
      file.close();
    }
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
      return;
    }

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
        } else {
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
          return;
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
};//! class NodeIDAllocator

//============================================================================================================

#pragma pack(push, 2)


class Node {
  friend class NodeManageSystem;

private:
  NodeID m_id_;
  NodeInfo m_info_{};
  std::shared_ptr<NodeLinks> m_links_;
  bool m_is_read_;

public:
  Node(const NodeID &id, const NodeInfo &info) : m_id_(id), m_info_(info) {}

  [[nodiscard]] const NodeID &getId() const { return m_id_; }
  [[nodiscard]] const NodeInfo &getInfo() const { return m_info_; }
  [[nodiscard]] const std::shared_ptr<NodeLinks> &getLinks() const { return m_links_; }

  void emplace(NodeType type, const EleInfo &newLink) {
    switch (type) {
      case NodeTypeEnum::STATIC:
        m_links_->static_links.emplace_back(newLink);
        m_info_.static_value += newLink.link_val;
        break;
      case NodeTypeEnum::DYNAMIC:
        m_links_->dynamic_links.emplace_back(newLink);
        m_info_.dynamic_value += newLink.link_val;
        break;
      default:
        break;
    }
  }
};
#pragma pack(pop)

using NodePtr  = Node *;
using NodeList = std::vector<Node>;

//============================================================================================================

//节点管理
//申请节点，查找节点，删除节点，修改节点
class NodeManageSystem {
  NodeIDAllocator m_id_alloc_;

  std::map<NodeType, NodeList> m_const_node_;
  std::map<NodeID, Node> m_dynamics_nodes_;
  std::map<NodeID, Node> m_tmp_static_nodes_;

public:
  Node *applyNode(const NodeInfo &initInfo) {
    if (initInfo.node_type == NodeTypeEnum::STATIC) {
      NodeID newId = m_id_alloc_.allocate();
      return &m_tmp_static_nodes_.emplace(std::pair<NodeID, Node>{newId, {newId, initInfo}}).first->second;
    } else if (initInfo.node_type == NodeTypeEnum::STATIC) {
      NodeID newId = m_id_alloc_.allocate();
      return &m_tmp_static_nodes_.emplace(std::pair<NodeID, Node>{newId, {newId, initInfo}}).first->second;
    } else {
      throw std::runtime_error(std::string("local:class_") + typeid(decltype(*this)).name() + " line_" + std::to_string(__LINE__) + ("Error:节点申请失败，此节点为静态节点，请用另一种申请方式"));
    }
  }

  NodeList *applyNode(const NodeInfo &initInfo, size_t typeSize) {
    if (initInfo.node_type == NodeTypeEnum::STATIC || initInfo.node_type == NodeTypeEnum::DYNAMIC) {
      throw std::runtime_error(std::string("local:class_") + typeid(decltype(*this)).name() + " line_" + std::to_string(__LINE__) + ("Error:节点申请失败，此节点为静态节点，请用另一种申请方式"));
    } else {
      std::vector<NodeID> ids;
      std::vector<Node> tmpNodes;
      tmpNodes.reserve(typeSize);
      m_id_alloc_.allocate(ids, typeSize);
      for (const auto &id: ids) {
        tmpNodes.emplace_back(Node{id, initInfo});
      }
      return &m_const_node_.emplace(std::pair<NodeType, NodeList>{initInfo.node_type, tmpNodes}).first->second;
    }
  }


  Node &createNode(const NodeInfo &initInfo, const std::shared_ptr<NodeLinks> &initLinks = nullptr) {
    NodeID newId = m_id_alloc_.allocate();
    Node ret{newId, initInfo};
    if (initInfo.node_type == NodeTypeEnum::STATIC) {
      auto res = m_tmp_static_nodes_.emplace(std::pair<NodeID, Node>{newId, ret});
      ret      = res.first->second;
    } else if (initInfo.node_type == NodeTypeEnum::DYNAMIC) {
      auto res = m_dynamics_nodes_.emplace(std::pair<NodeID, Node>{newId, ret});
      ret      = res.first->second;
    } else {
      m_const_node_.emplace(std::pair<NodeType, NodeList>{initInfo.node_type, {}});
    }

    std::string path = newId.id_path() + "node.dat";
    if (!fs::exists(path)) {
      fs::create_directory(path);
    }
    std::ofstream outFile(path, std::ios::out | std::ios::binary);
    if (outFile.is_open()) {
      outFile.write(reinterpret_cast<const char *>(&initInfo), sizeof(NodeInfo));
      outFile.close();
      return true;
    }
    return false;
  }

  [[nodiscard]] bool deleteNode(NodeIDAllocator &nodeAlloca) const {
    std::string path = m_id_.id_path();
    if (fs::exists(path)) {
      fs::remove_all(path);
      return true;
    }
    return false;
  }

  bool saveNode() {
    std::string path = m_id_.id_path() + "node.dat";
    if (!fs::exists(path)) {
      fs::create_directory(path);
    }
    std::ofstream outFile;
    outFile.open(path, std::ios::out | std::ios::binary);
    if (outFile.is_open()) {
      outFile.write(reinterpret_cast<const char *>(&m_info_), sizeof(NodeInfo));
      outFile.write(reinterpret_cast<const char *>(m_links_->const_links.data()),
                    static_cast<std::streamsize>(m_links_->const_links.size() * sizeof(EleInfo)));
      outFile.write(reinterpret_cast<const char *>(m_links_->static_links.data()),
                    static_cast<std::streamsize>(m_links_->static_links.size() * sizeof(EleInfo)));
      outFile.write(reinterpret_cast<const char *>(m_links_->dynamic_links.data()),
                    static_cast<std::streamsize>(m_links_->dynamic_links.size() * sizeof(EleInfo)));
      outFile.close();
      return true;
    }
    return false;
  }

  bool readNode() {
    if (m_is_read_) {
      return false;
    }
    std::string path = m_id_.id_path() + "node.dat";
    if (!fs::exists(path)) {
      fs::create_directory(path);
    }
    std::ifstream outFile;
    outFile.open(path, std::ios::in | std::ios::binary);
    if (outFile.is_open()) {
      size_t constCount, staticCount, dynamicCount;
      outFile.read(reinterpret_cast<char *>(&constCount), sizeof(size_t));
      outFile.read(reinterpret_cast<char *>(&staticCount), sizeof(size_t));
      outFile.read(reinterpret_cast<char *>(&dynamicCount), sizeof(size_t));
      m_links_->const_links.reserve(constCount);
      m_links_->static_links.reserve(staticCount);
      m_links_->dynamic_links.reserve(dynamicCount);

      outFile.read(reinterpret_cast<char *>(m_links_->const_links.data()),
                   static_cast<std::streamsize>(constCount * sizeof(EleInfo)));
      outFile.read(reinterpret_cast<char *>(m_links_->static_links.data()),
                   static_cast<std::streamsize>(staticCount * sizeof(EleInfo)));
      outFile.read(reinterpret_cast<char *>(m_links_->dynamic_links.data()),
                   static_cast<std::streamsize>(dynamicCount * sizeof(EleInfo)));
      outFile.close();
      m_is_read_ = true;
      return true;
    }
    throw std::runtime_error(std::string("local:class_") + typeid(decltype(*this)).name() + " line_" + std::to_string(__LINE__) + ("Error:节点读取文件时失败，文件未成功打开!!"));
  }
};

}// namespace think

#endif//DATA_MANAGE_H_
