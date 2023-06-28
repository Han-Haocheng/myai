//
// Created by HanHaocheng on 2023/05/12.
//

#ifndef THINKSYSTEM_THINKCORE_H
#define THINKSYSTEM_THINKCORE_H

#include <algorithm>
#include <cstddef>
#include <filesystem>
#include <forward_list>
#include <fstream>
#include <iostream>
#include <memory>
#include <numeric>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace fs = std::filesystem;

namespace think
{
#pragma pack(push, 2)

using id_type           = unsigned long long;
using size_type         = unsigned long long int;
using link_type         = unsigned short int;
using active_type       = unsigned long long int;
using weight_value_type = double;
enum class kind_enum
{
  NULL_TYPE,
  DYNAMIC_TYPE,
  CONST_TYPE,

  CONST_MEMORY,//思维
  CONST_VIDEO, //视觉
  CONST_AUDIO, //听觉
  CONST_ACTION,//行为
};

constexpr static const char *STR_NODE_LIB_PATH    = ".\\nodelib\\";
constexpr static const char *STR_NODE_FILE_NAME   = "id.dat";
static const constexpr id_type NUM_NODE_ID_MAX    = 0x1000000000000U;
static const constexpr id_type NUM_NODE_ID_MIN    = 0x0;
static const constexpr link_type NUM_LINK_VAL_MAX = 0xffff;
static const constexpr link_type NUM_LINK_VAL_MIN = 0x0;
constexpr const static char *ARRAY_TYPE_TO_STRING[]{
        "null", "dynamic", "static", "const", "const_memory", "const_video", "const_audio", "const_action",
};

[[nodiscard]] std::string GetIdPath(unsigned long long id)
{
  std::string res = STR_NODE_LIB_PATH;
  std::stringstream ss;

  for (size_t i = sizeof(unsigned long long); i > 0; --i)
  {
    unsigned char byte = (id >> (8 * (i - 1))) & 0xFF;
    ss << static_cast<int>(byte) << "\\";
  }

  res += ss.str();
  return res;
}

/// 区间，表示方法[cbegin,cend)
template<typename Ty>
struct range
{
  using value_type         = Ty;
  using size_type          = long long int;
  using reference          = Ty &;
  using const_reference    = const Ty &;

  value_type begin         = value_type();
  value_type end           = value_type();

  range()                  = default;
  range(range &&) noexcept = default;
  range(const range &)     = default;
  range(value_type begin, value_type end) : begin(begin), end(end) {}

  range &operator=(range &&) noexcept = default;
  range &operator=(const range &)     = default;

  // 获取区间长度
  [[nodiscard]] size_type size() const { return static_cast<size_type>(end > begin ? end - begin : begin - end); }

  // 判断区间是否为空
  [[nodiscard]] bool is_empty() const { return this->begin == this->end; }

  // 判断一个值是否在区间内
  [[nodiscard]] bool is_belong(const_reference linkVal) const
  {
    return end > begin ? linkVal < end && linkVal >= begin : linkVal >= end && linkVal < begin;
  }

  // 判断两个区间是否相交
  [[nodiscard]] bool is_intersect(const_reference other) const
  {
    return this->begin < other.cend && other.cbegin < this->end;
  }

  // 计算两个区间的交集
  [[nodiscard]] range intersection(const range &other) const
  {
    if (!is_intersect(other)) { return {}; }
    value_type rStart = std::max(this->m_begin_, other.begin);
    value_type rEnd   = std::min(this->m_end_, other.end);
    return {rStart, rEnd};
  }
  // 计算两个区间的并集
  [[nodiscard]] range merge(const range &other) const
  {
    if (!is_intersect(other)) { return {}; }
    value_type rStart = std::min(this->m_begin_, other.m_begin_);
    value_type rEnd   = std::max(this->m_end_, other.m_end_);
    return {rStart, rEnd};
  }
};

///
/// \tparam RightTy right m_val_
template<typename RightTy>
class weight
{
public:
  using weight_type = double;
  using right_type  = RightTy;
  static_assert(std::is_arithmetic_v<RightTy>, "right_t为数值类型");

  constexpr weight() = default;

  constexpr explicit weight(RightTy linkVal) : m_val_(linkVal) {}
  constexpr weight(weight_type linkVal) : m_val_(linkVal) {}
  weight &operator=(weight &&) noexcept = default;
  weight &operator=(const weight &)     = default;

  weight &operator=(weight_type rhs)
  {
    m_val_ = ::std::min(::std::max(rhs, m_min_), m_max_);
    return *this;
  }

  constexpr weight(weight_type linkVal, weight_type weightMax, weight_type weightMin)
  : m_val_(linkVal),
    m_max_(weightMax),
    m_min_(weightMin)
  {
  }
  weight operator-() { return weight{-linkVal()}; }

