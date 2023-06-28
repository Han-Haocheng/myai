//
// Created by HanHaocheng on 2023/06/06.
//

#ifndef THINK_LINKSET_H
#define THINK_LINKSET_H

#include <forward_list>
#include <unordered_map>
#include <vector>

#include "../../io/Serialization.h"
#include "LinkElement.h"

namespace thinksystem::entity
{
interface LinkListInterface
{
public:
  using value_type         = LinkElement;
  using reference          = LinkElement &;
  using const_reference    = const LinkElement &;
  using size_type          = size_t;
  using self_type          = LinkListInterface;
  using deduplic_container = ::std::unordered_map<id_t, link_t>;
  using auto_list          = std::initializer_list<LinkElement>;

public:
  virtual reference append(value_type &&)   = 0;
  virtual reference append(const_reference) = 0;
  virtual bool tidyUp()                     = 0;
};

class LinkArrayList : private std::vector<LinkElement>, public LinkListInterface
{
public:
  using list       = std::vector<LinkElement>;
  using self_type  = LinkArrayList;
  using super_type = LinkListInterface;

public:
  using list::begin;
  using list::cbegin;
  using list::cend;
  using list::end;

public:
  LinkElement &append(value_type &&newEle) final { return this->emplace_back(newEle); }
  LinkElement &append(const_reference newEle) final { return this->emplace_back(newEle); }
  void append(self_type &&lst)
  {
    this->insert(end(), std::make_move_iterator(lst.begin()), std::make_move_iterator(lst.end()));
  }
  void append(const self_type &lst) { this->insert(end(), lst.cbegin(), lst.cend()); }

  bool tidyUp() override
  {
    if (this->empty()) { return false; }
    deduplic_container tmpMap(this->size());
    for (auto &&item: *this)
    {
      auto findRes = tmpMap.find(item.id());
      if (findRes != tmpMap.end()) { findRes->second += item.linkVal(); }
      else { tmpMap.emplace((value_type::link_pair) item); }
    }
    this->assign(tmpMap.begin(), tmpMap.end());
    return true;
  }
};

class LinkLinkedList : private std::forward_list<LinkElement>, public LinkListInterface
{
public:
  using list       = std::forward_list<LinkElement>;
  using self_type  = LinkLinkedList;
  using super_type = LinkListInterface;

public:
  using list::begin;
  using list::cbegin;
  using list::cend;
  using list::end;

public:
  LinkElement &append(value_type &&newEle) override { return this->emplace_front(newEle); }
  LinkElement &append(const_reference newEle) override { return this->emplace_front(newEle); }
  void append(self_type &&lst) { this->splice_after(cbefore_begin(), lst); }
  void append(const self_type &lst) { insert_after(cbefore_begin(), lst.cbegin(), lst.cend()); }
  bool tidyUp() override
  {
    if (this->empty()) { return false; }
    deduplic_container tmpMap;
    for (auto &&item: *this)
    {
      auto findRes = tmpMap.find(item.id());
      if (findRes != tmpMap.end()) { findRes->second += item.linkVal(); }
      else { tmpMap.emplace((value_type::link_pair) item); }
    }
    this->assign(tmpMap.begin(), tmpMap.end());
    return true;
  }
};

class LinkSet : public io::SerializationInterface
{
public:
  using value_type         = LinkElement;
  using reference          = LinkElement &;
  using const_reference    = const LinkElement &;

  using size_type          = size_t;
  using array_list         = ::std::vector<value_type>;
  using linked_list        = ::std::forward_list<value_type>;
  using deduplic_container = ::std::unordered_map<id_t, link_t>;

private:
  array_list *m_const_;
  array_list *m_normal_;
  linked_list *m_cache_;

public:
  LinkSet() : m_const_(new array_list()), m_normal_(new array_list()), m_cache_(new linked_list()) {}

  explicit LinkSet(id_t id) : m_const_(nullptr), m_normal_(nullptr), m_cache_(new linked_list())
  {
    io::Serialization(utils::to_path<id_t>(id) + STR_NODE_FILE_NAME).deserialization(*this);
  }

  ///转移构造
  LinkSet(LinkSet &&other) noexcept : m_const_(other.m_const_), m_normal_(other.m_normal_), m_cache_(other.m_cache_)
  {
    other.m_const_  = nullptr;
    other.m_normal_ = nullptr;
    other.m_cache_  = nullptr;
  }

  ///复制构造
  LinkSet(const LinkSet &other) noexcept
  : m_const_(new array_list()),
    m_normal_(new array_list()),
    m_cache_(new linked_list())
  {
    m_const_->assign(other.m_const_->begin(), other.m_const_->end());
    m_normal_->assign(other.m_normal_->begin(), other.m_normal_->end());
    m_cache_->assign(other.m_cache_->begin(), other.m_cache_->end());
  }

