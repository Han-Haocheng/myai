//
// Created by HanHaocheng on 2023/06/17.
//

#ifndef THINK_THINKCONTROL_H
#define THINK_THINKCONTROL_H
#include "../entity/ThinkNode.h"
#include "../utils/ResourceAllocator.h"
namespace thinksystem::entity
{

class ThinkControl
{
public:
  using id_type  = ThinkNode::NodeIdType;
  using id_alloc = utils::ResourceAllocator<id_type::value_type>;

public:
  id_alloc* m_id_alloc_ = nullptr;

public:
  ThinkControl() : m_id_alloc_(new id_alloc(id_type::value_type::MIN, id_type::value_type::MAX)) {}
  ~ThinkControl() { io::Serialization("./IdAlloc").serialization(*m_id_alloc_); }
};

}// namespace thinksystem::entity

#endif//THINK_THINKCONTROL_H
