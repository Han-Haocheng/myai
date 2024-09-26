#ifndef MYAI_LINK_H_
#define MYAI_LINK_H_

#include "type.h"
#include <unordered_map>
#include <utility>
#include <vector>

MYAI_BEGIN

/// <summary>
/// link
/// </summary>
typedef union Link {
  using pair_t = std::pair<const nodeid_t, weight_t>;
  struct {
    nodeid_t id;
    weight_t weight;
  };

  explicit Link();
  explicit Link(nodeid_t lk_node, weight_t lk_weight);
  explicit Link(const pair_t &pair);

  inline bool operator==(Link &rhs) const { return rhs.id == id; }
  inline bool operator!=(Link &rhs) const { return rhs.id != id; }

  inline pair_t &to_pair() { return m_pair; }
  inline const pair_t &to_pair() const { return m_pair; }

  inline char *to_char() { return m_char; }
  inline const char *to_char() const { return m_char; }

private:
  pair_t m_pair;
  char m_char[sizeof(pair_t)];
} link_t;

/// <summary>
/// link stream.
/// </summary>
class LinkStream {
public:
  using ptr = std::shared_ptr<LinkStream>;

  LinkStream &operator<<(const LinkStream &);

  LinkStream &operator>>(LinkStream &);

  void clear();

private:
  std::vector<link_t> m_links;
};

MYAI_END


#endif// !MYAI_LINK_H_
