//
// Created by HanHaocheng on 2023/11/9.
//

#ifndef MY_AI_COROUTINE_H
#define MY_AI_COROUTINE_H
#include <functional>
#include <memory>
#include <string>
#include <ucontext.h>

namespace myai
{
/// 协程
/// \概念 可以保存当前运行状态，并将当前状态设置为要执行的状态
///
class Coroutine : public std::enable_shared_from_this<Coroutine>
{
public:
  typedef std::shared_ptr<Coroutine> ptr;
  typedef std::weak_ptr<Coroutine> weak_ptr;
  enum State
  {
    INITIAL = 1 << 0,
    READY = 1 << 1,
    HOLD = 1 << 2,
    EXECUTING = 1 << 3,
    TERMINATED = 1 << 4,
    EXCEPT = 1 << 5,
  };

  static std::string toString(Coroutine::State state);
  static size_t GetId();
  static ptr GetThis();
  static void SetThis(Coroutine*co);

public:
  explicit Coroutine(const std::function<void()> &cb, size_t stacksize = 0);
  ~Coroutine() noexcept(false);

  void resume();

  State state();
  void reset(const std::function<void()> &cb);

  static void Yield();
  static void Restart();

private:
  static void _run();
  Coroutine();
  void swap_out();
  void swap_in();

private:
  uint64_t m_id = 0;
  State m_state = Coroutine::INITIAL;
  std::function<void()> m_cb = nullptr;

  void *m_stack = nullptr;
  size_t m_stacksize = 0;
  ucontext_t m_ctx;

  //ptr m_envCoroutine = nullptr;

  static const size_t DEF_STACK_SIZE = 1024 * 1024;
  static thread_local Coroutine* t_this_coroutine;
  static thread_local ptr t_root_coroutine;
  static std::atomic<size_t> s_id_alloc;
  static std::atomic<size_t> s_fiber_count;
};
}// namespace myai

#endif//MY_AI_COROUTINE_H
