//
// Created by HanHaocheng on 2023/06/13.
//

#ifndef THINK_LINKCONTAINER_H
#define THINK_LINKCONTAINER_H

#include "../Define.h"
#include "LinkElement.h"

namespace thinksystem::entity
{
interface LinkConstIteratorInterface
{
  using value_type      = LinkElement;
  using reference       = LinkElement &;
  using right_reference = LinkElement &&;
  using const_reference = const LinkElement &;
  using pointer         = LinkElement *;
  using const_pointer   = LinkElement *;

  using self_type       = LinkConstIteratorInterface;

public:
  virtual const_reference operator*() const        = 0;
  virtual const_pointer operator->() const         = 0;
  virtual bool operator!=(const self_type &) const = 0;
  virtual self_type &operator++()                  = 0;
  virtual self_type &operator++(int)               = 0;
};
interface LinkIteratorInterface : public LinkConstIteratorInterface
{
public:
  using value_type      = LinkElement;
  using reference       = LinkElement &;
  using right_reference = LinkElement &&;
  using const_reference = const LinkElement &;
  using pointer         = LinkElement *;
  using const_pointer   = LinkElement *;

public:
  virtual reference operator*() = 0;
  virtual pointer operator->()  = 0;
};

interface LinkContainerInterface
{
public:
  using value_type      = LinkElement;
  using reference       = LinkElement &;
  using right_reference = LinkElement &&;
  using const_reference = const LinkElement &;

  using self_type       = LinkContainerInterface;
  using iterator        = LinkIteratorInterface;
  using const_iterator  = LinkIteratorInterface;

public:
  virtual iterator begin()  = 0;
  virtual iterator cbegin() = 0;

public:
  virtual void assign(self_type &&) = 0;
};

}// namespace thinksystem::entity
#endif//THINK_LINKCONTAINER_H
