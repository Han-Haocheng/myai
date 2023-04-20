//
// Created by HanHaocheng on 2023/04/19.
//

#ifndef THINKSYSTEM_SYSTEM_MANAGE_CONST_H
#define THINKSYSTEM_SYSTEM_MANAGE_CONST_H
#include "../system_manage.h"

namespace think {

///常态节点管理系统
class ConstManageSystem final : public NodeManageSystem {
  std::unordered_map<NodeType, std::shared_ptr<std::vector<NodeID>>> m_type_id_;

public:
  //c++代码优化,read_const_id()函数是用来获取类型为std::unordered_map<NodeType, std::shared_ptr<std::vector<NodeID>>>的m_type_id_
  //read_node()函数是用来获取类型为std::unordered_map<NodeID, std::shared_ptr<LinkListArray>>的m_node_data_中的std::shared_ptr<LinkListArray>参数
  explicit ConstManageSystem(NodeIDAllocator &alloc) : NodeManageSystem(alloc) {
    if (read_const_id()) {
      init_node_data();
    }
  }

  ~ConstManageSystem() final {
    write_const_id();
    save_node();
  }

  std::shared_ptr<LinkListArray> getLinkListArray(NodeID id) final {
    auto res = m_node_data_.find(id);
    if (res != m_node_data_.end()) {
      return res->second;
    } else {
      return nullptr;
    }
  }

  bool setNode(NodeID id, const std::shared_ptr<LinkListArray> &links) final {
    auto res = m_node_data_.find(id);
    if (res == m_node_data_.end()) {
      return false;
    }
    res->second = links;
    return true;
  }

  NodeMapPair create(NodeType type) final {
    auto id  = m_id_alloc_.allocate();
    auto res = m_node_data_.try_emplace(id, std::make_unique<LinkListArray>(type));
    return *res.first;
  }

  bool remove(NodeID id) final {
    return false;
  }

private:
  void init_node_data() {
    for (const auto &item: m_type_id_) {
      for (const auto &id: *item.second) {
        auto nodeRes = read_node(id);
        if (nodeRes->m_node_type_ == NodeType::NULL_TYPE) {
          m_node_data_[id] = std::make_unique<LinkListArray>(item.first);
        } else if (nodeRes->m_node_type_ == NodeType::STATIC || nodeRes->m_node_type_ == NodeType::DYNAMIC) {
          std::cerr << "节点" << static_cast<NodeID::id_type>(id) << "读取错误，此节点为非常态节点。" << std::endl;
        } else {
          m_node_data_[id] = nodeRes;
        }
      }
    }
  }

  constexpr static const char *STRING_CONST_ID_FOLDER = "./const_id/";

  bool read_const_id() {
    if (!fs::exists(STRING_CONST_ID_FOLDER)) {
      return false;
    }

    // 判断
    std::ifstream idFile;
    size_t tmpSize;
    idFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    for (auto &[constType, fileName]: MAP_CONST_TYPE_TO_STRING) {
      idFile.seekg(0, std::ios::beg);
      idFile.open(STRING_CONST_ID_FOLDER + fileName, std::ios::binary | std::ios::in);
      try {
        idFile.read(reinterpret_cast<char *>(&tmpSize), sizeof(size_t));
        auto mapRes = m_type_id_.emplace_hint(m_type_id_.end(), constType, std::make_unique<std::vector<NodeID>>(tmpSize));
        idFile.read(reinterpret_cast<char *>(mapRes->second->data()), static_cast<std::streamsize>(sizeof(NodeID) * tmpSize));
      } catch (const std::exception &e) {
        std::cerr << "Failed to read " << fileName << " due to " << e.what() << std::endl;
      }
    }
    idFile.close();

    return true;
  }


  bool write_const_id() {
    if (!fs::create_directories(STRING_CONST_ID_FOLDER)) {
      return false;
    }

    // 写入
    for (auto &[constType, fileName]: MAP_CONST_TYPE_TO_STRING) {
      auto mapIter = m_type_id_.find(constType);
      if (mapIter == m_type_id_.end() || mapIter->second->empty()) {
        std::cerr << "Empty " << fileName << " vector, skipping write\n";
        continue;
      }
      try {
        std::ofstream idFile(STRING_CONST_ID_FOLDER + fileName, std::ios::binary);
        const size_t tmpSize = mapIter->second->size();
        idFile.write(reinterpret_cast<const char *>(&tmpSize), sizeof(size_t));
        idFile.write(reinterpret_cast<const char *>(mapIter->second->data()), static_cast<std::streamsize>(sizeof(NodeID) * mapIter->second->size()));
      } catch (const std::exception &e) {
        std::cerr << "Failed to write " << fileName << " due to " << e.what() << std::endl;
      }
    }
    return true;
  }
};//! class ConstManageSystem;

}// namespace think

#endif//THINKSYSTEM_SYSTEM_MANAGE_CONST_H
