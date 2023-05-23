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
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "base_type/exception.h"

namespace fs = std::filesystem;

#define GetValInRange(min_val, max_val, val) (std::max(min_val, std::min(max_val, val)))
/// 区间，表示方法[begin,end)
template<typename Ty>
class Range
{
  using data_type       = Ty;
  using reference       = Ty &;
  using size_type       = ptrdiff_t;
  using const_reference = const Ty &;

private:
  data_type m_begin_;
  data_type m_end_;

public:
  Range()                  = default;
  Range(Range &&) noexcept = default;
  Range(const Range &)     = default;
  Range(data_type begin, data_type end) : m_begin_(begin), m_end_(end) {}

  Range &operator=(Range &&) noexcept = default;
  Range &operator=(const Range &)     = default;

  reference begin() { return m_begin_; }
  reference end() { return m_end_; }

  // 判断区间是否为空
  [[nodiscard]] bool is_empty() const { return this->m_begin_ == this->m_end_; }

  // 获取区间长度
  [[nodiscard]] size_type length() const { return this->m_end_ - this->m_begin_; }

  // 判断一个值是否在区间内
  [[nodiscard]] bool contains(const_reference value) const { return value >= this->m_begin_ && value < this->m_end_; }

  // 判断两个区间是否相交
  [[nodiscard]] bool intersects(const Range &other) const
  {
    return this->m_begin_ < other.m_end_ && other.m_begin_ < this->m_end_;
  }

  // 计算两个区间的交集
  [[nodiscard]] Range intersection(const Range &other) const
  {
    int start = std::max(this->m_begin_, other.m_begin_);
    int end   = std::min(this->m_end_, other.m_end_);
    return {start, end};
  }

  // 计算两个区间的并集
  [[nodiscard]] Range merge(const Range &other) const
  {
    int start = std::min(this->m_begin_, other.m_begin_);
    int end   = std::max(this->m_end_, other.m_end_);
    return {start, end};
  }
};

namespace think
{
constexpr static const char *STR_NODE_LIB_PATH  = "./nodelib/";
constexpr static const char *STR_NODE_FILE_NAME = "id.dat";

using node_id                                   = size_t;
using C_NODE_ID                                 = const node_id;
using UP_NODE_ID                                = std::unique_ptr<node_id>;
using UPC_NODE_ID                               = std::unique_ptr<C_NODE_ID>;

static const constexpr node_id NUM_NODE_ID_MAX  = 0x1000000000000U;
static const constexpr node_id NUM_NODE_ID_MIN  = 0x0;

[[nodiscard]] std::string GetNodeParentPath(node_id id)
{
  std::string res = STR_NODE_LIB_PATH;
  for (size_t i = 0U; i < sizeof(node_id); ++i)
  {
    res += std::to_string(id & 0xff) + "/";
    id >>= 8;
  }
  return res;
}

[[nodiscard]] std::string GetNodePath(node_id id) { return GetNodeParentPath(id) + STR_NODE_FILE_NAME; }

using active_t                                   = size_t;

using link_val                                   = uint16_t;

static const constexpr link_val NUM_LINK_VAL_MAX = 0xffff;
static const constexpr link_val NUM_LINK_VAL_MIN = 0x0;

template<typename right_t>
class weight_t
{
public:
  using weight_type = double;
  using right_type  = right_t;
  static_assert(std::is_arithmetic_v<right_t>, "right_t为数值类型");

  constexpr weight_t() = default;
  constexpr weight_t(weight_type val) : m_val_(val) {}

  weight_t &operator=(weight_t &&) noexcept = default;
  weight_t &operator=(const weight_t &)     = default;

  weight_t &operator=(weight_type rhs)
  {
    m_val_ = ::std::min(::std::max(rhs, m_min_), m_max_);
    return *this;
  }

  constexpr weight_t(weight_type val, weight_type weightMax, weight_type weightMin)
  : m_val_(val),
    m_max_(weightMax),
    m_min_(weightMin)
  {
  }

