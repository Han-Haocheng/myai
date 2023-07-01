//
// Created by HanHaocheng on 2023/06/18.
//

#ifndef THINK_THINKNODE_H
#define THINK_THINKNODE_H
#include <forward_list>
#include <functional>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "../io/Serialization.h"
#include "../utils/BoundedValue.h"
#include "../utils/Log.h"

namespace thinksystem::entity
{

using attribute_t = uint32_t;
using weight_t    = utils::BoundedValue<double, -10, 10>;
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
  class NodeIdType : public utils::BoundedValue<unsigned long long, 0x0000'0000'0000ULL, 0xffff'ffff'ffffULL>
  {
    using self  = NodeIdType;
    using super = utils::BoundedValue<unsigned long long, 0x0000'0000'0000ULL, 0xffff'ffff'ffffULL>;

  public:
    using super::super;
    using super::operator=;

  public:
    NodeIdType() : super(super::MIN) {}
    NodeIdType(self &&)      = default;
    NodeIdType(const self &) = default;

  public:
    self &operator=(self &&)      = default;
    self &operator=(const self &) = default;

  public:
    //转换为物理位置
    [[nodiscard]] std::string to_path() const
    {
      std::stringstream ss{".\\NodeStruct\\"};
      auto tmpVal = val();
      for (size_t i = 0; i < sizeof(tmpVal); ++i)
      {
        unsigned char byte = (tmpVal >>= 8) & 0xFF;
        ss << static_cast<int>(byte) << "\\";
      }
      return ss.str();
    }

    struct Hash
    {
      using hash_type = size_t;
      hash_type operator()(const self &id) { return std::hash<hash_type>{}(id.val()); }
    };

  };//! class NodeIdType

  /*class LinkSet
   * 链接集
   * */
  class LinkSet : private io::SerializationInterface
  {
  public:
    /* struct LinkType
     * 链接结构
     * */
    using link_type = class LinkType
    {
      using self = LinkType;

    public:
      using id_type    = NodeIdType;
      using value_type = utils::BoundedValue<std::uint32_t, 0x0000'0000U, 0xffff'ffffU>;

    public:
      id_type id;
      value_type linkVal;

    public:
      LinkType() : id(), linkVal(value_type::MIN) {}
      LinkType(const id_type &id, const value_type &linkVal) : id(id), linkVal(linkVal) {}
      LinkType(const std::pair<id_type, value_type> &pr) : id(pr.first), linkVal(pr.second) {}

    public:
      bool operator==(const LinkType &l) const { return l.id == id; }
      bool operator!=(const LinkType &l) const { return l.id != id; }
      self &operator=(const std::pair<id_type, value_type> &pr)
      {
        id      = pr.first;
        linkVal = pr.second;
      }

    public:
      char *data()
      {
        sizeof(*this);
        return reinterpret_cast<char *>(this);
      }
      [[nodiscard]] const char *data() const { return reinterpret_cast<const char *>(this); }
      struct Hash
      {
        using hash_type = size_t;
        hash_type operator()(const self &lk) { return id_type::Hash{}(lk.id); }
      };

    };//! struct LinkType

    /* class LinkListBase
    * 链接列表基类
    */
    class LinkListBase
    {
      using self = LinkListBase;

    public:
      using value_type      = LinkType;
      using reference       = LinkType &;
      using const_reference = const LinkType &;
      using size_type       = size_t;
      using stand_type      = value_type::value_type;

    public:
      virtual ~LinkListBase() {}

    public:
      virtual self &operator+=(const stand_type &) = 0;
      virtual self &operator-=(const stand_type &) = 0;
      virtual self &operator*=(const weight_t &)   = 0;

    public:
      virtual bool empty() const     = 0;
      virtual size_type size() const = 0;

    public:
      virtual void for_each(std::function<void(value_type &)> func) = 0;
      virtual void insert(std::initializer_list<value_type> _il)    = 0;
      virtual void emplace(value_type &&)                           = 0;
      virtual void emplace(const value_type &)                      = 0;
      virtual bool remove_replication()                             = 0;
      virtual bool remove_by_standard(const stand_type &_stand)     = 0;

    };//! class LinkListBase

    /* class LinkArrayList
     * 链接数组
     * */
    class LinkArrayList : public LinkListBase
    {
      using list_type = std::vector<LinkType>;
      using self      = LinkArrayList;
      using super     = LinkListBase;

    private:
      list_type m_list_;

    public:
      // 通过 LinkListBase 继承
      virtual self &operator+=(const stand_type &) override {}

      virtual self &operator-=(const stand_type &) override {}

      virtual self &operator*=(const weight_t &) override {}

    public:
      virtual size_type size() const override { return m_list_.size(); }
      bool empty() const { return m_list_.empty(); }

    public:
      virtual void for_each(std::function<void(value_type &)> func) override
      {
        std::for_each(m_list_.begin(), m_list_.end(), func);
      }

      virtual void insert(std::initializer_list<value_type> _il) override { m_list_.assign(_il); }

      virtual void emplace(value_type &&val) override { m_list_.emplace_back(val); }

      virtual void emplace(const value_type &val) override { m_list_.emplace_back(val); }

      virtual bool remove_replication() override
      {
        if (empty()) return false;
        std::unordered_map<id_type, link_type> tmpMap{m_list_.size()};
        for (const auto &val: m_list_) { tmpMap.emplace(val);
        }
        
      }

      virtual bool remove_by_standard(const stand_type &_stand) override {}

    };//!class LinkArrayList

    /* class LinkLinkedList
     * 链接链表
     * */
    class LinkLinkedList : public LinkListBase
    {
      using list_type = std::forward_list<LinkType>;
      using self      = LinkLinkedList;
      using super     = LinkListBase;

    public:
      using value_type = LinkType;

      using size_type  = size_t;

    private:
      list_type m_list_;
      size_type m_size_;

    public:
    };//!class LinkLinkedList
  public:
    using value_type      = struct LinkType;
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
