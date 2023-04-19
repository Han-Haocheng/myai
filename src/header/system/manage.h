//
// Created by HanHaocheng on 2023/03/31.
//

#ifndef DATA_MANAGE_H_
#define DATA_MANAGE_H_

#include "../type/type_node.h"
#include "allocator.h"

#include <memory>
#include <stdexcept>

#include <map>

#include <filesystem>
#include <fstream>
#include <iostream>
namespace fs = std::filesystem;

namespace think {

constexpr const static char *STRING_NODE_FILE = "node.dat";

enum class NMSType {
  CONST_NMS,
  STATIC_NMS,
  DYNAMIC_NMS
};

//============================================================================================================
using NodeMap     = std::unordered_map<NodeID, std::shared_ptr<LinkListArray>>;
using NodeMapPair = std::pair<NodeID, std::shared_ptr<LinkListArray>>;

//节点管理
//申请节点，查找节点，删除节点，修改节点
class NodeManageSystem {
protected:
  NodeMap m_id_map_;
  NodeIDAllocator &m_id_alloc_;

public:
  explicit NodeManageSystem(NodeIDAllocator &idAlloc) : m_id_alloc_(idAlloc) {}
  std::unique_ptr<NodeMap> getNodeMap() const {
    return std::make_unique<NodeMap>(m_id_map_);
  }

  virtual ~NodeManageSystem()                                                  = 0;
  virtual NodeMapPair create(NodeType)                                         = 0;
  virtual bool remove(const NodeID &id)                                        = 0;
  virtual std::shared_ptr<LinkListArray> getNodeLinks(const NodeID &id)        = 0;
  virtual bool setNode(const NodeID &, const std::shared_ptr<LinkListArray> &) = 0;

  void saveDynamicLinks(double saveWeight) {
    (void) std::for_each(m_id_map_.begin(),
                         m_id_map_.end(),
                         [&](const std::pair<const NodeID, std::shared_ptr<LinkListArray>> &item) -> void {
                           item.second->saveDynamicData(saveWeight);
                         });
  }

protected:
  struct FileInfo {
    NodeType nType;
    size_t const_count;
    size_t const_value;
    size_t static_count;
    size_t static_value;
  };

  static std::shared_ptr<LinkListArray> read_node(const NodeID &id) {
    std::string path = id.id_path();
    if (!fs::exists(path + STRING_NODE_FILE)) {
      return nullptr;
    }
    std::ifstream file{path + STRING_NODE_FILE, std::ios ::binary | std::ios ::in};
    if (!file.is_open()) {
      return nullptr;
    }
    std::unique_ptr<LinkListArray> res = std::make_unique<LinkListArray>();
    FileInfo tmpInfo{};
    file.read(reinterpret_cast<char *>(&tmpInfo), sizeof(FileInfo));
    res->m_const_link_.link_val  = tmpInfo.const_value;
    res->m_static_link_.link_val = tmpInfo.static_value;
    res->m_const_link_.link.resize(tmpInfo.const_count);
    res->m_static_link_.link.resize(tmpInfo.static_count);
    file.read(reinterpret_cast<char *>(res->m_const_link_.link.data()),
              static_cast<std::streamsize>(sizeof(LinkInfo) * tmpInfo.const_count));
    file.read(reinterpret_cast<char *>(res->m_static_link_.link.data()),
              static_cast<std::streamsize>(sizeof(LinkInfo) * tmpInfo.static_count));
    file.close();
    return res;
  }

  static bool write_node(const NodeID &id, const std::shared_ptr<LinkListArray> &links) {
    std::string path = id.id_path();
    if (!fs::exists(path)) {
      fs::create_directories(path);
    }
    std::ofstream file{path + STRING_NODE_FILE, std::ios ::binary | std::ios ::out};
    if (file.is_open()) {
      return false;
    }

    FileInfo tmpInfo{
            links->m_node_type_,
            links->m_const_link_.link.size(),
            links->m_const_link_.link_val,
            links->m_static_link_.link.size(),
            links->m_static_link_.link_val,
    };

    file.write(reinterpret_cast<const char *>(&tmpInfo), sizeof(FileInfo));
    file.write(reinterpret_cast<const char *>(links->m_const_link_.link.data()),
               static_cast<std::streamsize>(sizeof(LinkInfo) * tmpInfo.const_count));
    file.write(reinterpret_cast<const char *>(links->m_static_link_.link.data()),
               static_cast<std::streamsize>(sizeof(LinkInfo) * tmpInfo.static_count));
    file.close();
    return true;
  }

