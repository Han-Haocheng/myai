#ifndef MYAI_SERVICE_NODE_SERVICE_H_
#define MYAI_SERVICE_NODE_SERVICE_H_
#include "../config/define.h"
#include "../node/Node.h"
#include "../node/NodeDao.h"
#include "../node/NodeFactory.h"
#include "../node/NodeIdAllocator.h"
#include "Activator.h"

#include <unordered_map>

MYAI_BEGIN

// 思维器，处理思维的运转
class Thinker {
  enum ThinkerPeriod
  {
    INPUT_PERIOD,
    OUTPUT_PERIOD,
    LEARN_PERIOD,
  };

public:
  using ptr = std::shared_ptr<Thinker>;

  explicit Thinker(NodeFactory::ptr factory, NodeDao::ptr dao);
  ~Thinker() = default;

  Node::ptr addNode(Node::Type type, weight_t bias = 0.0) {
    Node::ptr node = m_nodeFactory->createNode(type, bias);
    if (type == Node::NT_RECORD) {
      m_recordBuffer.emplace_back(node);
    }
    return node;
  }

  Node::ptr getNode(id_t id) {
    return m_nodeFactory->createNode(id);
  }

  template<class Iterator, class Type = typename Iterator::value_type>
  bool getNode(std::vector<Node::ptr> &out, Iterator begin, Iterator end) {
    for (auto it = begin; it != end; it++) {
      id_t id = *it;
      Node::ptr node = m_nodeFactory->createNode(id);
      if (!node) {
        continue;
      }
      out.emplace_back(node);
    }
  }

  ///
  /// @brief 输入周期，从激活器中获取所有输入节点
  ///
  void inputPeriod() {
    m_rec_node = addNode(Node::Type::NT_RECORD);
    for (const auto &actor: m_activators) {
      actor->getInputInfos(m_rec_node->links());
    }
  }

  ///
  /// @breaf 输出周期，按照输出表执行行为
  ///
  void outputPeriod() {
    for (const auto &link: m_rec_node->links()) {
      auto node = getNode(link.id);
      if (!node) {
        continue;
      }
      
    }

    m_recordBuffer.emplace_back(m_rec_node);
  }

  // 学习周期
  void learnPeriod() {
    for (const auto &item: m_recordBuffer) {
    }
  }

protected:
  NodeFactory::ptr m_nodeFactory = nullptr;
  std::vector<Activator::ptr> m_activators;
  Node::ptr m_rec_node;
  std::vector<Node::ptr> m_recordBuffer;
};

MYAI_END
#endif//! MYAI_SERVICE_NODE_SERVICE_H_