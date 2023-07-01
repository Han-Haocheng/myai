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
  static std::string to_string(Coroutine::State state);

public:
  explicit Coroutine(const std::function<void()> &cb, size_t stacksize = 0);
  ~Coroutine();

  void resume();
  void yield();

private:
  void run();

private:
  uint64_t m_id = 0;
  State m_state;
  size_t m_stacksize = 0;
  void *m_stack = nullptr;
  ucontext_t m_ctx;
  std::function<void()> m_cb;

  static const size_t DEF_STACK_SIZE = 1024 * 1024 * 4;

  static size_t t_id_alloc;
  static size_t t_fiber_count;
};
}// namespace myai

#endif//MY_AI_COROUTINE_H
