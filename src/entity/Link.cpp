#include "Link.h"

MYAI_BEGIN

Link &Link::operator=(Link &&rhs) {
  if (&rhs == this) {
    return *this;
  }
  id = rhs.id;
  weight = rhs.weight;
  return *this;
}

Link &Link::operator=(const Link &rhs) {
  if (&rhs == this) {
    return *this;
  }
  id = rhs.id;
  weight = rhs.weight;
  return *this;
}

Link &LinkList::push(Link &&e) {
  if (m_datas.empty()) {
    m_datas.emplace_back(e);
  }
  auto fd_i = binary_search(e.weight);
  auto insert_i = m_datas.insert(fd_i, std::move(e));
  return *insert_i;
}

Link &LinkList::push(const Link &e) {
  if (m_datas.empty()) {
    m_datas.emplace_back(e);
  }
  auto fd_i = binary_search(e.weight);
  auto insert_i = m_datas.insert(fd_i, e);
  return *insert_i;
}

size_t LinkList::eraseLessStand(weight_t w) {
  // 考虑只有一个元素
  if (m_datas.empty()) {
    return 0;
  }

  
  auto i = binary_search(w);
  if (i == m_datas.end()) {
    return 0;
  }
  auto rm_n = m_datas.end() - i;
  m_datas.erase(i);

  return rm_n;
}
MYAI_END