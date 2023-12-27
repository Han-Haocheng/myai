//
// Created by HanHaocheng on 2023/11/7.
//

#include "node.h"
#include "../config/Config.h"
#include "../config/Configer.h"
#include "../log/LoggerManager.h"
#include <utility>

namespace myai
{
struct ThinkConfig {
  struct NodeIdAllocatorConfig {
    id_t currentId;
    std::forward_list<id_t> ids;
  };
  struct NodeProcessorConfig {
    Node::Type type;
    id_t idStart;
    size_t idMaxNum;
  };
  NodeIdAllocatorConfig idAllocator;
  std::vector<NodeProcessorConfig> processors;
};
template<>
struct ConfigCast<std::string, ThinkConfig> {
  ThinkConfig operator()(const std::string &val);
};
template<>
struct ConfigCast<ThinkConfig, std::string> {
  std::string operator()(const ThinkConfig &val);
};
//=====================================================================================================================

Logger::ptr g_loggger = MYAI_LOGGER_NAME("system");
Config<ThinkConfig>::ptr g_think_conf =
    Configer::GetInstance()->setConfig<ThinkConfig>("think", {}, "think");

//=====================================================================================================================
ThinkConfig ConfigCast<std::string, ThinkConfig>::operator()(const std::string &val)
{
  YAML::Node node = YAML::Load(val);
  ThinkConfig ret{};
  if (node["idAllocator"].IsDefined()) {
    if (node["idAllocator"]["currentId"].IsDefined()) {
      ret.idAllocator.currentId = node["idAllocator"]["currentId"].as<id_t>();
    }
    if (node["idAllocator"]["ids"].IsDefined()) {
      ret.idAllocator.ids = ConfigCast<std::string, std::forward_list<id_t>>{}(Dump(node["idAllocator"]["ids"]));
    }
  }
  if (node["processors"].IsDefined()) {
    for (const auto &processor_node: node["processors"]) {
      ret.processors.emplace_back(ThinkConfig::NodeProcessorConfig{
          Node::fromString(processor_node["type"].as<std::string>()),
          processor_node["idStart"].as<id_t>(),
          processor_node["idMaxNum"].as<size_t>(),
      });
    }
  }

  return ret;
}
std::string ConfigCast<ThinkConfig, std::string>::operator()(const ThinkConfig &val)
{
  YAML::Node node;
  if (val.idAllocator.currentId != 0) {
    node["idAllocator"]["currentId"] = val.idAllocator.currentId;
  }
  node["idAllocator"]["ids"] = ConfigCast<std::forward_list<id_t>, std::string>{}(val.idAllocator.ids);
  for (const auto &item: val.processors) {
    YAML::Node processor_node;
    processor_node["type"] = Node::toString(item.type);
    processor_node["idStart"] = item.idStart;
    processor_node["idMaxNum"] = item.idMaxNum;
    node["processors"].push_back(processor_node);
  }
  return YAML::Dump(node);
}
//=====================================================================================================================
NodeIdAllocator::NodeIdAllocator(const std::string &file) { load_from_file(); }
NodeIdAllocator::~NodeIdAllocator() { save_to_file(); }
id_t NodeIdAllocator::allocate()
{
  if (m_ids.empty()) {
    return m_currentId++;
  }

  id_t res{m_ids.front()};
  m_ids.pop_front();
  return res;
}
void NodeIdAllocator::deallocate(id_t id) { m_ids.emplace_front(id); }
void NodeIdAllocator::load_from_file() {}
void NodeIdAllocator::save_to_file() {}
//=====================================================================================================================
Link::Link(id_t link_id, weight_t weight) : id(link_id), weight(weight) {}
Link &Link::operator=(const char *bytes)
{
  memcpy((void *) this, bytes, sizeof(*this));
  return *this;
}
Link::operator const char *() const { return reinterpret_cast<const char *>(this); }
Link::operator bool() const { return weight != 0 || !!(uint64_t) id; }
bool Link::operator==(const Link &rhs) const { return id == rhs.id; }
bool Link::operator!=(const Link &rhs) const { return !(rhs == *this); }
bool Link::operator<(const Link &rhs) const { return id < rhs.id; }
bool Link::operator>(const Link &rhs) const { return rhs < *this; }
bool Link::operator<=(const Link &rhs) const { return !(rhs < *this); }
bool Link::operator>=(const Link &rhs) const { return !(*this < rhs); }
//=====================================================================================================================
Node::Node(id_t id, Type type, weight_t bais) : m_type(type), m_id(id), m_bias(bais) {}
id_t Node::getId() const { return m_id; }
weight_t Node::getBais() const { return m_bias; }
void Node::setBais(weight_t bais) { m_bias = bais; }
Node::Type Node::getType() const { return m_type; }
void Node::setType(Type type) { m_type = type; }
std::string Node::toString(Node::Type val)
{
  switch (val) {
#define XX(ty)  \
  case NT_##ty: \
    return #ty
    XX(UNKNOWN);
    XX(MEMORY);
    XX(EMOTIMOAL);
  }
#undef XX
}
std::unordered_map<std::string, Node::Type> s_nodety_str_map{
    {"1", Node::NT_MEMORY},
    {"Memory", Node::NT_MEMORY},
    {"memory", Node::NT_MEMORY},
    {"MEMORY", Node::NT_MEMORY},
    {"NT_MEMORY", Node::NT_MEMORY},
};
Node::Type Node::fromString(const std::string &type)
{
  auto res = s_nodety_str_map.find(type);
  return res != s_nodety_str_map.end() ? res->second : NT_UNKNOWN;
}
bool Node::operator==(const Node &rhs) const { return m_id == rhs.m_id; }
bool Node::operator!=(const Node &rhs) const { return !(rhs == *this); }
bool Node::operator<(const Node &rhs) const { return m_id < rhs.m_id; }
bool Node::operator>(const Node &rhs) const { return rhs < *this; }
bool Node::operator<=(const Node &rhs) const { return !(rhs < *this); }
bool Node::operator>=(const Node &rhs) const { return !(*this < rhs); }
LinkGroup &Node::linkGroup() { return m_linkGroup; }
const LinkGroup &Node::linkGroup() const { return m_linkGroup; }
//=====================================================================================================================

//=====================================================================================================================

void Noder::activate()
{
  std::unordered_set<Link,Link::hash> linkInfo;
  for (const auto &active_node: m_cache) {
    active_node.second.activator->setActivationInfo(linkInfo);
  }
  //linkInfo -> active node map
  std::vector<Node::ptr> activeNodes;
  for (const auto &link: linkInfo) {
    Node::ptr node = m_nodeDao->selectById(link.first);
    if (!node) {
      continue;
    }
    activeNodes.emplace_back(node);
    m_activtors[node->getType()]->active(link.first, link.second);
  }
}
Node::ptr Noder::getRecverNode(weight_t bias)
{
  Node::ptr node = std::make_shared<Node>(m_idAlloc->allocate(), Node::NT_MEMORY, bias);
  if (m_nodeDao->insert(node) == 0) {
    m_idAlloc->deallocate(node->getId());
    return nullptr;
  }
  return node;
}
}// namespace myai