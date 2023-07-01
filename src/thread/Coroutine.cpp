//
// Created by HanHaocheng on 2023/11/9.
//

#include "Coroutine.h"
#include "../log/Logger.h"
#include "../log/LoggerManager.h"

namespace myai
{
size_t Coroutine::t_id_alloc = 0;
size_t Coroutine::t_fiber_count = 0;

static Logger::ptr g_logger = MYAI_LOGGER_NAME("system");

std::string
Coroutine::to_string(Coroutine::State state)
{
  switch (state) {
#define XX(name) \
  case name:     \
    return #name
    XX(INITIAL);
    XX(READY);
    XX(HOLD);
    XX(EXECUTING);
    XX(TERMINATED);
    XX(EXCEPT);
#undef XX
  }
}

class MemeoryAlloc
{
public:
  static void *allocate(size_t size)
  {
    return malloc(size);
  }
  static void deallocate(void *ptr)
  {
    free(ptr);
  }
};

Coroutine::Coroutine(const std::function<void()> &cb, size_t stacksize)
    : m_id(t_id_alloc++),
      m_state(State::INITIAL),
      m_stacksize(stacksize),
      m_stack(MemeoryAlloc::allocate(stacksize == 0 ? DEF_STACK_SIZE : stacksize)),
      m_ctx(),
      m_cb(cb)
{
  ++t_fiber_count;

  int rt = getcontext(&m_ctx);
  if (rt) {
    MYAI_LOG_ERROR(g_logger) << "context careat faild,"
                             << "rt=" << rt;
    throw std::runtime_error("context careat faild");
  }
  m_ctx = {
      .uc_link = nullptr,
      .uc_stack = {
          .ss_sp = m_stack,
          .ss_size = m_stacksize,
      },
  };

  void (Coroutine::*runfunc)() = &myai::Coroutine::run;
  makecontext(&m_ctx, (void (*)()) & runfunc, 1, this);
  m_state = State::READY;
}

Coroutine::~Coroutine()
{
  static_assert();
  --t_fiber_count;
  MemeoryAlloc::deallocate(m_stack);
}

void Coroutine::resume()
{
}
void Coroutine::yield()
{
}

void Coroutine::run()
{
  m_state = State::EXECUTING;
  try {
    std::function<void()> cb = nullptr;
    cb.swap(m_cb);
    cb();
    m_state = State::TERMINATED;
  } catch (...) {
    m_state = State::EXCEPT;
  }

  yield();
}
}// namespace myai