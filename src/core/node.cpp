//
// Created by HanHaocheng on 2023/11/7.
//

#include "node.h"
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
  struct NoderConfig {
    Node::Type type;
    id_t idStart;
    size_t idMaxNum;
  };
  NodeIdAllocatorConfig idAllocator;
  std::vector<NoderConfig> noders;
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

Logger::ptr g_logger = MYAI_LOGGER_NAME("think");

Config<ThinkConfig>::ptr NodeControl::s_think_conf =
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
  if (node["noders"].IsDefined()) {
    for (const auto &processor_node: node["noders"]) {
      ret.noders.emplace_back(ThinkConfig::NoderConfig{
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
  for (const auto &item: val.noders) {
    YAML::Node processor_node;
    processor_node["type"] = Node::toString(item.type);
    processor_node["idStart"] = item.idStart;
    processor_node["idMaxNum"] = item.idMaxNum;
    node["noders"].push_back(processor_node);
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
id_t NodeIdAllocator::allocate(size_t i)
{
  id_t res = m_currentId;
  m_currentId += i;
  return res;
}
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
void LinkGroup::addLink(const Link &link) { m_datas.emplace_front(link); }
const std::forward_list<Link> &LinkGroup::getAllLinks() const { return m_datas; }
void LinkGroup::for_each(std::function<void(Link &)> func) { std::for_each(m_datas.begin(), m_datas.end(), std::move(func)); }
void LinkGroup::for_each(std::function<void(const Link &)> func) const { std::for_each(m_datas.begin(), m_datas.end(), std::move(func)); }

//=====================================================================================================================
Node::Node(id_t id, Type type, weight_t bias) : m_id(id), m_bias(bias), m_type(type) {}
id_t Node::getId() const { return m_id; }
weight_t Node::getBias() const { return m_bias; }
void Node::setBias(weight_t bias) { m_bias = bias; }
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
    XX(EMOTION);
  }
#undef XX
}
std::unordered_map<std::string, Node::Type> s_nodeTy_str_map{
    {"1", Node::NT_MEMORY},
    {"Memory", Node::NT_MEMORY},
    {"memory", Node::NT_MEMORY},
    {"MEMORY", Node::NT_MEMORY},
    {"NT_MEMORY", Node::NT_MEMORY},
};
Node::Type Node::fromString(const std::string &type)
{
  auto res = s_nodeTy_str_map.find(type);
  return res != s_nodeTy_str_map.end() ? res->second : NT_UNKNOWN;
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
Activator::Activator(Node::Type type) : m_type(type) {}
//=====================================================================================================================
void Noder::activate(const Node::ptr &per_node, const Node::ptr &next_node)
{
  std::unordered_map<id_t, weight_t> linkInfo;
  for (const auto &active_node: m_cache) {
    active_node.second.activator->setActivationInfo(linkInfo);
  }

  std::vector<Node::ptr> activeNodes;
  for (const auto &link: linkInfo) {

    per_node->linkGroup().addLink(link.first, link.second);

    Node::ptr node = m_nodeDao->selectById(link.first);
    if (!node) {
      continue;
    }

    node->linkGroup().addLink(next_node->getId(), link.second);
    activeNodes.emplace_back(node);
    m_activtors[node->getType()]->active(Link{link});
  }
}

Node::ptr Noder::getRecordNode(weight_t bias)
{
  Node::ptr node = std::make_shared<Node>(m_idAlloc->allocate(), Node::NT_MEMORY, bias);
  if (m_nodeDao->insert(node) == 0) {
    m_idAlloc->deallocate(node->getId());
    return nullptr;
  }
  return node;
}
id_t Noder::getNode(Node::Type type, size_t size, std::vector<Node::ptr> &out_nodes)
{
  out_nodes.clear();
  out_nodes.reserve(size);
  id_t begin = m_idAlloc->allocate(size);
  for (size_t i = 0; i < size; ++i) {
    out_nodes.emplace_back(std::make_shared<Node>(begin + i, type, 0));
  }
  return begin;
}
void Noder::addActivator(const Activator::ptr &activator) { m_activtors.emplace_back(activator); }
void Noder::clearActivator() { m_activtors.clear(); }

//=====================================================================================================================

void NodeControl::init()
{
  auto &noders_config = s_think_conf->getValue().noders;
  auto conf_it = noders_config.begin();
  for (; conf_it != noders_config.end(); ++conf_it) {
    static std::unordered_map<Node::Type, std::function<Activator::ptr()>> s_type_activator_map = {
        {Node::Type::NT_MEMORY, []() { return std::make_shared<MemoryActivator>(); }},
        {Node::Type::NT_EMOTION, [&]() { return std::make_shared<EmotionalActivator>(conf_it->idStart); }},
    };
    m_noder->addActivator(s_type_activator_map[conf_it->type]());
  }
}

MemoryActivator::MemoryActivator()
    : Activator(Node::NT_MEMORY)
{
}
EmotionalActivator::EmotionalActivator(id_t id_begin)
    : Activator(Node::NT_EMOTION), m_begin(id_begin)
{
}
}// namespace myai