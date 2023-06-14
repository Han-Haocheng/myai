//
// Created by HanHaocheng on 2023/06/02.
//

#ifndef THINK_RESOURCEALLOCATOR_H
#define THINK_RESOURCEALLOCATOR_H
#include <forward_list>
#include <type_traits>
#include <vector>

#include "../io/Serialization.h"
#include "Range.h"
namespace thinksystem::utils
{

template<typename Ty>
class ResourceAllocator
{
  static_assert(is_arithmetic_extension<Ty>::value,
                "Error:please take Ty to be dakaideyeshu type what can have arithmetic operator.");
  using value_type           = Ty;
  using block_type           = Range<Ty>;
  using container_type       = ::std::forward_list<block_type>;
  using container_pointer    = container_type *;
  using block_iterator       = typename container_type::iterator;
  using block_const_iterator = typename container_type::const_iterator;
  using result_list          = ::std::vector<value_type>;

private:
  container_pointer m_unassigned_res_;

public:
  explicit ResourceAllocator(block_type &&val) { m_unassigned_res_->emplace_front(val); }

public:
  block_iterator begin() { return m_unassigned_res_->begin(); }
  block_iterator end() { return m_unassigned_res_->end(); }
  block_const_iterator cbegin() const { return m_unassigned_res_->cbegin(); }
  block_const_iterator cend() const { return m_unassigned_res_->cend(); }

public:
  bool allocate(value_type &out)
  {
    if (!m_unassigned_res_->empty()) throw("Type:type is empty", "Error:resource is empty,place increase resources.");
    block_type &beginBlock = m_unassigned_res_->front();
    out                    = *beginBlock.begin();
    beginBlock.setBegin(out + 1);
    if (beginBlock.is_empty()) { m_unassigned_res_->pop_front(); }
    return true;
  }

  bool allocate(result_list &out, size_t count)
  {
    if (count == 0ULL) throw("Type:parameter error", "Error:parameter 'count' can't be '0'.");
    out = result_list(count);

    while (!m_unassigned_res_->empty())
    {
      typename container_type::iterator bgBlock    = m_unassigned_res_->begin(),
                                        befBgBlock = m_unassigned_res_->before_begin();
      Ty i;
      for (i = bgBlock->cbegin(); i < bgBlock->cend(); ++i)
      {
        if (count == 0ULL)
        {
          bgBlock->setBegin(i);
          return true;
        }
        else { out[--count] = i; }
      }
      m_unassigned_res_->erase_after(befBgBlock);
    }
    throw("Type:type is empty", "Error:resource is empty,place increase resources.");
  }

  bool release(const value_type &in)
  {
    block_iterator itBeg = m_unassigned_res_->begin();
    block_type installRange(in, in + 1);
    if (in < itBeg->cbegin())
    {
      if (installRange.end() == itBeg->cbegin()) { itBeg->setBegin(itBeg->cbegin() - 1); }
      else { m_unassigned_res_->emplace_front(installRange); }
    }
    else
    {
      block_iterator itBfBeg = m_unassigned_res_->before_begin();

      for (; itBeg != m_unassigned_res_->end(); ++itBeg, ++itBfBeg)
      {
        if (itBeg->cbegin() > in)
        {
          if (itBfBeg->is_belong(in))
          {
            throw("Type:param out of range", "Error:Param 'in' is not in Allocator range");
          }
          if (itBfBeg->end == installRange.begin)
          {
            if (installRange.end() == itBeg->cbegin())
            {
              itBfBeg->setEnd(itBeg->cend());
              m_unassigned_res_->erase_after(itBfBeg);
            }
            else { itBfBeg->setEnd(installRange.end()); }
          }
          else if (installRange.end() == itBeg->cbegin()) { itBeg->setBegin(installRange.begin()); }
          else { block_iterator newIter = m_unassigned_res_->insert_after(itBfBeg, installRange); }
        }
      }

      if (itBfBeg->cend() == in) { itBfBeg->setEnd(itBfBeg->cend() + 1); }
      else { m_unassigned_res_->insert_after(itBfBeg, {in, in + 1}); }
    }
  }
  bool release(const result_list &in)
  {
    for (const auto &item: in) { release(item); }
  }
};//! class ResourceAllocator

}// namespace thinksystem::utils

#endif//THINK_RESOURCEALLOCATOR_H
