//
// Created by HanHaocheng on 2023/05/05.
//

#ifndef THINKSYSTEM_1_NODE_H
#define THINKSYSTEM_1_NODE_H
namespace think {




/*using NODE        = std::pair<NODE_ID, SP_NODE_INFO>;
using CNODE       = const NODE;
using UPNODE      = std::unique_ptr<NODE>;
using SPNODE      = std::shared_ptr<NODE>;
using UPCNODE     = std::unique_ptr<CNODE>;*/


/*typedef struct tagNODE {

  NODE_ID m_id_;
  UP_NODE_INFO lpLkGp_;
  explicit tagNODE(NODE_ID pNode, NODETYPE type = NODETYPE::NULL_TYPE, UP_NODE_INFO lpLG = nullptr)
      : m_id_(pNode), lpLkGp_(lpLG) {}

  tagNODE &operator=(tagNODE &&)      = delete;
  tagNODE &operator=(const tagNODE &) = delete;
  [[nodiscard]] bool isNull() const { return m_id_ == 0; }
  bool operator==(const tagNODE &rhs) const { return m_id_ == rhs.m_id_; }
  bool operator!=(const tagNODE &rhs) const { return !(rhs == *this); }
  bool operator<(const tagNODE &rhs) const { return m_id_ < rhs.m_id_; }
  bool operator>(const tagNODE &rhs) const { return rhs < *this; }
  bool operator<=(const tagNODE &rhs) const { return !(rhs < *this); }
  bool operator>=(const tagNODE &rhs) const { return !(*this < rhs); }

  inline void addAct(active_t rhs) const { lpLkGp_->actVal += rhs; }
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

  void emplace_link(NODETYPE &&ty, NODE_ID &&pNode, link_t &&all_val) {
    if (ty == NODETYPE::DYNAMIC_TYPE) {
      this->lpLkGp_->dyLks.emplace(pNode, all_val);
    }
    this->lpLkGp_->ctLks.insert(other.ctLks);
    this->lpLkGp_->scLks.insert(other.scLks);
    this->lpLkGp_->dyLks.insert(other.dyLks);
  }

  void emplace_link_group(const CNodeInfo &other) const {
    this->lpLkGp_->ctLks.insert(other.ctLks);
    this->lpLkGp_->scLks.insert(other.scLks);
    this->lpLkGp_->dyLks.insert(other.dyLks);
  }
} NODE, *LPNODE;
typedef const NODE CNODE, *UPCNODE;

typedef std::unordered_set<NODE> NODESET, *LPNODESET;
typedef const NODESET CNODESET, *LPCNODESET;*/

}
#endif//THINKSYSTEM_1_NODE_H
