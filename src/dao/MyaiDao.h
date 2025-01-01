#pragma once
#include "MyaiFileIO.h"

MYAI_BEGIN

class MyaiDao {
public:
  using ptr = std::shared_ptr<MyaiDao>;
  int insert(Node::ptr node);
  int insert(std::vector<Node::ptr> node);

  int updata(Node::ptr node);
  int updata(std::vector<Node::ptr> nodes);

  int deleteById(nodeid_t id);
  int deleteById(std::vector<nodeid_t> ids);

  Node::ptr selectById(nodeid_t id);
  std::vector<Node::ptr> selectById(std::vector<nodeid_t> id);

private:
  bool check_node_legality(Node::ptr node);

};

MYAI_END