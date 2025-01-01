#ifndef MYAI_NODE_H_
#define MYAI_NODE_H_


#include "define.h"

#include <memory>
#include <vector>


MYAI_BEGIN

struct ActivateInfo {
  weight_t active_val;
  std::vector<NodeLink> records;
};

class MyaiNode {
public:
  using ptr                                     = std::shared_ptr<MyaiNode>;
  static constexpr const weight_t MAX_NODE_BAIS = 100.0;
  static constexpr const weight_t MIN_NODE_BAIS = -100.0;
  static constexpr const size_t MAX_NODE_LINK_NUM = 0x10000;
  enum Type : int8 {
    ND_UNDEFINED = -1,
    ND_MEMORY,
    ND_EMOTION,
    ND_AUDIO,
    ND_VIDEO,
    ND_MODEL_ACT,
    ND_MODEL_SENSE,
  };
  MyaiNode();
  ~MyaiNode();

  inline nodeid_t id() const { return m_id; }
  inline weight_t bias() const { return m_bias; }
  inline const std::vector<NodeLink> &links() const {
    std::vector<NodeLink> v{m_links.begin(), m_links.end()};
    v.insert(v.end(), m_act_infos.records.begin());
    return ; }
  // inline std::vector<NodeLink> &links() { return m_links; }

  inline void setBias(weight_t bias) {
    if (bias >= MAX_NODE_BAIS || bias <= MIN_NODE_BAIS) {
      //TODO: <log> date error
      return;
    }
    m_bias = bias;
  }

  Type type() const { return m_type; }


  void addLink(const NodeLink& link) {
    m_act_infos.records.emplace_back(link);
  }


private:
  Type m_type;
  nodeid_t m_id;
  weight_t m_bias;
  std::vector<NodeLink> m_links;
  ActivateInfo m_act_infos;
};

using Node = MyaiNode;

MYAI_END

#endif// !MYAI_NODE_H_