  friend constexpr right_type operator+(const weight_t &lhs, const right_type &rhs)
  {
    return std::clamp(lhs.m_val_ + rhs, lhs.m_min_, lhs.m_max_);
  }
  friend constexpr right_type operator-(const weight_t &lhs, const right_type &rhs)
  {
    return std::clamp(lhs.m_val_ - rhs, lhs.m_min_, lhs.m_max_);
  }
  friend constexpr right_type operator*(const weight_t &lhs, const right_type &rhs)
  {
    return std::clamp(lhs.m_val_ * rhs, lhs.m_min_, lhs.m_max_);
  }
  friend constexpr right_type operator/(const weight_t &lhs, const right_type &rhs)
  {
    return std::clamp(lhs.m_val_ / rhs, lhs.m_min_, lhs.m_max_);
  }

  constexpr weight_t &operator+=(const right_type &rhs) { return *this = *this + rhs; }
  constexpr weight_t &operator-=(const right_type &rhs) { return *this = *this - rhs; }
  constexpr weight_t &operator*=(const right_type &rhs) { return *this = *this * rhs; }
  constexpr weight_t &operator/=(const right_type &rhs) { return *this = *this / rhs; }

  [[nodiscard]] constexpr weight_type val() const { return m_val_; }
  void setMax(weight_type mMax) { m_max_ = mMax; }
  void setMin(weight_type mMin) { m_min_ = mMin; }

private:
  weight_type m_val_ = 0.0;
  weight_type m_max_ = 1.0;
  weight_type m_min_ = 0.0;
};

using active_t = size_t;
using ActFunc  = void (*)(active_t);

enum class ENodeType
{
  NULL_TYPE,
  DYNAMIC_TYPE,
  STATIC_TYPE,
  CONST_TYPE,

  CONST_MEMORY,//思维
  CONST_VIDEO, //视觉
  CONST_AUDIO, //听觉
  CONST_ACTION,//行为
};

constexpr const static char *ARRAY_TYPE_TO_STRING[]{
        "null", "dynamic", "static", "const", "const_memory", "const_video", "const_audio", "const_action",
};

#pragma pack(push, 2)
struct link_t
{
  node_id id             = NUM_NODE_ID_MIN;
  link_val linkVal       = NUM_LINK_VAL_MIN;
  link_t()               = default;
  link_t(link_t &&)      = default;
  link_t(const link_t &) = default;
  link_t(node_id id, link_val linkVal) : id(id), linkVal(linkVal) {}

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
  size_t operator()(const link_t &rhs) const { return std::hash<node_id>{}(rhs.id); }
};

#pragma pack(pop)
///\brief ???id??????
class NodeIdAllocator
{
private:
  using IdRange = Range<node_id>;
  std::forward_list<IdRange> m_cListUnassignedIDs;

  constexpr static const char *STR_UNASSIGNED_PATH = "id_allocator.dat";

public:
  explicit NodeIdAllocator(size_t nodeCount);

  ~NodeIdAllocator();

  /// 申请节点id
  /// \param out
  node_id allocate();

  /// 申请节点id
  /// \param out
  /// \param count
  void allocate(std::vector<node_id> &out, size_t count);

  /// 释放节点id
  /// \param in
  void release(const node_id &in);

  /// 释放节点id
  /// \param in
  void release(const std::vector<node_id> &in);

private:
  void _getUnassignedId(const std::string &path);
  void _saveUnassignedId(const std::string &path);
};// class NodeIdAllocator

/*
 * */
class LinkList
{
public:
  using data_type       = link_t;
  using container       = std::vector<data_type>;
  using iterator        = container::iterator;
  using const_iterator  = container::const_iterator;
  using pointer         = container::pointer;
  using reference       = container::reference;
  using const_reference = container::const_reference;
  using state           = int;

private:
  enum state_type : state
  {
    is_deduplication,

  };

private:
  container m_datas_;
  state m_state_;

public:
  LinkList() : m_state_(0), m_datas_() {}
  LinkList(std::ifstream &ifs, std::ifstream::pos_type pos, size_t count) : m_state_(0), m_datas_(count)
  {
    ifs.seekg(pos);
    ifs.read(reinterpret_cast<char *>(m_datas_.data()), static_cast<std::streamsize>(sizeof(data_type) * count));
  }
  LinkList(LinkList &&)                 = default;
  LinkList(const LinkList &)            = default;
  LinkList &operator=(LinkList &&)      = default;
  LinkList &operator=(const LinkList &) = default;
  LinkList &operator+=(int val);
  LinkList &operator-=(int val);
  LinkList &operator*=(const weight_t<link_val> &weight);
  LinkList operator*(const weight_t<link_val> &weight);
  LinkList operator-(int val);
  LinkList operator+(int val);

