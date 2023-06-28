//
// Created by HanHaocheng on 2023/06/18.
//

#ifndef THINK_THINKNODE_H
#define THINK_THINKNODE_H
#include <forward_list>
#include <functional>
#include <sstream>
#include <string>
#include <vector>

#include "../io/Serialization.h"
#include "../utils/BoundedValue.h"
#include "../utils/Log.h"

namespace thinksystem::entity
{

using attribute_t = uint32_t;

class ThinkNode
{
public:
  /// enum NodeAttributeEnum
  /// 节点状态类型
  ///
  enum NodeAttributeEnum : attribute_t
  {
    is_null       = 0b0000'0001,
    is_empty      = 0b0000'0010,
    is_repetitive = 0b0000'0100,
    is_const      = 0b0000'1000,
    is_static     = 0b0001'0000,
    is_dynamic    = 0b0010'0000,
  };

  /// class NodeIdType
  /// 节点编号 - 用于表示节点在结构中的位置
  class NodeIdType
  {
  public:
    using value_type = utils::BoundedValue<unsigned long long, 0x0000'0000'0001ULL, 0xffff'ffff'ffffULL>;

  private:
    using self = NodeIdType;

  private:
    value_type m_val_;

  public:
    NodeIdType(self &&)      = default;
    NodeIdType(const self &) = default;
    NodeIdType() : m_val_(value_type::MAX) {}
    explicit NodeIdType(value_type id) : m_val_(id)
    {
#ifdef DEBUG
      utils::Assert(id >= MAX_NODE_LOCATION || id == NULL_NODE_LOCATION,
                    {utils::LogLevel::ERROR, "未知的节点生成结果。"});
#endif
    }

  public:
    self &operator=(self &&)      = default;
    self &operator=(const self &) = default;
    self &operator=(value_type id)
    {
#ifdef DEBUG
      utils::Assert(id >= MAX_NODE_LOCATION || id == NULL_NODE_LOCATION,
                    {utils::LogLevel::ERROR, "未知的节点生成结果。"});
#endif
      m_val_ = id;
      return *this;
    }

    friend bool operator==(const self &lhs, const self &rhs) { return lhs.m_val_ == rhs.m_val_; }
    friend bool operator==(const value_type &lhs, const self &rhs) { return lhs == rhs.m_val_; }
    friend bool operator!=(const self &lhs, const self &rhs) { return !(lhs == rhs); }
    friend bool operator!=(const value_type &lhs, const self &rhs) { return !(lhs == rhs); }

  public:
    //转换为物理位置
    [[nodiscard]] std::string to_path() const
    {
      std::stringstream ss{".\\NodeStruct\\"};
      for (size_t i = sizeof(unsigned long long); i > 0; --i)
      {
        unsigned char byte = (m_val_ >> (8 * (i - 1))) & 0xFF;
        ss << static_cast<int>(byte) << "\\";
      }
      return ss.str();
    }

    struct Hash
    {
      using hash_type = unsigned long long;
      hash_type operator()(const self &id) { return std::hash<value_type>{}(id.m_val_); }
    };

  };//! class NodeIdType

  /*class LinkSet
   * 链接集
   * */
  class LinkSet : private io::SerializationInterface
  {
  public:
    /* struct link_t
     * 链接结构
     * */
    struct link_t
    {
      using id_type    = NodeIdType;
      using value_type = utils::BoundedValue<std::uint32_t, 0x0000'0000U, 0xffff'ffffU>;
      using self       = link_t;

    public:
      id_type id;
      value_type linkVal;

    public:
      link_t() : id(), linkVal() {}
      link_t(id_type::value_type id, value_type _val) : id(id), linkVal(_val) {}

    public:
      self &operator=(const std::pair<id_type, value_type> &pr)
      {
        id      = pr.first;
        linkVal = pr.second;
      }
      bool operator==(const link_t &l) const { return l.id == id; }
      bool operator!=(const link_t &l) const { return l.id != id; }

