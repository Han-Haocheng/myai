//
// Created by HanHaocheng on 2024/2/19.
//

#ifndef MY_AI_ACTIVATOR_H
#define MY_AI_ACTIVATOR_H
#include "../config/define.h"
#include "../node/Node.h"
#include "Thinker.h"
#include <memory>

MYAI_BEGIN

class Activator {
public:
  using ptr = std::shared_ptr<Activator>;

  explicit Activator(Node::Type type,uint32_t nodeNum) : m_type(type),m_nodeNum(nodeNum) {
    m_buffer.reserve(this->m_nodeNum);
    m_nodeIds.reserve(this->m_nodeNum);
  }

  ~Activator() = default;

  bool loadInfos(const Thinker::ptr& thinker);

  [[nodiscard]] inline Node::Type getType() const{return m_type;}
  [[nodiscard]] inline uint32_t getNodeNum() const{return m_nodeNum;}

  virtual void getInputInfos(std::vector<Link> &infos) = 0;

  virtual void setOutputInfos(const std::vector<Link> &infos) = 0;

protected:
  Node::Type m_type = Node::Type::NT_UNKNOWN;
  uint32_t m_nodeNum=0;

  std::vector<id_t> m_nodeIds;
  std::vector<Node::ptr> m_buffer;
};


class EmotionActivator final : public Activator {
public:
  EmotionActivator() : Activator(Node::Type::NT_EMOTION,8) {
  }
  void getInputInfos(std::vector<Link> &infos) override {
  }
  void setOutputInfos(const std::vector<Link> &infos) override {
  }

private:
};


MYAI_END

#endif//MY_AI_ACTIVATOR_H