  friend constexpr right_type operator+(const weight &lhs, const right_type &rhs)
  {
    return std::clamp(lhs.m_val_ + rhs, lhs.m_min_, lhs.m_max_);
  }
  friend constexpr right_type operator-(const weight &lhs, const right_type &rhs)
  {
    return std::clamp(lhs.m_val_ - rhs, lhs.m_min_, lhs.m_max_);
  }
  friend constexpr right_type operator*(const weight &lhs, const right_type &rhs)
  {
    return std::clamp(lhs.m_val_ * rhs, lhs.m_min_, lhs.m_max_);
  }
  friend constexpr right_type operator/(const weight &lhs, const right_type &rhs)
  {
    return std::clamp(lhs.m_val_ / rhs, lhs.m_min_, lhs.m_max_);
  }

  constexpr weight &operator+=(const right_type &rhs) { return *this = *this + rhs; }
  constexpr weight &operator-=(const right_type &rhs) { return *this = *this - rhs; }
  constexpr weight &operator*=(const right_type &rhs) { return *this = *this * rhs; }
  constexpr weight &operator/=(const right_type &rhs) { return *this = *this / rhs; }

  [[nodiscard]] constexpr weight_type linkVal() const { return m_val_; }
  [[nodiscard]] constexpr weight_type &ref() { return m_val_; }
  void setMax(weight_type mMax) { m_max_ = mMax; }

private:
  weight_type m_val_ = 0.0;
  weight_type m_max_ = 1.0;
  weight_type m_min_ = -1.0;
};

struct link_t
{
  id_type id             = NUM_NODE_ID_MIN;
  link_type linkVal      = NUM_LINK_VAL_MIN;
  link_t()               = default;
  link_t(link_t &&)      = default;
  link_t(const link_t &) = default;
  link_t(id_type id, link_type linkVal) : id(id), linkVal(linkVal) {}

  link_t &operator=(link_t &&)      = default;
  link_t &operator=(const link_t &) = default;

  bool operator==(const link_t &rhs) const { return id == rhs.id; }
  bool operator!=(const link_t &rhs) const { return !(rhs == *this); }
  bool operator<(const link_t &rhs) const { return id < rhs.id; }
  bool operator>(const link_t &rhs) const { return rhs < *this; }
  bool operator<=(const link_t &rhs) const { return !(rhs < *this); }
  bool operator>=(const link_t &rhs) const { return !(*this < rhs); }
};

struct LinkHash
{
  size_t operator()(const link_t &rhs) const { return std::hash<id_type>{}(rhs.id); }
};

///\brief
class NodeIdAllocator
{
private:
  using IdRange = range<id_type>;
  std::forward_list<IdRange> m_cListUnassignedIDs;

  constexpr static const char *STR_UNASSIGNED_PATH = "id_allocator.dat";

public:
  explicit NodeIdAllocator(::size_t nodeCount)
  {
    if (fs::exists(STR_UNASSIGNED_PATH)) { _getUnassignedId(STR_UNASSIGNED_PATH); }
    else { m_cListUnassignedIDs.emplace_front(id_type{1}, id_type{nodeCount}); }
  }

  ~NodeIdAllocator() { _saveUnassignedId(STR_UNASSIGNED_PATH); }

  id_type allocate()
  {
    range<id_type> res = m_cListUnassignedIDs.front();
    id_type out        = res.begin++;

    if (m_cListUnassignedIDs.front().is_empty()) { m_cListUnassignedIDs.pop_front(); }
    return out;
  }

  void allocate(std::vector<id_type> &out, size_t count)
  {
    out.resize(count);
    auto beg = m_cListUnassignedIDs.begin(), bfBeg = m_cListUnassignedIDs.before_begin();
    while (beg != m_cListUnassignedIDs.end())
    {
      for (; beg->begin < beg->end; ++beg->begin)
      {
        if (count == 0) return;
        out[--count] = beg->begin;
      }
      m_cListUnassignedIDs.erase_after(bfBeg);
      beg = ++bfBeg;
    }
    throw std::overflow_error("Error:id allocate out of range");
  }