  ~LinkSet()
  {
    delete m_const_;
    delete m_normal_;
    delete m_cache_;
  }

public:
  LinkSet &operator=(LinkSet &&other) noexcept
  {
    if (&other != this)
    {
      delete m_const_;
      delete m_normal_;
      delete m_cache_;
      this->m_const_  = other.m_const_;
      this->m_normal_ = other.m_normal_;
      this->m_cache_  = other.m_cache_;
    }
    return *this;
  }
  LinkSet &operator=(const LinkSet &other)
  {
    if (&other != this)
    {
      m_const_->assign(other.m_const_->begin(), other.m_const_->end());
      m_normal_->assign(other.m_normal_->begin(), other.m_normal_->end());
      m_cache_->assign(other.m_cache_->begin(), other.m_cache_->end());
    }
    return *this;
  }

public:
  array_list &constLinks() { return *m_const_; }
  array_list &normalLinks() { return *m_normal_; }
  linked_list &cacheLinks() { return *m_cache_; }
  [[nodiscard]] const array_list &constLinks() const { return *m_const_; }
  [[nodiscard]] const array_list &normalLinks() const { return *m_normal_; }
  [[nodiscard]] const linked_list &cacheLinks() const { return *m_cache_; }

public:
  void close(id_t id) { io::Serialization(utils::to_path<id_t>(id) + STR_NODE_FILE_NAME).serialization(*this); }

  void writeFunc(std::ofstream *ofPtr) const final
  {
    ofPtr->write(reinterpret_cast<const char *>(m_const_->size()), sizeof(m_const_->size()));
    ofPtr->write(reinterpret_cast<const char *>(m_normal_->size()), sizeof(m_normal_->size()));
    ofPtr->write(reinterpret_cast<const char *>(m_const_->data()),
                 static_cast<std::streamsize>(sizeof(value_type) * m_const_->size()));
    ofPtr->write(reinterpret_cast<const char *>(m_normal_->data()),
                 static_cast<std::streamsize>(sizeof(value_type) * m_normal_->size()));
  }
  void readFunc(std::ifstream *ifPtr) final
  {
    size_type constSize = 0, normalSize = 0;
    ifPtr->read(reinterpret_cast<char *>(&constSize), sizeof(size_type));
    ifPtr->read(reinterpret_cast<char *>(&normalSize), sizeof(size_type));
    m_const_  = new array_list(constSize);
    m_normal_ = new array_list(normalSize);
    ifPtr->read(reinterpret_cast<char *>(m_const_->data()),
                static_cast<std::streamsize>(sizeof(value_type) * constSize));
    ifPtr->read(reinterpret_cast<char *>(m_normal_->data()),
                static_cast<std::streamsize>(sizeof(value_type) * normalSize));
  }

public:
  reference appendBuffer(value_type &&newLk) { return m_cache_->emplace_front(newLk); }
  reference appendBuffer(const_reference newLk) { return m_cache_->emplace_front(newLk); }
  void appendBuffer(const linked_list &lst)
  {
    m_cache_->insert_after(m_cache_->cbefore_begin(), lst.begin(), lst.end());
  }
  void appendBuffer(linked_list &&lst) { m_cache_->splice_after(m_cache_->cbefore_begin(), std::move(lst)); }

  reference appendStatic(value_type &&newLk) { return m_normal_->emplace_back(newLk); }
  reference appendStatic(const_reference newLk) { return m_normal_->emplace_back(newLk); }
  void appendStatic(const array_list &lst) { m_normal_->insert(m_normal_->cend(), lst.begin(), lst.end()); }
  void appendStatic(array_list &&lst)
  {
    m_normal_->insert(m_normal_->cend(), std::make_move_iterator(lst.begin()), std::make_move_iterator(lst.end()));
  }

public:
  bool stable(weight_t stableWeight)
  {
    if (m_cache_->empty()) return false;
    for (value_type &item: *m_cache_) item *= stableWeight;
    m_normal_->insert(m_normal_->end(), std::make_move_iterator(m_cache_->begin()),
                      std::make_move_iterator(m_cache_->end()));
    return true;
  }

  bool tidyUp()
  {
    auto *tmpMap = new deduplic_container(m_normal_->size());
    for (auto &&item: *m_normal_)
    {
      auto findRes = tmpMap->find(item.id());
      if (findRes != tmpMap->end()) { findRes->second += item.linkVal(); }
      else { tmpMap->emplace((value_type::link_pair) item); }
    }

    delete tmpMap;
  }
};

}// namespace thinksystem::entity
#endif//THINK_LINKSET_H
