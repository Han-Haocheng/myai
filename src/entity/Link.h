#pragma once
#include "../define.h"

MYAI_BEGIN
/**
 * @brief 用于保存链接
 */
union Link
{
  constexpr static const nodeid_t NULL_ID = 0ULL;
  constexpr static const weight_t NULL_WEIGHT = 0.0;
  struct {
    nodeid_t id;
    weight_t weight;
  };

  std::pair<nodeid_t, weight_t> pair;

  explicit Link(nodeid_t i = NULL_ID, weight_t w = NULL_WEIGHT) : id(i), weight(w) {}
  explicit Link(const std::pair<const nodeid_t, weight_t> &p) : pair(p) {}
  Link(Link &&) = default;
  Link(const Link &) = default;
  ~Link() = default;

  Link &operator=(Link &&rhs);
  Link &operator=(const Link &rhs);
};

class LinkList {
public:
  using ptr = std::shared_ptr<LinkList>;

  LinkList() {}
  ~LinkList() {}

  Link &push(Link &&e);
  Link &push(const Link &e);

  inline auto clear() { m_datas.clear(); }

  inline auto begin() { return m_datas.begin(); }
  inline auto end() { return m_datas.end(); }
  inline auto cbegin() { return m_datas.cbegin(); }
  inline auto cend() { return m_datas.cend(); }

  size_t eraseLessStand(weight_t w);

private:
  std::vector<Link>::iterator binary_search(weight_t e);

private:
  std::vector<Link> m_datas;
};

MYAI_END