  /// 释放节点id
  /// \param in
  void release(const id_type &in)
  {
    auto itBeg = m_cListUnassignedIDs.begin();
    IdRange installRange(in, in + 1);
    if (in < itBeg->begin)
    {
      if (installRange.end == itBeg->begin) { --itBeg->begin; }
      else { m_cListUnassignedIDs.emplace_front(installRange); }
    }
    else
    {
      auto itBfBeg = m_cListUnassignedIDs.before_begin();

      for (; itBeg != m_cListUnassignedIDs.end(); ++itBeg, ++itBfBeg)
      {
        if (itBeg->begin > in)
        {
          if (itBfBeg->is_belong(in)) { throw std::runtime_error("Error:??????????"); }
          if (itBfBeg->end == installRange.begin)
          {
            if (installRange.end == itBeg->begin)
            {
              itBfBeg->end = itBeg->end;
              m_cListUnassignedIDs.erase_after(itBfBeg);
            }
            else { itBfBeg->end = installRange.end; }
          }
          else if (installRange.end == itBeg->begin) { itBeg->begin = installRange.begin; }
          else { auto newIter = m_cListUnassignedIDs.insert_after(itBfBeg, installRange); }
        }
      }

      if (itBfBeg->end == in) { ++itBfBeg->end; }
      else { m_cListUnassignedIDs.insert_after(itBfBeg, {in, in + 1}); }
    }
  }

  /// 释放节点id
  /// \param in
  void release(const std::vector<id_type> &in)
  {
    if (in.empty()) { return; }
    for (const auto item: in) { release(item); }
  }

private:
  void _getUnassignedId(const std::string &path)
  {
    std::ifstream file;
    IdRange tmp;
    file.open(path, std::ios::binary | std::ios::in);
    if (file.is_open())
    {
      while (file.read(reinterpret_cast<char *>(&tmp), sizeof(IdRange))) { m_cListUnassignedIDs.emplace_front(tmp); }
      file.close();
    }
  }
  void _saveUnassignedId(const std::string &path)
  {
    std::ofstream file(path, std::ios::binary | std::ios::out);
    if (file.is_open())
    {
      for (const auto &id: m_cListUnassignedIDs) { file.write(reinterpret_cast<const char *>(&id), sizeof(IdRange)); }
      file.close();
    }
  }
};// class NodeIdAllocator

class LinkList
{
public:
  using data_type       = link_t;
  using container       = std::vector<data_type>;
  using iterator        = container::iterator;
  using const_iterator  = container::const_iterator;
  using reference       = container::reference;
  using const_reference = container::const_reference;
  using state_type      = int;

private:
  enum state_kind : state_type
  {
    define_state = 0,
    //The list value do not have the same elements.
    is_Deduplicated = 1,
  };

private:
  state_type m_state_ = define_state;
  container m_datas_;

public:
  LinkList() : m_state_(define_state), m_datas_() {}
  explicit LinkList(size_t s) : m_datas_(s), m_state_(define_state){};

  /// use file to init list
  /// \param ifs data file stream
  /// \param pos data pos in file
  /// \param count data count
  LinkList(std::ifstream &ifs, std::ifstream::pos_type pos, size_t count) : m_state_(define_state), m_datas_(count)
  {
    ifs.seekg(pos);
    for (auto &item: m_datas_) { ifs.read(reinterpret_cast<char *>(&item), sizeof(data_type)); }
  }
  LinkList(LinkList &&)                 = default;
  LinkList(const LinkList &)            = default;
  LinkList &operator=(LinkList &&)      = default;
  LinkList &operator=(const LinkList &) = default;
  LinkList &operator+=(int linkVal)
  {
    for (link_t &lk: *this) { lk.linkVal = std::min<int>(NUM_LINK_VAL_MAX, lk.linkVal + linkVal); }
    return *this;
  }
  LinkList &operator-=(int linkVal)
  {
    for (link_t &lk: *this) { lk.linkVal = std::max<int>(NUM_LINK_VAL_MIN, lk.linkVal - linkVal); }
    return *this;
  }
  LinkList &operator*=(const weight<link_type> &weight)
  {
    for (link_t &lk: *this) { lk.linkVal = weight * lk.linkVal; }
    return *this;
  }
  LinkList operator*(const weight<link_type> &weight)
  {
    LinkList res = *this;
    return res *= weight;
  }
  LinkList operator-(int linkVal)
  {
    LinkList res = *this;
    return res -= linkVal;
  }
  LinkList operator+(int linkVal)
  {
    LinkList res = *this;
    return res += linkVal;
  }
  reference operator[](size_t pos) { return m_datas_[pos]; }

  [[nodiscard]] size_t size() const { return m_datas_.size(); }
  iterator begin()
  {
    m_state_ &= ~is_Deduplicated;
    return m_datas_.begin();
  }
  iterator end()
  {
    m_state_ &= ~is_Deduplicated;
    return m_datas_.end();
  }
  [[nodiscard]] const_iterator begin() const { return m_datas_.begin(); }
  [[nodiscard]] const_iterator end() const { return m_datas_.end(); }

  /// \time   other.size + this.size
  /// \param  other
  void insert(LinkList &other)
  {
    m_state_ &= ~is_Deduplicated;
    m_datas_.insert(m_datas_.end(), other.m_datas_.begin(), other.m_datas_.end());
  }

