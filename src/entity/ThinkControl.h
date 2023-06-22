//
// Created by HanHaocheng on 2023/06/17.
//

#ifndef THINK_THINKCONTROL_H
#define THINK_THINKCONTROL_H
#include "../utils/ResourceAllocator.h"
#include "ThinkNode.h"
namespace thinksystem::entity
{

class ThinkControl
{
public:
  using id_type  = ThinkNode::NodeId;
  using id_alloc = utils::ResourceAllocator<id_type::value_type>;

public:
  id_alloc* m_id_alloc_ = nullptr;

public:
  ThinkControl() : m_id_alloc_(new id_alloc(id_type::MIN_NODE_LOCATION, id_type::MAX_NODE_LOCATION)) {}
  ~ThinkControl(){
    m_id_alloc_->save
  }
};

}// namespace thinksystem::entity

#endif//THINK_THINKCONTROL_H
