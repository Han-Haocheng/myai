//
// Created by HanHaocheng on 2023/11/7.
//

#include "Node.h"

namespace myai
{

Node::Node() = default;
Node::Node(uint64_t id, uint16_t bais, uint16_t edgeNum, uint16_t activateNum, Node::Type type)
    : m_id(id), m_type(type), m_bais(bais), m_activateNum(activateNum), m_edgeNum(edgeNum) {}

uint64_t Node::getId() const { return m_id; }
void Node::setId(uint64_t id) { m_id = id; }
uint16_t Node::getBais() const { return m_bais; }
void Node::setBais(uint16_t bais) { m_bais = bais; }
uint16_t Node::getEdgeNum() const { return m_edgeNum; }
void Node::setEdgeNum(uint16_t edgeNum) { m_edgeNum = edgeNum; }
uint16_t Node::getActivateNum() const { return m_activateNum; }
void Node::setActivateNum(uint16_t activateNum) { m_activateNum = activateNum; }
Node::Type Node::getType() const { return m_type; }
void Node::setType(Node::Type type) { m_type = type; }
uint32_t Node::getActivateValue() const { return m_activateValue; }
void Node::setActivateValue(uint32_t activateValue) { m_activateValue = activateValue; }
std::vector<Node::Edge> &Node::edges() { return m_edges; }

}// namespace myai