  /// \time   other.size + this.size
  /// \param  other
  void insert(LinkList &&other)
  {
    m_state_ &= ~is_Deduplicated;
    m_datas_.insert(m_datas_.end(), other.m_datas_.begin(), other.m_datas_.end());
  }

  template<class... ValTy>
  void emplace(ValTy &&...Val)
  {
    m_state_ &= ~is_Deduplicated;
    m_datas_.emplace_back(std::forward<ValTy>(Val)...);
  }

  /// @brief Remove duplicate values from your data,
  void deduplicate()
  {
    if (m_state_ & is_Deduplicated) { return; }
    std::unordered_set<link_t, LinkHash> s;
    s.reserve(m_datas_.size());
    for (link_t &lstLink: *this)
    {
      auto [setLink, bIsAdd] = s.emplace(lstLink);
      if (!bIsAdd) { const_cast<link_type &>(setLink->linkVal) += lstLink.linkVal; }
    }
    m_datas_.assign(s.begin(), s.end());
    m_state_ |= is_Deduplicated;
  }

  void saveIntoFile(std::ofstream &ofs)
  {
    for (const_reference item: m_datas_) ofs.write(reinterpret_cast<const char *>(&item), sizeof(data_type));
  }
};//! class LinkList

struct LinkArray
{
  LinkList constLink;
  LinkList staticLink;
  LinkList dynamicLink;
  LinkArray() = default;
  LinkArray(LinkArray &&oth) noexcept
  : constLink(std::move(oth.constLink)),
    staticLink(std::move(oth.staticLink)),
    dynamicLink(std::move(oth.dynamicLink))
  {
  }
};

class NodeInfo : public LinkArray
{
  using state_type = unsigned int;
  using size_type  = size_t;

private:
  struct ConstFileInfo
  {
    size_type const_count  = 0;
    size_type static_count = 0;
  };
  enum state_kind : state_type
  {
    define_state = 0,
    IS_STABLE    = 1,
  };

private:
  state_type m_state_ = define_state;
  size_type m_iActNum = 0;

  kind_enum m_type_   = kind_enum::NULL_TYPE;

public:
  active_type actVal = 0;

public:
  NodeInfo() = default;
  explicit NodeInfo(kind_enum nodetype) : LinkArray(), m_type_(nodetype), m_state_(define_state) {}
  explicit NodeInfo(id_type id) : LinkArray(), m_type_(kind_enum::NULL_TYPE), m_state_(define_state)
  {
    std::string path = GetIdPath(id) + STR_NODE_FILE_NAME;
    if (!fs::exists(path)) { return; }

    std::ifstream file{path, std::ios ::binary | std::ios ::in};
    if (!file.is_open()) throw;
    ConstFileInfo tmpInfo{};
    file.read(reinterpret_cast<char *>(&tmpInfo), sizeof(ConstFileInfo));
    constLink  = LinkList(file, file.tellg(), tmpInfo.const_count);
    staticLink = LinkList(file, file.tellg(), tmpInfo.static_count);
    file.close();
  }

  NodeInfo(NodeInfo &&oth) = default;

  /// \b Turn dynamic links into static links.
  /// \param dySaveWeight Save to BoundedValue.
  void stableLink(const weight<link_type> &dySaveWeight)
  {
    if (m_state_ & IS_STABLE) { return; }
    dynamicLink *= dySaveWeight;
    staticLink.insert(dynamicLink);
    staticLink.deduplicate();
    constLink.deduplicate();
    m_state_ |= IS_STABLE;
  }

  void act(const weight<active_type> &actWeight, active_type actStand, LinkArray &out)
  {
    if (actStand > actVal) { return; }
    ++m_iActNum;
    weight<link_type> saveVal{static_cast<double>(actWeight * (actVal / NUM_LINK_VAL_MAX))};
    out.constLink.insert(constLink * saveVal);
    out.staticLink.insert(staticLink * saveVal);
    out.dynamicLink.insert(dynamicLink * saveVal);
  };

  bool writeNodeInfo(id_type id) noexcept(false)
  {
    std::string path = GetIdPath(id);
    if (!fs::exists(path)) { fs::create_directories(path); }

    std::ofstream file{path + STR_NODE_FILE_NAME, std::ios ::binary | std::ios ::out};
    if (!file.is_open()) throw;

    size_t tmp = constLink.size();
    file.write(reinterpret_cast<const char *>(&tmp), sizeof(size_t));
    tmp = staticLink.size();
    file.write(reinterpret_cast<const char *>(&tmp), sizeof(size_t));
    constLink.saveIntoFile(file);
    staticLink.saveIntoFile(file);
    file.close();
    return true;
  }

};// class NodeInfo

using node_type = std::pair<const id_type, std::shared_ptr<NodeInfo>>;//think node type

#pragma pack(pop)

}// namespace think

#endif//THINKSYSTEM_THINKCORE_H
