#ifndef MYAI_MODEL_NODE_H_
#define MYAI_MODEL_NODE_H_

#include "../config/define.h"
#include "../link/Link.h"

#include <memory>

MYAI_BEGIN

/// 节点
/// 节点会进行输入输出
/// /
class Node {
  friend bool operator==(const Node &lhs, const Node &rhs);
  friend bool operator!=(const Node &lhs, const Node &rhs);

public:
  //可以保存激活时间戳或最后激活的次数编号，进行有规律的清除无用节点
  using ptr = std::shared_ptr<Node>;
  using LinkGroup = std::vector<Link>;

  enum Type : int32_t
  {
    NT_UNKNOWN = -1,
    NT_EMOTION,

    NT_RECORD,
    NT_MEMORY,
  };
  struct Hash {
    size_t operator()(const Node &node) const { return std::hash<id_t>{}(node.id()); }
  };

  Node();
  Node(id_t id, Type type, weight_t bias);
  Node(id_t id, Type type, weight_t bias, const LinkGroup &lg);
  Node(id_t id, Type type, weight_t bias, LinkGroup &&lg);

  [[nodiscard]] inline id_t id() const { return m_id; }
  [[nodiscard]] inline weight_t link_total() const { return m_link_total; }
  [[nodiscard]] inline Type &type() { return m_type; }
  [[nodiscard]] inline weight_t &bias() { return m_bias; }
  [[nodiscard]] inline bool empty() const { return m_id == -1; }

  inline std::vector<Link> &links() { return m_links; }

  bool addLink(Link &&link);
  bool addLink(const Link &link);
  bool delLinkByWeight(weight_t weight);
  void delLinkById(id_t id);

  void for_each(const std::function<void(Link &)> &op);

private:
  id_t m_id;
  Type m_type;
  weight_t m_bias;
  weight_t m_link_total;

  std::vector<Link> m_links;
};

inline bool operator==(const Node &lhs, const Node &rhs) { return lhs.m_id == rhs.m_id; }
inline bool operator!=(const Node &lhs, const Node &rhs) { return lhs.m_id != rhs.m_id; }

MYAI_END

#endif//! MYAI_MODEL_NODE_H_