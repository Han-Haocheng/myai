#ifndef MYAI_DAO_NODE_DAO_H_
#define MYAI_DAO_NODE_DAO_H_
#include "../config/define.h"
#include "Node.h"

#include <string>

MYAI_BEGIN

class NodeDao {
public:
  using ptr = std::shared_ptr<NodeDao>;

  explicit NodeDao(std::string dataRootPath = "./data");
  int insert(const Node::ptr &node, bool isForced = false);
  int update(const Node::ptr &node);

  int deleteById(id_t id);
  Node::ptr selectById(id_t id);

private:
  std::string path_parse(id_t id);

private:
  std::string m_rootPath = ".";
};

MYAI_END

#endif //! MYAI_DAO_NODE_DAO_H_