  [[nodiscard]] size_t size() const { return m_datas_.size(); }
  iterator begin()
  {
    m_state_ &= ~is_deduplication;
    return m_datas_.begin();
  }
  iterator end()
  {
    m_state_ &= ~is_deduplication;
    return m_datas_.end();
  }
  [[nodiscard]] const_iterator begin() const { return m_datas_.begin(); }
  [[nodiscard]] const_iterator end() const { return m_datas_.end(); }

  /// \time   other.size + this.size
  /// \param  other
  void insert(LinkList &other)
  {
    m_state_ &= ~is_deduplication;
    m_datas_.insert(m_datas_.end(), other.m_datas_.begin(), other.m_datas_.end());
  }

  /// \time   other.size + this.size
  /// \param  other
  void insert(LinkList &&other)
  {
    m_state_ &= ~is_deduplication;
    m_datas_.insert(m_datas_.end(), other.m_datas_.begin(), other.m_datas_.end());
  }

  /// \time   1
  /// \param  other
  void emplace(data_type &other)
  {
    m_state_ &= ~is_deduplication;
    m_datas_.emplace_back(other);
  }

  /// \time   1
  /// \param  other
  void emplace(data_type &&other)
  {
    m_state_ &= ~is_deduplication;
    m_datas_.emplace_back(other);
  }

  template<class... ValTy>
  void emplace(ValTy &&...Val)
  {
    m_state_ &= ~is_deduplication;
    m_datas_.emplace_back(std::forward<ValTy>(Val)...);
  }

  void erase(link_val standardValue);

  /// @brief Remove duplicate values from your data,
  void deduplicate();

  void saveIntoFile(std::ofstream &ofs)
  {
    ofs.seekp(std::ios::end);
    for (const_reference item: m_datas_) ofs.write(reinterpret_cast<const char *>(&item), sizeof(data_type));
  }
};//! class LinkList

struct LinkArray
{
  LinkList m_lsConstLink;
  LinkList m_lsStaticLink;
  LinkList m_lsDynamicLink;
  LinkArray() = default;
  LinkArray(LinkArray &&oth)
  : m_lsConstLink(std::move(oth.m_lsConstLink)),
    m_lsStaticLink(std::move(oth.m_lsStaticLink)),
    m_lsDynamicLink(std::move(oth.m_lsDynamicLink))
  {
  }
};

class NodeInfo : public LinkArray
{
private:
  struct ConstFileInfo
  {
    size_t const_count;
    size_t static_count;
  };
  enum node_state : unsigned int
  {
    IS_INITIALIZED = 0b0001,
    IS_STABLE      = 0b0010,
    IS_DUPLICATED  = 0b0100,
  };

private:
  unsigned int m_state_  = 0;
  unsigned int m_iActNum = 0;

  ENodeType m_type_      = ENodeType::NULL_TYPE;

public:
  active_t actVal = 0;

public:
  NodeInfo() = default;
  explicit NodeInfo(node_id id);

  explicit NodeInfo(ENodeType nodetype);

  NodeInfo(NodeInfo &&oth) = default;
  [[nodiscard]] ENodeType nodeType() const { return m_type_; }

  /// \b Turn dynamic links into static links.
  /// \param dySaveWeight Save to weight.
  void stableLink(const weight_t<link_val> &dySaveWeight);

  bool initNodeInfo(node_id id) noexcept(false);

  bool writeNodeInfo(node_id id) noexcept(false);