    public:
      char *data() { return reinterpret_cast<char *>(this); }
      [[nodiscard]] const char *data() const { return reinterpret_cast<const char *>(this); }
      struct Hash
      {
        using hash_type = size_t;
        hash_type operator()(const self &lk) { return id_type::Hash{}(lk.id); }
      };

    };//! struct link_t

    /* class LinkArrayList
     *
     * */
    class LinkArrayList : private std::vector<link_t>
    {
    };//!class LinkArrayList

    /* class LinkLinkedList
     *
     * */
    class LinkLinkedList : private std::forward_list<link_t>
    {
    public:
      using value_type = link_t;
      using self       = LinkLinkedList;
      using super      = std::forward_list<value_type>;

    public:
      using super::forward_list;
      using super::operator=;
      using super::begin;
      using super::end;
    };//!class LinkLinkedList
  public:
    using value_type      = struct link_t;
    using array_list      = std::vector<value_type>;
    using linked_list     = std::forward_list<value_type>;
    using size_type       = size_t;
    using self            = LinkSet;
    using difference_type = long long;

  private:
    array_list *m_const_;
    array_list *m_static_;
    linked_list *m_dynamic_;

  public:
    LinkSet() : m_const_(new array_list()), m_static_(new array_list()), m_dynamic_(new linked_list()) {}
    LinkSet(NodeIdType id) : m_const_(nullptr), m_static_(nullptr), m_dynamic_(new linked_list())
    {
      io::Serialization(id.to_path() + "node.dat").deserialization(*this);
    }
    LinkSet(self &&)      = delete;
    LinkSet(const self &) = delete;
    ~LinkSet()
    {
      delete m_static_;
      delete m_dynamic_;
    }

  public:
  public:
    [[nodiscard]] bool empty() const { return m_static_->empty() && m_dynamic_->empty(); }

    void save(NodeIdType id) const { io::Serialization(id.to_path() + "node.dat").serialization(*this); }

    array_list::iterator normalBegin() { return m_static_->begin(); }
    array_list::iterator normalEnd() { return m_static_->end(); }
    array_list::iterator constBegin() { return m_const_->begin(); }
    array_list::iterator constEnd() { return m_const_->end(); }
    linked_list::iterator dynamicBegin() { return m_dynamic_->begin(); }
    linked_list::iterator dynamicEnd() { return m_dynamic_->end(); }

    [[nodiscard]] array_list::const_iterator normalBegin() const { return m_static_->begin(); }
    [[nodiscard]] array_list::const_iterator normalEnd() const { return m_static_->end(); }
    [[nodiscard]] array_list::const_iterator constBegin() const { return m_const_->begin(); }
    [[nodiscard]] array_list::const_iterator constEnd() const { return m_const_->end(); }
    [[nodiscard]] linked_list::const_iterator dynamicBegin() const { return m_dynamic_->cbegin(); }
    [[nodiscard]] linked_list::const_iterator dynamicEnd() const { return m_dynamic_->cend(); }

  public:
  private:
    void writeFunc(std::ofstream *ofPtr) const override
    {
      ofPtr->write(reinterpret_cast<const char *>(m_static_->size()), sizeof(size_type));
      ofPtr->write(reinterpret_cast<const char *>(m_const_size_), sizeof(size_type));
      for (const value_type &it: *m_static_) ofPtr->write(it.data(), sizeof(value_type));
    }

    void readFunc(std::ifstream *ifPtr) override
    {
      size_type tmpSize = 0;
      ifPtr->read(reinterpret_cast<char *>(tmpSize), sizeof(size_type));
      ifPtr->read(reinterpret_cast<char *>(m_const_size_), sizeof(size_type));
      m_static_ = new array_list(tmpSize);
      for (value_type &item: *m_static_) ifPtr->read(item.data(), sizeof(value_type));
    }
  };//! class LinkSet
public:
  using attribute_type = NodeAttributeEnum;
  using id_type        = NodeIdType;
  using link_set_type  = LinkSet;

private:
  id_type m_id_;
  attribute_type m_attr_;
  link_set_type m_set_;
};

}// namespace thinksystem::entity

#endif//THINK_THINKNODE_H