  void save_all_node() {
    if (m_id_map_.empty()) {
      return;
    }
    for (const auto &item: m_id_map_) {
      write_node(item.first, item.second);
    }
  }
};//! virtual class NodeManageSystem

class DynamicManageSystem final : public NodeManageSystem {
  friend class StaticManageSystem;

public:
  explicit DynamicManageSystem(NodeIDAllocator &idAlloc) : NodeManageSystem(idAlloc) {}
  ~DynamicManageSystem() final {
    m_id_map_.clear();
  }
  NodeMapPair create(NodeType type) final {
    auto id  = m_id_alloc_.allocate();
    auto res = m_id_map_.try_emplace(id, std::make_unique<LinkListArray>(NodeType::DYNAMIC));
    return *res.first;
  }
  bool remove(const NodeID &id) final {
    if (m_id_map_.erase(id) != 0) {
      m_id_alloc_.release(id);
      return true;
    }
    return false;
  }

  std::shared_ptr<LinkListArray> getNodeLinks(const NodeID &id) final {
    auto res = m_id_map_.find(id);
    if (res != m_id_map_.end()) {
      return res->second;
    } else {
      return nullptr;
    }
  }

  bool setNode(const NodeID &id, const std::shared_ptr<LinkListArray> &links) final {
    auto res = m_id_map_.find(id);
    if (res == m_id_map_.end()) {
      return false;
    }
    res->second = links;
    return true;
  }
};//! class DynamicManageSystem

///静态节点管理系统
class StaticManageSystem final : public NodeManageSystem {
public:
  explicit StaticManageSystem(NodeIDAllocator &idAlloc) : NodeManageSystem(idAlloc) {}
  ~StaticManageSystem() final {
    save_all_node();
    m_id_map_.clear();
  }

  NodeMapPair create(NodeType type) final {
    auto id = m_id_alloc_.allocate();
    return {m_id_map_.try_emplace(id, std::make_unique<LinkListArray>(NodeType::STATIC)).first->first,
            m_id_map_[id]};
  }

  bool remove(const NodeID &id) final {
    if (m_id_map_.erase(id) != 0) {
      m_id_alloc_.release(id);
      return true;
    }
    std::string path = id.id_path();
    if (fs::exists(path + "node.dat")) {
      fs::remove(path);
      m_id_alloc_.release(id);
      return true;
    } else {
      return false;
    }
  }

  void saveDynamic(DynamicManageSystem &in, double saveWeight) {
    in.saveDynamicLinks(saveWeight);
    this->saveDynamicLinks(saveWeight);
    this->m_id_map_.insert(in.m_id_map_.begin(), in.m_id_map_.end());
  }

  std::shared_ptr<LinkListArray> getNodeLinks(const NodeID &id) final {
    auto res = m_id_map_.find(id);
    if (res != m_id_map_.end()) {
      return res->second;
    }
    auto type = read_node(id);
    if (type->m_node_type_ == NodeType::STATIC) {
      return m_id_map_[id] = type;
    } else if (type->m_node_type_ == NodeType::NULL_TYPE) {
      return nullptr;
    } else {
      throw std::logic_error("流程错误，输入id是非静态节点");
    }
  }

  bool setNode(const NodeID &id, const std::shared_ptr<LinkListArray> &links) final {
    auto res = m_id_map_.find(id);
    if (res == m_id_map_.end()) {
      return false;
    }
    res->second = links;
    return true;
  }
};//! class StaticManageSystem;

///常态节点管理系统
class ConstManageSystem final : public NodeManageSystem {
  std::unordered_map<NodeType, std::vector<NodeMapPair>> m_type_map_;

public:
  explicit ConstManageSystem(NodeIDAllocator &alloc) : NodeManageSystem(alloc) {
    init_node_data();
  }

  ~ConstManageSystem() final {
    write_const_id();
    save_all_node();
  }

  std::shared_ptr<LinkListArray> getNodeLinks(const NodeID &id) final {
    auto res = m_id_map_.find(id);
    if (res != m_id_map_.end()) {
      return res->second;
    } else {
      return nullptr;
    }
  }

  std::vector<NodeMapPair> *getNodeLinks(NodeType type) {
    auto res = m_type_map_.find(type);
    if (res != m_type_map_.end()) {
      return &res->second;
    } else {
      return nullptr;
    }
  }

