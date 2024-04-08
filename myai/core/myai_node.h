#ifndef MYAI_NODE_H_
#define MYAI_NODE_H_


#include "myai_def.h"

#include <memory>
#include <vector>


MYAI_SPACE_BEGIN

struct NodeLink {
  nid_t id;
  weight_t val;
  NodeLink() : id(0), val(0) {}
  NodeLink(nid_t id, weight_t val) : id(id), val(val) {}
};

using LinkList = std::vector<NodeLink>;

class MyaiNode {
public:
  using ptr                                     = std::shared_ptr<MyaiNode>;
  static constexpr const weight_t MAX_NODE_BAIS = 100.0;
  static constexpr const weight_t MIN_NODE_BAIS = -100.0;
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

  inline nid_t id() const { return m_id; }
  inline weight_t bias() const { return m_bias; }
  inline std::weak_ptr<LinkList> links() const { return std::weak_ptr<LinkList>{m_links}; }

  inline void setBias(weight_t bias) {
    if (bias >= MAX_NODE_BAIS || bias <= MIN_NODE_BAIS) {
      //TODO: <log> date error
      return;
    }
    m_bias = bias;
  }

  Type type() const { return m_type; }

  weight_t activeValue;

private:
  Type m_type;
  nid_t m_id;
  weight_t m_bias;
  std::shared_ptr<LinkList> m_links;
};

using Node = MyaiNode;

MYAI_SPACE_END

#endif// !MYAI_NODE_H_