  void act(const weight_t<active_t> &actWeight, active_t actStand, LinkArray &out)
  {
    if (actStand > actVal) { return; }
    ++m_iActNum;
    weight_t<link_val> saveVal{static_cast<double>(actWeight * (actVal / NUM_LINK_VAL_MAX))};
    out.m_lsDynamicLink.insert(m_lsDynamicLink * saveVal);
    out.m_lsStaticLink.insert(m_lsStaticLink * saveVal);
    out.m_lsConstLink.insert(m_lsConstLink * saveVal);
  };

};// class NodeInfo

using CLINKGROUP   = const NodeInfo;
using UP_NODE_INFO = std::unique_ptr<NodeInfo>;
using SP_NODE_INFO = std::shared_ptr<NodeInfo>;
using WPLINKGROUP  = std::weak_ptr<NodeInfo>;

using node_type    = std::pair<const node_id, std::shared_ptr<NodeInfo>>;//???
using node_list    = std::vector<node_type>;

}// namespace think

namespace think
{
NodeIdAllocator::NodeIdAllocator(size_t nodeCount)
{
  if (fs::exists(STR_UNASSIGNED_PATH)) { _getUnassignedId(STR_UNASSIGNED_PATH); }
  else { m_cListUnassignedIDs.emplace_front(node_id{1}, node_id{nodeCount}); }
}
NodeIdAllocator::~NodeIdAllocator() { _saveUnassignedId(STR_UNASSIGNED_PATH); }
node_id NodeIdAllocator::allocate()
{
  node_id out = m_cListUnassignedIDs.front().begin()++;
  if (m_cListUnassignedIDs.front().is_empty()) { m_cListUnassignedIDs.pop_front(); }
  return out;
}

void NodeIdAllocator::allocate(std::vector<node_id> &out, size_t count)
{
  if (count == 0) { return; }
  out.reserve(count);
  auto beg = m_cListUnassignedIDs.begin(), bfBeg = m_cListUnassignedIDs.before_begin();
  while (beg != m_cListUnassignedIDs.end())
  {
    for (; beg->begin() < beg->end(); ++beg->begin())
    {
      out.emplace_back(beg->begin()++);
      if (--count == 0) { return; }
    }
    m_cListUnassignedIDs.erase_after(bfBeg);
    beg = ++bfBeg;
  }
  throw std::overflow_error("");
}

void NodeIdAllocator::release(const node_id &in)
{
  auto itBeg = m_cListUnassignedIDs.begin();
  IdRange installRange(in, in + 1);
  if (in < itBeg->begin())
  {
    if (installRange.end() == itBeg->begin()) { --itBeg->begin(); }
    else { m_cListUnassignedIDs.emplace_front(installRange); }
  }
  else
  {
    auto itBfBeg = m_cListUnassignedIDs.before_begin();

    for (; itBeg != m_cListUnassignedIDs.end(); ++itBeg, ++itBfBeg)
    {
      if (itBeg->begin() > in)
      {
        if (itBfBeg->contains(in)) { throw std::runtime_error("Error:??????????"); }
        if (itBfBeg->end() == installRange.begin())
        {
          if (installRange.end() == itBeg->begin())
          {
            itBfBeg->end() = itBeg->end();
            m_cListUnassignedIDs.erase_after(itBfBeg);
          }
          else { itBfBeg->end() = installRange.end(); }
        }
        else if (installRange.end() == itBeg->begin()) { itBeg->begin() = installRange.begin(); }
        else { auto newIter = m_cListUnassignedIDs.insert_after(itBfBeg, installRange); }
      }
    }

    if (itBfBeg->end() == in) { ++itBfBeg->end(); }
    else { m_cListUnassignedIDs.insert_after(itBfBeg, {in, in + 1}); }
  }
}
void NodeIdAllocator::release(const std::vector<node_id> &in)
{
  if (in.empty()) { return; }
  for (const auto item: in) { release(item); }
}
void NodeIdAllocator::_getUnassignedId(const std::string &path)
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
void NodeIdAllocator::_saveUnassignedId(const std::string &path)
{
  std::ofstream file(path, std::ios::binary | std::ios::out);
  if (file.is_open())
  {
    for (const auto &id: m_cListUnassignedIDs) { file.write(reinterpret_cast<const char *>(&id), sizeof(IdRange)); }
    file.close();
  }
}

void LinkList::deduplicate()
{
  std::unordered_set<link_t, LinkHash> s;
  s.reserve(m_datas_.size());
  for (link_t &lstLink: *this)
  {
    auto [setLink, bIsAdd] = s.emplace(lstLink);
    if (!bIsAdd) { const_cast<link_val &>(setLink->linkVal) += lstLink.linkVal; }
  }
  m_datas_.assign(s.begin(), s.end());
}

void LinkList::erase(link_val standardValue)
{
  if (m_datas_.empty()) { return; }
  auto condition = [&](const link_t &item) -> bool { return item.linkVal < standardValue; };
  auto res       = std::remove_if(this->begin(), this->end(), condition);
  m_datas_.erase(res);
}

LinkList LinkList::operator+(int val)
{
  LinkList res = *this;
  return res += val;
}
LinkList LinkList::operator-(int val)
{
  LinkList res = *this;
  return res -= val;
}
LinkList LinkList::operator*(const weight_t<link_val> &weight)
{
  LinkList res = *this;
  return res *= weight;
}
LinkList &LinkList::operator*=(const weight_t<link_val> &weight)
{
  for (link_t &lk: *this) { lk.linkVal = weight * lk.linkVal; }
  return *this;
}
LinkList &LinkList::operator-=(int val)
{
  for (link_t &lk: *this) { lk.linkVal = std::max<int>(NUM_LINK_VAL_MIN, lk.linkVal - val); }
  return *this;
}
LinkList &LinkList::operator+=(int val)
{
  for (link_t &lk: *this) { lk.linkVal = std::min<int>(NUM_LINK_VAL_MAX, lk.linkVal + val); }
  return *this;
}

NodeInfo::NodeInfo(node_id id)
{
  if (!initNodeInfo(id)) {}
}

NodeInfo::NodeInfo(ENodeType nodetype) : LinkArray(), m_type_(nodetype) {}

bool NodeInfo::writeNodeInfo(node_id id)
{
  if (m_state_ ^ IS_INITIALIZED) { return false; }
  std::string path = GetNodeParentPath(id);
  if (!fs::exists(path)) { fs::create_directories(path); }

  std::ofstream file{path + STR_NODE_FILE_NAME, std::ios ::binary | std::ios ::out};
  if (!file.is_open())
  {
    throw think_error{"д?????????????????δ???????????", std::make_error_code(std::errc::no_such_file_or_directory)};
  }

  ConstFileInfo tmpInfo{
          m_lsConstLink.size(),
          m_lsStaticLink.size(),
  };

  file.write(reinterpret_cast<const char *>(&tmpInfo), sizeof(ConstFileInfo));
  m_lsConstLink.saveIntoFile(file);
  m_lsStaticLink.saveIntoFile(file);
  file.close();
  return true;
}
bool NodeInfo::initNodeInfo(node_id id) noexcept(false)
{
  if (m_state_ & IS_INITIALIZED) { return false; }
  std::string path = GetNodeParentPath(id) + STR_NODE_FILE_NAME;
  if (!fs::exists(path)) return false;

  std::ifstream file{path, std::ios ::binary | std::ios ::in};
  if (!file.is_open()) throw think_error{"", std::make_error_code(std::errc::no_such_file_or_directory)};
  ConstFileInfo tmpInfo{};
  file.read(reinterpret_cast<char *>(&tmpInfo), sizeof(ConstFileInfo));
  m_lsConstLink  = LinkList(file, file.tellg(), tmpInfo.const_count);
  m_lsStaticLink = LinkList(file, file.tellg(), tmpInfo.static_count);
  file.close();
  m_state_ |= IS_INITIALIZED;
  return true;
}

void NodeInfo::stableLink(const weight_t<link_val> &dySaveWeight)
{
  if (m_state_ & IS_STABLE) { return; }
  m_lsDynamicLink *= dySaveWeight;
  m_lsStaticLink.insert(m_lsDynamicLink);
  m_lsStaticLink.deduplicate();
  if (m_state_ ^ IS_DUPLICATED)
  {
    m_lsConstLink.deduplicate();
    m_state_ |= IS_DUPLICATED;
  }
  m_state_ |= IS_STABLE;
}

}// namespace think

#endif//THINKSYSTEM_THINKCORE_H
