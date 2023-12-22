//
// Created by HanHaocheng on 2023/9/27.
//

#include "node.h"

#include <fstream>
#include <memory>
#include <utility>
#include <valarray>
namespace myai
{

//static NoderManager::ptr s_node_mgr = NoderManager::GetInstance();
Node::ptr Recorder::s_shareNode = nullptr;
Node::ptr Recorder::UpdateNode(NodeType::Type type, double bais)
{
  s_shareNode = NoderManager::GetInstance()->lookup(bais, type);
  return s_shareNode;
}
Recorder::Recorder(NodeBuffer::ptr record_nodes) : m_recordeds(std::move(record_nodes))
{
}

class IdAllocator
{
public:
  typedef id_t IdType;
  typedef std::shared_ptr<IdAllocator> ptr;
  std::vector<IdType> alloc(size_t size = 1)
  {
    std::vector<IdType> res{size};
    for (int i = 0; i < size; ++i) {
      if (m_startId >= m_endId) {
        throw std::range_error("IdAllocator::alloc() out of range");
      }
      res.emplace_back(m_startId);
      ++m_startId;
    }
    return res;
  }
  void dealloc(std::vector<IdType> &&id)
  {
    m_unuseId.insert(m_unuseId.end(), id.begin(), id.end());
  }

  ~IdAllocator() = default;
  static IdAllocator GetInstance()
  {
    static IdAllocator s_id_alloc;
    return s_id_alloc;
  }

private:
  IdAllocator(){};
  IdType m_startId = 0;
  IdType m_endId = 0;
  std::vector<IdType> m_unuseId;
};

typedef IdAllocator IdAlloc;

Edge::Edge(Edge::IdType id, double weight) : m_linkId(id), m_weight(weight) {}

std::pair<Edge::IdType, double> Edge::toPair() { return std::make_pair(m_linkId, m_weight); }

void Edge::fromPair(const std::pair<IdType, double> &v)
{
  m_linkId = v.first;
  m_weight = v.second;
}

Node::Node(Node::IdType id, NodeType::Type type, double bias) : m_id(id), m_type(type), m_bias(bias), m_actWeight(0) {}

bool Node::operator==(const Node &rhs) const
{
  return rhs.m_id == m_id;
}

bool Node::operator!=(const Node &rhs) const
{
  return !(rhs == *this);
}

void NodeBuffer::addNode(const Node::ptr &node) { m_nodes.emplace(node->getId(), node); }

void NodeBuffer::delNode(NodeBuffer::IdType id) { m_nodes.erase(id); }

void NodeBuffer::clear() { m_nodes.clear(); }

Node::ptr NodeBuffer::getNode(NodeBuffer::IdType id)
{
  auto findRes = m_nodes.find(id);
  return findRes == m_nodes.end() ? nullptr : findRes->second;
}

void BaseActivater::init() {}

void BaseActivater::activate(Node::ptr acted_node, NodeBuffer::ptr act_res)
{
  //激活函数的位置 默认线性
  double act_w = acted_node->actWeight() + acted_node->getBias();

  acted_node->actWeight() = 0;
  auto &next_edges = acted_node->nextEdges();

  auto activate_func = [&](Edge &next_edge) {
    if (next_edge.getWeight() == 0) {
      return true;
    }
    auto next_node = act_res->getNode(next_edge.getLinkId());
    if (!next_node) {
      next_node = NoderManager::GetInstance()->lookup(next_edge.getLinkId());
      if (!next_node) {
        return true;
      }
      act_res->addNode(next_node);
    }
    next_node->actWeight() += act_w * next_edge.getWeight();
    return false;
  };

  next_edges.erase(std::remove_if(next_edges.begin(), next_edges.end(), activate_func), next_edges.end());
}

void NoderManager::activate()
{
  NodeBuffer::ptr next_act_nodes{new NodeBuffer()};
  m_preRecorder->record(m_actNodes);
  for (const auto &act_pair: m_actNodes->getAllNodes()) {
    auto &act_node = act_pair.second;

    if (act_node->getType() == NodeType::NT_UNKNOWN) {
      throw std::logic_error("NoderManager::activate() node type is LL_UNKNOWN");
    }

    m_activaters[act_node->getType()]->activate(act_node, next_act_nodes);
  }
  m_nextRecorder->record(next_act_nodes);
  Recorder::UpdateNode(NodeType::BASIC_NODE, 0);
  m_actNodes = next_act_nodes;
}

void NoderManager::study()
{
}

Node::ptr NoderManager::lookup(double bias, NodeType::Type type)
{
  Node::ptr node{new Node(IdAlloc::GetInstance().alloc()[0], type, bias)};
  m_rootNodes->addNode(node);
  return node;
}
void NoderManager::init()
{
  m_activaters = {
#define XX(type, cl)                              \
  {                                               \
    NodeType::type, NodeActivater::ptr { new cl } \
  }
      XX(BASIC_NODE, BaseActivater()),
      XX(INPUT_NODE, BaseActivater())
#undef XX
  };

  NodeBuffer::ptr records;
  m_preRecorder->setRecords(records);
  m_nextRecorder->setRecords(records);
}
NoderManager::ptr NoderManager::GetInstance()
{
  static NoderManager::ptr node_manager{new NoderManager};
  return node_manager;
}

void PreRecorder::record(NodeBuffer::ptr nodes)
{
  for (const auto &node_pair: nodes->getAllNodes()) {
    auto node = node_pair.second;
    node->nextEdges().emplace_back(s_shareNode->getId(), 1.0);
  }
}

void NextRecorder::record(NodeBuffer::ptr ptr)
{
  for (const auto &node_pair: ptr->getAllNodes()) {
    auto node = node_pair.second;
    s_shareNode->nextEdges().emplace_back(node->getId(), node->actWeight() * 0.1);
    // node->nextEdges().emplace_back(s_shareNode->getLinkId(), 1.0);
  }
}
}// namespace myai