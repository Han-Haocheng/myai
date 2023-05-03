//
// Created by HanHaocheng on 2023/04/27.
//

#ifndef THINKSYSTEM_1_THINK_TYPE_H
#define THINKSYSTEM_1_THINK_TYPE_H
#include "0_include.h"
#include "1_think_enum.h"

#define GetValInRange(min_val, max_val, val) (std::max(min_val, std::min(max_val, val)))


namespace think {
#pragma pack(push, 2)

static const constexpr size_t NUM_NODE_ID_MAX   = 0x1000000000000U;


constexpr static const char *STR_NODE_LIB_PATH  = "./nodelib/";
constexpr static const char *STR_NODE_FILE_NAME = "node.dat";

using NODE_ID                                   = size_t;
using C_NODE_ID                                 = const NODE_ID;
using UP_NODE_ID                                = std::unique_ptr<NODE_ID>;
using UPC_NODE_ID                               = std::unique_ptr<C_NODE_ID>;


[[nodiscard]] std::string GetNodeParentPath(NODE_ID id) {
  std::string res = STR_NODE_LIB_PATH;
  for (size_t i = 0U; i < sizeof(NODE_ID); ++i) {
    res += std::to_string(id & 0xff) + "/";
    id >>= 8;
  }
  return res;
}

[[nodiscard]] std::string GetNodePath(NODE_ID id) {
  return GetNodeParentPath(id) + STR_NODE_FILE_NAME;
}


#define NUM_LINK_VAL_MAX 0xffff
#define NUM_LINK_VAL_MIN 0x0

using LINK_VAL = uint16_t;
struct SLink {
  NODE_ID id   = 0;
  LINK_VAL val = NUM_LINK_VAL_MIN;
  SLink()      = default;
  SLink(NODE_ID id, LINK_VAL val) : id(id), val(val) {}
};



// 软性（浮点型）链接会导致链接值的局部性，从而破坏思维链接一致性原则，因此链接必须是硬性（整型）的。

// 软性链接列表

using ALL_LINK_VAL = size_t;
using WEIGHT       = double;

#define USE_FLOAT_LINK false
#if USE_FLOAT_LINK

struct SLinkList {
  ALL_LINK_VAL all_val = 0;
  std::vector<SLink> data;

  SLinkList() = default;

  SLinkList &operator+=(int val) {
    all_val = 0;
    std::transform(data.begin(), data.end(), data.begin(), [&](SLink &lk) {
      lk.val = std::min<int>(NUM_LINK_VAL_MAX, lk.val + val);
      all_val += lk.val;
      return lk;
    });
    return *this;
  }

  SLinkList &operator-=(int val) {
    all_val = 0;
    std::transform(data.begin(), data.end(), data.begin(), [&](SLink &lk) {
      lk.val = std::max<int>(NUM_LINK_VAL_MIN, lk.val - val);
      all_val += lk.val;
      return lk;
    });
    return *this;
  }

  SLinkList &operator*=(WEIGHT weight) {
    all_val = 0;
    std::for_each(data.begin(), data.end(), [&](SLink &lk) -> void {
      lk.val = static_cast<LINK_VAL>(static_cast<double>(lk.val) * weight);
      all_val += lk.val;
    });
    return *this;
  }

  SLinkList operator*(WEIGHT weight) {
    SLinkList res = *this;
    return res *= weight;
  }

  SLinkList operator-(int val) {
    SLinkList res = *this;
    return res -= val;
  }

  SLinkList operator+(int val) {
    SLinkList res = *this;
    return res += val;
  }

  [[nodiscard]] bool empty() const { return data.empty(); }

  void clear() {
    data.clear();
    all_val = 0;
  }

  SLink &emplace(NODE_ID &&id, LINK_VAL &&val) {
    this->all_val += val;
#if __cplusplus > 201402L
    return data.emplace_back(id, val);
#else
    data.emplace_back(id, val);
    return data.back();
#endif
  }

  void insert(const SLinkList &other) {
    data.insert(data.end(), other.data.begin(), other.data.end());
    all_val += other.all_val;
  }

  void erase(LINK_VAL standardValue) {
    all_val = 0;
    auto it = std::remove_if(data.begin(), data.end(), [&](const SLink &avg) {
      if (avg.val > standardValue) {
        all_val += avg.val;
        return true;
      } else {
        return false;
      }
    });
    data.erase(it, data.end());
    for (const auto &item: data) {
      all_val += item.val;
    }
  }

  void erase(WEIGHT standardWeight) {
    erase(static_cast<LINK_VAL>(static_cast<double>(all_val) * standardWeight));
  }
};

using ACTIVE_VAL = double;

#else
struct SLinkList {
  std::vector<SLink> data;

  SLinkList &operator+=(int val) {
    std::transform(data.begin(), data.end(), data.begin(), [&](SLink &lk) {
      lk.val = std::min<int>(NUM_LINK_VAL_MAX, lk.val + val);
      return lk;
    });
    return *this;
  }

  SLinkList &operator-=(int val) {
    std::transform(data.begin(), data.end(), data.begin(), [&](SLink &lk) {
      lk.val = std::max<int>(NUM_LINK_VAL_MIN, lk.val - val);
      return lk;
    });
    return *this;
  }

  SLinkList &operator*=(WEIGHT weight) {
    std::for_each(data.begin(), data.end(), [&](SLink &lk) -> void {
      lk.val = static_cast<LINK_VAL>(static_cast<double>(lk.val) * weight);
    });
    return *this;
  }

