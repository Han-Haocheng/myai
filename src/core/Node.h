//
// Created by HanHaocheng on 2023/11/7.
//

#ifndef MY_AI_NODE_H
#define MY_AI_NODE_H

#include "../define.h"
#include <cstdint>
#include <forward_list>
#include <memory>
#include <string>
#include <vector>

namespace myai
{

class Node
{
public:
  enum class Type : uint16_t
  {
    UNKNOWN_NODE,
    OUTPUT_NODE,
    INPUT_NODE,
    HIDDEN_NODE,
  };

public:
  using ptr = std::shared_ptr<Node>;
  using Edge = std::pair<uint64_t, uint32_t>;

  Node();
  Node(uint64_t id, uint16_t bais, uint16_t edgeNum, uint16_t activateNum, Type type);

  uint64_t getId() const;
  void setId(uint64_t id);
  uint16_t getBais() const;
  void setBais(uint16_t bais);
  uint16_t getEdgeNum() const;
  void setEdgeNum(uint16_t edgeNum);
  uint16_t getActivateNum() const;
  void setActivateNum(uint16_t activateNum);
  Type getType() const;
  void setType(Type type);
  uint32_t getActivateValue() const;
  void setActivateValue(uint32_t activateValue);

  std::vector<Edge> &edges();

private:
  uint64_t m_id = 0;
  Type m_type = Type::UNKNOWN_NODE;
  uint16_t m_bais = 0;
  uint16_t m_activateNum = 0;
  uint16_t m_edgeNum = 0;

  uint32_t m_activateValue = 0;
  std::vector<Edge> m_edges = {};
};

}// namespace myai

#endif//MY_AI_NODE_H
