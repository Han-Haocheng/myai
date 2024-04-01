//
// Created by HanHaocheng on 2024/2/19.
//

#include "Node.h"

MYAI_BEGIN

Node::Node()
    : m_id(-1), m_type(NT_UNKNOWN), m_bias(0.0), m_link_total(0.0), m_links() {
}
Node::Node(id_t id, Node::Type type, weight_t bias)
    : m_id(id), m_type(type), m_bias(bias), m_link_total(0.0), m_links() {
}

Node::Node(id_t id, Node::Type type, weight_t bias, const Node::LinkGroup &lg)
    : m_id(id), m_type(type), m_bias(bias), m_link_total(0.0), m_links(lg) {
  for (const auto &item: lg) {
    m_link_total += item.weight;
  }
}
Node::Node(id_t id, Node::Type type, weight_t bias, Node::LinkGroup &&lg)
    : m_id(id), m_type(type), m_bias(bias), m_link_total(0.0), m_links(lg) {
  for (const auto &item: lg) {
    m_link_total += item.weight;
  }
}

void Node::for_each(const std::function<void(Link &)> &op) {
  for (auto &item: m_links) { op(item); }
}
bool Node::addLink(Link &&link) {
  if (link.id == m_id) {
    return false;
  }
  if (link.weight <= 0.0) {
    return false;
  }
  m_links.emplace_back(link);
  m_link_total += link.weight;
  return true;
}
bool Node::addLink(const Link &link) {
  if (link.id == m_id) {
    return false;
  }
  if (link.weight<= 0.0){
    return false;
  }
  m_links.emplace_back(link);
  m_link_total += link.weight;
  return true;
}
bool Node::delLinkByWeight(weight_t weight) {

  return false;
}
void Node::delLinkById(id_t id) {
}

MYAI_END
