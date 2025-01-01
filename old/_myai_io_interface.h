#pragma once

#include "../core/myai_def.h"
#include "../core/myai_node.h"

#include <functional>
#include <unordered_map>

MYAI_SPACE_BEGIN
class MyaiIOBase {
public:
  using ptr = std::shared_ptr<MyaiIOBase>;

  MyaiIOBase(Node::Type type);
  ~MyaiIOBase();

  inline Node::Type type() const { return m_type; }
  inline size_t nodeSize() const { return m_size; }

  const std::vector<Node::ptr> &getAllNodes() const { return m_input; };

  virtual void output(std::unordered_map<nid_t, Node::ptr> &act_data) = 0;
  virtual void input(std::unordered_map<nid_t, Node::ptr> &act_data)  = 0;

  virtual void addInputInfo(NodeLink &&newInfo)                       = 0;

protected:
  nid_t m_begin;
  size_t m_size;

  std::unordered_map<nid_t, std::function<void(weight_t)>> m_output;
  std::vector<Node::ptr> m_input;

private:
  Node::Type m_type;
};

MyaiIOBase::MyaiIOBase(Node::Type type) {
}

MyaiIOBase::~MyaiIOBase() {
}

using MyaiIO = MyaiIOBase;

MYAI_SPACE_END