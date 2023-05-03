//
// Created by HanHaocheng on 2023/04/27.
//

#ifndef THINKSYSTEM_3_THINK_MANAGE_H
#define THINKSYSTEM_3_THINK_MANAGE_H
#include "0_include.h"
#include "1_think_enum.h"
#include "2_think_idalloc.h"
#include <iostream>


namespace think {
constexpr static const char *STR_CONST_ID_FOLDER = "./const_id/";

class CNodeManage {
private:
  CNodeIDAllocator m_cIDAlloc_;
  std::unordered_map<NODE_ID, SP_NODE_INFO> m_cMapNodeBuffer_;
  std::unordered_map<CONSTTYPE, std::vector<std::pair<NODE_ID, SP_NODE_INFO>>> m_cMapConstNode_;


public:
  CNodeManage() : m_cIDAlloc_(NUM_NODE_ID_MAX) {
    _readConstNodes();
  }

  ~CNodeManage() {
    _saveConstNodes();
  }

  size_t getBufferSize() { return m_cMapNodeBuffer_.size(); }

  void saveAllNode(WEIGHT dyWeakWeight, LINK_VAL saveStand, bool isEnd = false) {
    for (const auto &[id, spLkGp]: m_cMapNodeBuffer_) {
      spLkGp->dyLks *= dyWeakWeight;
      spLkGp->scLks.insert(spLkGp->dyLks);
      spLkGp->dyLks.clear();
      spLkGp->scLks.erase(saveStand);
      _saveNode(id, spLkGp);
    }
    if (!isEnd) {
      m_cMapNodeBuffer_.clear();
      for (const auto &[cstType, nodVec]: m_cMapConstNode_) {
        m_cMapNodeBuffer_ = std::unordered_map<NODE_ID, SP_NODE_INFO>(nodVec.begin(), nodVec.end());
      }
    }
  }


  std::pair<const NODE_ID, SP_NODE_INFO> *emplace() {
    return &*m_cMapNodeBuffer_.emplace(m_cIDAlloc_.allocate(), std::make_unique<SNodeInfo>(SNodeInfo{ENodeType::DYNAMIC_TYPE})).first;
  }

  std::pair<const NODE_ID, SP_NODE_INFO> *emplace(const SP_NODE_INFO &spLG) {
    return &*m_cMapNodeBuffer_.emplace(m_cIDAlloc_.allocate(), spLG).first;
  }

  std::pair<const NODE_ID, SP_NODE_INFO> *emplace(NODE_ID id, const SP_NODE_INFO &spLG) {
    return &*m_cMapNodeBuffer_.emplace(id, spLG).first;
  }

  std::pair<const NODE_ID, SP_NODE_INFO> *emplace(CONSTTYPE eCstType, const SP_NODE_INFO &spLG) {
    return &*m_cMapNodeBuffer_.emplace(m_cMapConstNode_[eCstType].emplace_back(m_cIDAlloc_.allocate(), spLG)).first;
  }

  bool erase(NODE_ID id) {
    auto fRes = m_cMapNodeBuffer_.find(id);
    if (fRes == m_cMapNodeBuffer_.end()) {
      fs::remove_all(GetNodePath(id));
    } else {
      if (!((int) fRes->second->nodetype & ((int) ENodeType::DYNAMIC_TYPE | (int) ENodeType::STATIC_TYPE))) {
        return false;
      }
      m_cMapNodeBuffer_.erase(fRes);
    }
    m_cIDAlloc_.release(id);
    return true;
  }

  std::pair<const NODE_ID, SP_NODE_INFO> *find(NODE_ID id) {
    auto fRes = m_cMapNodeBuffer_.find(id);
    if (fRes != m_cMapNodeBuffer_.end()) {
      return &*fRes;
    }
    SP_NODE_INFO rRes = _readNode(id);
    if (rRes != nullptr) {
      return this->emplace(id, rRes);
    } else {
      return nullptr;
    }
  }

  std::pair<const EConstType, std::vector<std::pair<NODE_ID, SP_NODE_INFO>>> *find(CONSTTYPE eConstType) {
    auto fRes = m_cMapConstNode_.find(eConstType);
    return fRes != m_cMapConstNode_.end() ? &*fRes : nullptr;
  }

private:
#if USE_FLOAT_LINK
  struct FileInfo {
    size_t const_count;
    size_t const_value;
    size_t static_count;
    size_t static_value;
  };
#else
  struct FileInfo {
    size_t const_count;
    size_t static_count;
  };
#endif

