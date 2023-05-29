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

  void resetBegin(const_reference avg) { m_begin_ = avg; }
  void resetEnd(const_reference avg) { m_end_ = avg; }

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
#pragma pack(push, 2)

constexpr static const char *STR_NODE_LIB_PATH  = ".\\nodelib\\";
constexpr static const char *STR_NODE_FILE_NAME = "id.dat";

using node_id                                   = unsigned long long;

static const constexpr node_id NUM_NODE_ID_MAX  = 0x1000000000000U;
static const constexpr node_id NUM_NODE_ID_MIN  = 0x0;

[[nodiscard]] std::string GetNodeParentPath(unsigned long long id)
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

  constexpr explicit weight_t(right_t val) : m_val_(val) {}
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
  weight_t operator-() { return weight_t{-val()}; }

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
  [[nodiscard]] constexpr weight_type &ref() { return m_val_; }
  void setMax(weight_type mMax) { m_max_ = mMax; }

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
  CONST_TYPE,

  CONST_MEMORY,//思维
  CONST_VIDEO, //视觉
  CONST_AUDIO, //听觉
  CONST_ACTION,//行为
};

constexpr const static char *ARRAY_TYPE_TO_STRING[]{
        "null", "dynamic", "static", "const", "const_memory", "const_video", "const_audio", "const_action",
};

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

///\brief ???id??????
class NodeIdAllocator
{
private:
  using IdRange = Range<node_id>;
  std::forward_list<IdRange> m_cListUnassignedIDs;

  constexpr static const char *STR_UNASSIGNED_PATH = "id_allocator.dat";

public:
  explicit NodeIdAllocator(::size_t nodeCount)
  {
    if (fs::exists(STR_UNASSIGNED_PATH)) { _getUnassignedId(STR_UNASSIGNED_PATH); }
    else { m_cListUnassignedIDs.emplace_front(node_id{1}, node_id{nodeCount}); }
  }

  ~NodeIdAllocator() { _saveUnassignedId(STR_UNASSIGNED_PATH); }

  node_id allocate()
  {
    auto &res   = m_cListUnassignedIDs.front();
    node_id out = res.begin()++;

    if (m_cListUnassignedIDs.front().is_empty()) { m_cListUnassignedIDs.pop_front(); }
    return out;
  }

  void allocate(std::vector<node_id> &out, size_t count)
  {
    out.resize(count);
    auto beg = m_cListUnassignedIDs.begin(), bfBeg = m_cListUnassignedIDs.before_begin();
    while (beg != m_cListUnassignedIDs.end())
    {
      for (node_id i = beg->begin(); i < beg->end(); i++)
      {
        if (count == 0)
        {
          beg->resetBegin(i);
          return;
        }
        out[--count] = i;
      }
      m_cListUnassignedIDs.erase_after(bfBeg);
      beg = ++bfBeg;
    }
    throw std::overflow_error("");
  }

  /// 释放节点id
  /// \param in
  void release(const node_id &in)
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

  /// 释放节点id
  /// \param in
  void release(const std::vector<node_id> &in)
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
  LinkList(std::ifstream *ifs, std::ifstream::pos_type pos, size_t count) : m_state_(define_state), m_datas_(count)
  {
    ifs->seekg(pos);
    ifs->read(reinterpret_cast<char *>(m_datas_.data()), sizeof(data_type) * count);
  }
  LinkList(LinkList &&)                 = default;
  LinkList(const LinkList &)            = default;
  LinkList &operator=(LinkList &&)      = default;
  LinkList &operator=(const LinkList &) = default;
  LinkList &operator+=(int val)
  {
    for (link_t &lk: *this) { lk.linkVal = std::min<int>(NUM_LINK_VAL_MAX, lk.linkVal + val); }
    return *this;
  }
  LinkList &operator-=(int val)
  {
    for (link_t &lk: *this) { lk.linkVal = std::max<int>(NUM_LINK_VAL_MIN, lk.linkVal - val); }
    return *this;
  }
  LinkList &operator*=(const weight_t<link_val> &weight)
  {
    for (link_t &lk: *this) { lk.linkVal = weight * lk.linkVal; }
    return *this;
  }
  LinkList operator*(const weight_t<link_val> &weight)
  {
    LinkList res = *this;
    return res *= weight;
  }
  LinkList operator-(int val)
  {
    LinkList res = *this;
    return res -= val;
  }
  LinkList operator+(int val)
  {
    LinkList res = *this;
    return res += val;
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
      if (!bIsAdd) { const_cast<link_val &>(setLink->linkVal) += lstLink.linkVal; }
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

  ENodeType m_type_   = ENodeType::NULL_TYPE;

public:
  active_t actVal = 0;

public:
  NodeInfo() = default;
  explicit NodeInfo(ENodeType nodetype) : LinkArray(), m_type_(nodetype), m_state_(define_state) {}
  explicit NodeInfo(node_id id) : LinkArray(), m_type_(ENodeType::NULL_TYPE), m_state_(define_state)
  {
    std::string path = GetNodeParentPath(id) + STR_NODE_FILE_NAME;
    if (!fs::exists(path)) { return; }

    std::ifstream file{path, std::ios ::binary | std::ios ::in};
    if (!file.is_open()) throw;
    ConstFileInfo tmpInfo{};
    file.read(reinterpret_cast<char *>(&tmpInfo), sizeof(ConstFileInfo));
    constLink  = LinkList(&file, file.tellg(), tmpInfo.const_count);
    staticLink = LinkList(&file, file.tellg(), tmpInfo.static_count);
    file.close();
  }

  NodeInfo(NodeInfo &&oth) = default;

  /// \b Turn dynamic links into static links.
  /// \param dySaveWeight Save to weight.
  void stableLink(const weight_t<link_val> &dySaveWeight)
  {
    if (m_state_ & IS_STABLE) { return; }
    dynamicLink *= dySaveWeight;
    staticLink.insert(dynamicLink);
    staticLink.deduplicate();
    constLink.deduplicate();
    m_state_ |= IS_STABLE;
  }

  void act(const weight_t<active_t> &actWeight, active_t actStand, LinkArray &out)
  {
    if (actStand > actVal) { return; }
    ++m_iActNum;
    weight_t<link_val> saveVal{static_cast<double>(actWeight * (actVal / NUM_LINK_VAL_MAX))};
    out.constLink.insert(constLink * saveVal);
    out.staticLink.insert(staticLink * saveVal);
    out.dynamicLink.insert(dynamicLink * saveVal);
  };

  bool writeNodeInfo(node_id id) noexcept(false)
  {
    std::string path = GetNodeParentPath(id);
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

using CLINKGROUP   = const NodeInfo;
using UP_NODE_INFO = std::unique_ptr<NodeInfo>;
using SP_NODE_INFO = std::shared_ptr<NodeInfo>;
using WPLINKGROUP  = std::weak_ptr<NodeInfo>;

using node_type    = std::pair<const node_id, std::shared_ptr<NodeInfo>>;//???
using node_list    = std::vector<node_type>;
#pragma pack(pop)

}// namespace think

#endif//THINKSYSTEM_THINKCORE_H
