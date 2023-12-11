//
// Created by HanHaocheng on 2023/11/7.
//

#ifndef MY_AI_NODER_H
#define MY_AI_NODER_H

#include "Node.h"
#include "NodeDao.h"
#include <unordered_map>
namespace myai
{

class Noder
{
public:
  using ptr = std::shared_ptr<Noder>;
  enum class Type
  {
    UNKNOWN,
    MEMORY,
  };

  explicit Noder(Type type, NodeDao::ptr);
  virtual ~Noder() = default;
  virtual void input(Node::ptr link_node) = 0;
  virtual void output(Node::ptr link_node) = 0;
  Type getType() const;
  void setType(Type type);

protected:
  Type m_type = Type::UNKNOWN;
  NodeDao::ptr m_nodeDao = nullptr;
  std::unordered_map<uint64_t, Node::ptr> m_inputNodes;
  std::unordered_map<uint64_t, Node::ptr> m_outputNodes;
};

class MemoryNoder : public Noder
{
public:
  explicit MemoryNoder(NodeDao::ptr nodeDao);

  void input(Node::ptr link_node) override;
  void output(Node::ptr link_node) override;

private:
};

}// namespace myai

#endif//MY_AI_NODER_H
