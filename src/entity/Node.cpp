

#include "Node.h"

#include <memory>
#include <sys/stat.h>
#include <utility>

MYAI_BEGIN

Node::Node() {
}

size_t Node::push(const Link &lk) {
  // 非法链接
  if (lk.weight <= 0) {
    return 0;
  }

  //插入第一个链接
  if (m_state == NDS_UNUSED) {
    m_links[m_link_size] = lk;
    m_link_size++;
    return 1;
  }

  return 1;
}

MYAI_END
