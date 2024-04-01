#ifndef MYAI_UTILS_NODE_ID_ALLOCATOR_H_
#define MYAI_UTILS_NODE_ID_ALLOCATOR_H_

#include "../config/define.h"

#include "../link/Link.h"

#include <forward_list>
#include <memory>
#include <string>

MYAI_BEGIN

class NodeIdAllocator {
public:
  using ptr = std::shared_ptr<NodeIdAllocator>;
  explicit NodeIdAllocator(std::string file);

  template<class Iterator>
  explicit NodeIdAllocator(id_t currentId, Iterator begin, Iterator end) : m_currentId(currentId), m_ids(begin, end) {}
  ~NodeIdAllocator();
  id_t allocate();
  void deallocate(id_t id);

  id_t allocate(size_t i);

private:
  void load_from_file();
  void save_to_file();
  std::string m_file;
  id_t m_currentId = 0;
  std::forward_list<id_t> m_ids = {};
};

MYAI_END

#endif//!MYAI_UTILS_NODE_ID_ALLOCATOR_H_