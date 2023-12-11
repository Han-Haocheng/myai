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

class Coroutine
{
public:
  typedef std::shared_ptr<Coroutine> ptr;
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
  static size_t GetId() { return t_this_coroutine->m_id; }
  static ptr GetThis()
  {
    if (!t_this_coroutine) {
      Coroutine::ptr main_fiber{new Coroutine};
      if (t_this_coroutine != t_main_coroutine){

      }
      t_this_coroutine = main_fiber;
    }
    return t_this_coroutine;
  }

public:
  explicit Coroutine(const std::function<void()> &cb, size_t stacksize = 0);
  ~Coroutine();

  void resume();
  static void Yield();

  static void restart();

  State state();
  void reset(const std::function<void()> &cb);

private:
  void run();
  Coroutine();
  void swap_out();

private:
  uint64_t m_id = 0;
  State m_state = Coroutine::INITIAL;
  std::function<void()> m_cb = nullptr;

  void *m_stack = nullptr;
  size_t m_stacksize = 0;
  ucontext_t m_ctx;

  static const size_t DEF_STACK_SIZE = 1024 * 1024 * 4;

  static thread_local ptr t_this_coroutine;
  static thread_local ptr t_main_coroutine;
  static size_t s_id_alloc;
  static size_t s_fiber_count;
  static void SetThis(Coroutine::ptr co);
};
}// namespace myai

#endif//MY_AI_COROUTINE_H