  bool setNode(const NodeID &id, const std::shared_ptr<LinkListArray> &links) final {
    auto res = m_id_map_.find(id);
    if (res == m_id_map_.end()) {
      return false;
    }
    res->second = links;
    return true;
  }

  NodeMapPair create(NodeType type) final {
    auto id  = m_id_alloc_.allocate();
    auto res = m_id_map_.try_emplace(id, std::make_unique<LinkListArray>(type));
    return *res.first;
  }

  bool remove(const NodeID &id) final {
    return false;
  }

private:
  bool init_node_data() {
    read_const_id();
    if (!fs::exists(STRING_CONST_ID_FOLDER)) {
      return false;
    }
    std::ifstream idFile;
    size_t tmpSize;
    idFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    for (const auto &[type, fileName]: MAP_CONST_TYPE_TO_STRING) {
      try {
        idFile.open(STRING_CONST_ID_FOLDER + fileName, std::ios::binary | std::ios::in);
        idFile.read(reinterpret_cast<char *>(&tmpSize), sizeof(size_t));
        auto &nodeVec = m_type_map_.at(type);
        for (size_t i = 0; i < tmpSize; ++i) {
          NodeID id;
          idFile.read(reinterpret_cast<char *>(&id), sizeof(NodeID));
          auto iter = m_id_map_.find(id);
          if (iter != m_id_map_.end()) {
            nodeVec.emplace_back(id, iter->second);
          } else {
            auto readRes = read_node(id);
            if (readRes == nullptr || readRes->m_node_type_ <= NodeType::DYNAMIC) {
              std::cerr << "节点" << static_cast<NodeID::id_type>(id) << "读取错误，此节点为非常态节点。" << std::endl;
              throw std::runtime_error("read node error");
            }
            nodeVec.emplace_back(id, readRes);
            m_id_map_[id] = readRes;
          }
        }
        idFile.close();
      } catch (const std::ifstream::failure &e) {
        std::cerr << "读取节点数据失败：" << e.what() << std::endl;
        return false;
      }
    }

    return true;
  }

  constexpr static const char *STRING_CONST_ID_FOLDER = "./const_id/";

  bool read_const_id() {


    // 判断
    std::ifstream idFile;
    size_t tmpSize;
    idFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    for (auto &[constType, fileName]: MAP_CONST_TYPE_TO_STRING) {
      idFile.seekg(0, std::ios::beg);
      idFile.open(STRING_CONST_ID_FOLDER + fileName, std::ios::binary | std::ios::in);
      try {
        idFile.read(reinterpret_cast<char *>(&tmpSize), sizeof(size_t));
        auto mapRes =
                m_type_map_.emplace_hint(m_type_map_.end(), constType, std::vector<NodeMapPair>(tmpSize));
        for (auto &[id, info]: mapRes->second) {
        }
        idFile.read(reinterpret_cast<char *>(mapRes->second.data()), static_cast<std::streamsize>(sizeof(NodeID) * tmpSize));
      } catch (const std::exception &e) {
        std::cerr << "Failed to read " << fileName << " due to " << e.what() << std::endl;
      }
    }
    idFile.close();

    return true;
  }


  bool write_const_id() {
    if (!fs::exists(STRING_CONST_ID_FOLDER)) {
      fs::create_directories(STRING_CONST_ID_FOLDER);
    }

    // 写入
    for (auto &[constType, fileName]: MAP_CONST_TYPE_TO_STRING) {
      auto mapIter = m_type_map_.find(constType);
      if (mapIter == m_type_map_.end() || mapIter->second.empty()) {
        std::cerr << "Empty " << fileName << " vector, skipping write\n";
        continue;
      }
      try {
        std::ofstream idFile(STRING_CONST_ID_FOLDER + fileName, std::ios::binary);
        const size_t tmpSize = mapIter->second.size();
        idFile.write(reinterpret_cast<const char *>(&tmpSize), sizeof(size_t));
        for (const auto &[id, info]: mapIter->second) {
          idFile.write(reinterpret_cast<const char *>(&id), sizeof(NodeID));
        }
      } catch (const std::exception &e) {
        std::cerr << "Failed to write " << fileName << " due to " << e.what() << std::endl;
      }
    }
    return true;
  }
};//! class ConstManageSystem;


}// namespace think

#endif//DATA_MANAGE_H_