  SLinkList operator*(WEIGHT weight) {
    SLinkList res = *this;
    return res *= weight;
  }

  SLinkList operator-(int val) {
    SLinkList res = *this;
    return res -= val;
  }

  SLinkList operator+(int val) {
    SLinkList res = *this;
    return res += val;
  }

  SLink &emplace(NODE_ID &&id, LINK_VAL &&val) {
#if __cplusplus > 201402L
    return data.emplace_back(id, val);
#else
    data.emplace_back(id, val);
    return data.back();
#endif
  }

  void insert(const SLinkList &other) {
    data.insert(data.end(), other.data.begin(), other.data.end());
  }

  void erase(LINK_VAL standardValue) {
    auto it = std::remove_if(data.begin(), data.end(), [&](const SLink &avg) {
      return avg.val > standardValue;
    });
    data.erase(it, data.end());
  }
  void clear() {
    data.clear();
  }
};

using ACTIVE_VAL = size_t;

#endif

using C_LINK_LIST  = const SLinkList;
using UP_LINK_LIST = std::unique_ptr<SLinkList>;
using LPCLINKLIST  = std::unique_ptr<C_LINK_LIST>;

struct SNodeInfo {
  NODETYPE nodetype = NODETYPE::NULL_TYPE;
  SLinkList ctLks;
  SLinkList scLks;
  SLinkList dyLks;

  SNodeInfo getActGroup(ACTIVE_VAL actWeight) {
#if USE_FLOAT_LINK
    return {this->nodetype,
            this->ctLks * actWeight,
            this->scLks * actWeight,
            this->dyLks * actWeight};
#else
    return {this->nodetype,
            this->ctLks * (static_cast<WEIGHT>(actWeight) / (NUM_LINK_VAL_MAX + 1)),
            this->scLks * (static_cast<WEIGHT>(actWeight) / (NUM_LINK_VAL_MAX + 1)),
            this->dyLks * (static_cast<WEIGHT>(actWeight) / (NUM_LINK_VAL_MAX + 1))};
#endif
  }
};

using CLINKGROUP   = const SLinkList;
using UP_NODE_INFO = std::unique_ptr<SNodeInfo>;
using SP_NODE_INFO = std::shared_ptr<SNodeInfo>;
using WPLINKGROUP  = std::weak_ptr<SNodeInfo>;

/*using NODE        = std::pair<NODE_ID, SP_NODE_INFO>;
using CNODE       = const NODE;
using UPNODE      = std::unique_ptr<NODE>;
using SPNODE      = std::shared_ptr<NODE>;
using UPCNODE     = std::unique_ptr<CNODE>;*/


/*typedef struct tagNODE {

  NODE_ID m_id_;
  UP_NODE_INFO lpLkGp_;
  explicit tagNODE(NODE_ID id, NODETYPE type = NODETYPE::NULL_TYPE, UP_NODE_INFO lpLG = nullptr)
      : m_id_(id), lpLkGp_(lpLG) {}

  tagNODE &operator=(tagNODE &&)      = delete;
  tagNODE &operator=(const tagNODE &) = delete;
  [[nodiscard]] bool isNull() const { return m_id_ == 0; }
  bool operator==(const tagNODE &rhs) const { return m_id_ == rhs.m_id_; }
  bool operator!=(const tagNODE &rhs) const { return !(rhs == *this); }
  bool operator<(const tagNODE &rhs) const { return m_id_ < rhs.m_id_; }
  bool operator>(const tagNODE &rhs) const { return rhs < *this; }
  bool operator<=(const tagNODE &rhs) const { return !(rhs < *this); }
  bool operator>=(const tagNODE &rhs) const { return !(*this < rhs); }

  inline void addAct(ACTIVE_VAL rhs) const { lpLkGp_->actVal += rhs; }
  void saveDynamicData(double saveWeight) const {
    lpLkGp_->dyLks.erase(saveWeight);
    lpLkGp_->scLks.insert(lpLkGp_->dyLks);
    lpLkGp_->dyLks.clear();
  }

  inline void setActWeight(LPACTLIST out) const {
    lpLkGp_->dyLks.getActList(lpLkGp_->m_act_val_, out);
    lpLkGp_->ctLks.getActList(lpLkGp_->m_act_val_, out);
    lpLkGp_->scLks.getActList(lpLkGp_->m_act_val_, out);
  }

  void tidyUp() const {
  }

  void emplace_link(NODETYPE &&ty, NODE_ID &&id, LINK_VAL &&all_val) {
    if (ty == NODETYPE::DYNAMIC_TYPE) {
      this->lpLkGp_->dyLks.emplace(id, all_val);
    }
    this->lpLkGp_->ctLks.insert(other.ctLks);
    this->lpLkGp_->scLks.insert(other.scLks);
    this->lpLkGp_->dyLks.insert(other.dyLks);
  }

  void emplace_link_group(const SNodeInfo &other) const {
    this->lpLkGp_->ctLks.insert(other.ctLks);
    this->lpLkGp_->scLks.insert(other.scLks);
    this->lpLkGp_->dyLks.insert(other.dyLks);
  }
} NODE, *LPNODE;
typedef const NODE CNODE, *UPCNODE;

typedef std::unordered_set<NODE> NODESET, *LPNODESET;
typedef const NODESET CNODESET, *LPCNODESET;*/


#pragma pack(pop)

}// namespace think

#endif//THINKSYSTEM_1_THINK_TYPE_H
