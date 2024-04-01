
#ifndef MYAI_MODEL_LINK_H_
#define MYAI_MODEL_LINK_H_
#include "../config/define.h"

#include <hash_map>

MYAI_BEGIN

class Link {
public:
  struct Hash {
    size_t operator()(const Link &link) const { return std::hash<id_t>{}(link.id); }
  };

  inline Link &operator=(const Link &) = default;
  inline Link &operator=(Link &&) = default;
  inline Link &operator=(const char *bytes) { return *this = *reinterpret_cast<const Link *>(bytes); }
  inline explicit operator bool() const { return id != 0; }
  inline explicit operator const char *() const { return reinterpret_cast<const char *>(this); }

  Link() = default;
  Link(const Link &) = default;
  Link(Link &&) = default;
  ~Link() = default;

  Link(id_t link_id, weight_t weight);
  explicit Link(const std::pair<id_t, weight_t> &pair);

public:
  id_t id = 0;
  weight_t weight = 0;
};

inline bool operator==(const Link &lhs, const Link &rhs) { return lhs.id == rhs.id; }
inline bool operator!=(const Link &lhs, const Link &rhs) { return lhs.id != rhs.id; }

MYAI_END

#endif//! MYAI_MODEL_LINK_H_
