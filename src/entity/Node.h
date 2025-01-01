#ifndef MYAI_NODE_H_
#define MYAI_NODE_H_

#include <array>
#include <fstream>
#include <utility>

#include "../define.h"
#include "Link.h"

MYAI_BEGIN

/**
 * @brief 用于保存节点
 */
class Node {
  friend class MyaiDatabase;
  friend class NodeService;

public:
  constexpr static const size_t MAX_LINK_NUMS = 0x1000;
  constexpr static const weight_t NULL_WEIGHT = 0.0;
  constexpr static const nodeid_t NULL_ID = 0;

  using const_ptr = std::shared_ptr<const Node>;
  using ptr = std::shared_ptr<Node>;

  enum State
  {
    // 1 未创建：未分配id
    // 2 已销毁：id被释放
    // 3 未填充数据：只有基础属性，没有链接属性
    // 4 已就绪：可以进行操作的节点
    // 5 已激活：已经完成激活的节点
    // 6 已整理：已经完成数据整理
    // 7 已保存：已经和硬盘同步的节点
    // 1-
    NDS_UNDEFINED,
    NDS_UNUSED,
    NDS_EMPTY,
    NDS_NORMAL,
    NDS_DELETE,
  };

  Node();
  ~Node() = default;

  inline auto bias() const { return m_bias; }
  inline auto size() const { return m_links.size(); }
  inline auto id() const { return m_id; }
  inline auto state() const { return m_state; }
  inline const auto &links() const { return m_links; }

private:
  nodeid_t m_id;
  weight_t m_bias;
  State m_state;
  std::vector<Link> m_links;
  std::vector<Link> m_buffer;
};



//class NodeLinkStream {
//public:
//  using ptr = std::shared_ptr<NodeLinkStream>;
//
//  std::unordered_map<nodeid_t, weight_t> stream;
//
//  NodeLinkStream &operator<<(const std::vector<Link> &lks) {
//    for (auto &lk: lks) {
//      stream.emplace(lk.id, lk.weight);
//    }
//    return *this;
//  }
//
//  NodeLinkStream &operator>>(std::vector<Link> &lks) {
//    for (auto &lk: stream) {
//      lks.emplace(lk.first, lk.second);
//    }
//    return *this;
//  }
//
//  [[nodiscard]] std::vector<Link> &&links() const {
//    std::vector<Link> res;
//    res.reserve(stream.size());
//    for (auto &lk: stream) {
//      res.emplace(lk);
//    }
//    return std::move(res);
//  }
//};

MYAI_END

#endif// !MYAI_NODE_H_