  static UP_NODE_INFO _readNode(NODE_ID id) {
    std::string path = GetNodeParentPath(id);
    if (!fs::exists(path + STR_NODE_FILE_NAME)) {
      return nullptr;
    }
    std::ifstream file{path + STR_NODE_FILE_NAME, std::ios ::binary | std::ios ::in};
    if (!file.is_open()) {
      return nullptr;
    }

    UP_NODE_INFO lpLinkArr = std::make_unique<SNodeInfo>();

    FileInfo tmpInfo{};
    file.read(reinterpret_cast<char *>(&tmpInfo), sizeof(FileInfo));
#if USE_FLOAT_LINK
    lpLinkArr->ctLks.all_val = tmpInfo.const_value;
    lpLinkArr->scLks.all_val = tmpInfo.static_value;
#endif
    lpLinkArr->ctLks.data.resize(tmpInfo.const_count);
    lpLinkArr->scLks.data.resize(tmpInfo.static_count);
    file.read(reinterpret_cast<char *>(lpLinkArr->ctLks.data.data()),
              static_cast<std::streamsize>(sizeof(SLink) * tmpInfo.const_count));
    file.read(reinterpret_cast<char *>(lpLinkArr->scLks.data.data()),
              static_cast<std::streamsize>(sizeof(SLink) * tmpInfo.static_count));
    file.close();
    return lpLinkArr;
  }

  static bool _saveNode(NODE_ID id, const SP_NODE_INFO &links) {
    std::string path = GetNodeParentPath(id);
    if (!fs::exists(path)) {
      fs::create_directories(path);
    }

    std::ofstream file{path + STR_NODE_FILE_NAME, std::ios ::binary | std::ios ::out};
    if (file.is_open()) {
      return false;
    }
#if USE_FLOAT_LINK
    FileInfo tmpInfo{
            links->ctLks.data.size(),
            links->ctLks.all_val,
            links->scLks.data.size(),
            links->scLks.all_val,
    };
#else
    FileInfo tmpInfo{
            links->ctLks.data.size(),
            links->scLks.data.size(),
    };
#endif
    file.write(reinterpret_cast<const char *>(&tmpInfo), sizeof(FileInfo));
    file.write(reinterpret_cast<const char *>(links->ctLks.data.data()),
               static_cast<std::streamsize>(sizeof(SLink) * tmpInfo.const_count));
    file.write(reinterpret_cast<const char *>(links->scLks.data.data()),
               static_cast<std::streamsize>(sizeof(SLink) * tmpInfo.static_count));
    file.close();
    return true;
  }


  struct ConstNodeIDHeader {
    CONSTTYPE constType;
    size_t idSize;
  };

  bool _readConstNodes() {
    if (!fs::exists(STR_CONST_ID_FOLDER)) { return false; }
    for (const auto &[eNodeType, szNodeType]: MAP_CONST_TYPE_TO_STRING) {
      std::ifstream idFile{STR_CONST_ID_FOLDER + szNodeType, std::ios::binary | std::ios::in};
      if (!idFile.is_open()) {
        std::cerr << "产生未知错误，静态节点id文件读取时打开失败！" << std::endl;
        continue;
      }

      ConstNodeIDHeader fileHeader{};
      idFile.read(reinterpret_cast<char *>(&fileHeader), sizeof(ConstNodeIDHeader));
      auto vecRes = m_cMapConstNode_.emplace(eNodeType, std::vector<std::pair<NODE_ID, SP_NODE_INFO>>{}).first;
      vecRes->second.reserve(fileHeader.idSize);
      for (size_t i = 0U; i < fileHeader.idSize; ++i) {
        NODE_ID id;

        idFile.read(reinterpret_cast<char *>(&id), sizeof(NODE_ID));
        auto readRes = _readNode(id);
        if (readRes == nullptr) { continue; }
        vecRes->second.emplace_back(*m_cMapNodeBuffer_.emplace(id, std::move(readRes)).first);
      }
      idFile.close();
    }
    return true;
  }

  bool _saveConstNodes() {
    if (!fs::exists(STR_CONST_ID_FOLDER)) { fs::create_directories(STR_CONST_ID_FOLDER); }
    for (auto &[constType, fileName]: MAP_CONST_TYPE_TO_STRING) {
      std::ofstream idFile{STR_CONST_ID_FOLDER + fileName, std::ios::binary | std::ios::out};
      if (!idFile.is_open()) {
        std::cerr << "产生未知错误，静态节点id文件写入时打开失败！" << std::endl;
        continue;
      }

      auto mapIter = m_cMapConstNode_.find(constType);
      ConstNodeIDHeader cnidHeader{constType, mapIter->second.size()};
      idFile.write(reinterpret_cast<const char *>(&cnidHeader.idSize), sizeof(ConstNodeIDHeader));
      for (const auto &[id, spLG]: mapIter->second) {
        idFile.write(reinterpret_cast<const char *>(&id), sizeof(NODE_ID));
      }
      idFile.close();
    }
  };
};
}// namespace think
#endif//THINKSYSTEM_3_THINK_MANAGE_H
