//
// Created by HanHaocheng on 2023/11/9.
//

#include "Coroutine.h"

#include "../log/Logger.h"
#include "../log/LoggerManager.h"
#include <utility>

namespace myai
{
static Logger::ptr g_logger = MYAI_LOGGER_NAME("system");

size_t Coroutine::s_id_alloc = 0;
size_t Coroutine::s_fiber_count = 0;
thread_local Coroutine::ptr Coroutine::t_this_coroutine = nullptr;

std::string
Coroutine::toString(Coroutine::State state)
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
  return "";
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
Coroutine::Coroutine()
    : m_ctx()
{
  m_state = Coroutine::EXECUTING;
  t_this_coroutine.reset(this);
  //获取协程的上下文
  if (getcontext(&m_ctx)) {
    MYAI_LOG_ERROR(g_logger) << "context careat faild.";
  }

  MYAI_LOG_DEBUG(g_logger) << "root id =" << m_id;
}

Coroutine::Coroutine(const std::function<void()> &cb, size_t stacksize)
    : m_id(s_id_alloc++),
      m_cb(cb),
      m_stacksize(stacksize == 0 ? DEF_STACK_SIZE : stacksize), m_ctx()
{

  m_stack = MemeoryAlloc::allocate(m_stacksize);
  ++s_fiber_count;

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
  void (Coroutine::*runfunc)() = &Coroutine::run;
  makecontext(&m_ctx, (void (*)()) & runfunc, 1, this);
  m_state = State::READY;
}

Coroutine::~Coroutine()
{
  --s_fiber_count;
  MemeoryAlloc::deallocate(m_stack);
}

void Coroutine::resume()
{
  if (t_this_coroutine == nullptr) {
    t_this_coroutine.reset(new Coroutine{});
  }

  if (t_this_coroutine.get() == this) {
    MYAI_LOG_ERROR(g_logger)
        << "id=" << m_id << "."
        << "When resuming a coroutine, This coroutine can't be a running coroutine for this thread.";
    return;
  }
  if (m_state & ~(HOLD | READY)) {
    MYAI_LOG_ERROR(g_logger)
        << "id=" << m_id << ";"
        << "state=" << toString(m_state) << "."
        << "When resuming a coroutine, this coroutine need to be kept EXECUTING state.";
  }
  MYAI_LOG_DEBUG(g_logger)
      << "this id=" << m_id << ";"
      << "this state=" << toString(m_state) << ";"
      << "running id=" << GetId() << "."
      << "Coroutine resume start.";
  m_state = EXECUTING;
  swapcontext(&t_this_coroutine->m_ctx, &m_ctx);

  MYAI_LOG_DEBUG(g_logger)
      << "this id=" << m_id << ";"
      << "this state=" << toString(m_state) << ";"
      << "running id=" << GetId() << "."
      << "Coroutine resume end.";
}

void Coroutine::Yield()
{
  auto this_co = t_this_coroutine.get();

  if (this_co->m_state & ~EXECUTING) {
    MYAI_LOG_ERROR(g_logger)
        << "id=" << this_co->m_id << ";"
        << "state=" << toString(this_co->m_state) << "."
        << "This coroutine need to be kept EXECUTING state.";
    return;
  }
  MYAI_LOG_DEBUG(g_logger)
      << "this id=" << this_co->m_id << ";"
      << "this state=" << toString(this_co->m_state) << "."
      << "Coroutine Yield start.";
  this_co->m_state = HOLD;
  swapcontext(&this_co->m_ctx, &t_this_coroutine->m_ctx);

  MYAI_LOG_DEBUG(g_logger)
      << "this id=" << this_co->m_id << ";"
      << "this state=" << toString(this_co->m_state) << "."
      << "Coroutine Yield end.";
}

void Coroutine::run()
{
  auto environment = t_this_coroutine.get();
  if (environment == this) {
    MYAI_LOG_ERROR(g_logger)
        << "id=" << m_id << ";"
        << "state=" << toString(m_state) << "."
        << "This coroutine can't equal to thread coroutine.";
  }
  t_this_coroutine.reset(this);

  m_state = State::EXECUTING;
  std::function<void()> cb = nullptr;

  try {
    cb.swap(m_cb);
    cb();
    m_state = State::TERMINATED;
  } catch (std::exception &e) {
    m_state = State::EXCEPT;
    MYAI_LOG_ERROR(g_logger)
        << "id=" << m_id << ";"
        << "except info=" << e.what() << "."
        << "Coroutine run exception.";
  } catch (...) {
    m_state = State::EXCEPT;
    MYAI_LOG_ERROR(g_logger)
        << "id=" << m_id << "."
        << "Coroutine run exception.";
  }

  t_this_coroutine.reset(environment);
}

Coroutine::State Coroutine::state()
{
  return m_state;
}

void Coroutine::reset(const std::function<void()> &cb)
{
  if (!m_stack) {
    MYAI_LOG_ERROR(g_logger) << "m_stack is null";
  }
  if (m_state & ~(Coroutine::TERMINATED | Coroutine::INITIAL | Coroutine::EXCEPT)) {
    MYAI_LOG_ERROR(g_logger) << "state is not";
  }
  m_cb = cb;

  if (getcontext(&m_ctx)) {
    MYAI_LOG_ERROR(g_logger) << "getcontext falid";
  }

  m_ctx = {
      .uc_link = nullptr,
      .uc_stack = {
          .ss_sp = m_stack,
          .ss_size = m_stacksize,
      },
  };

  void (Coroutine::*runfunc)() = &Coroutine::run;
  makecontext(&m_ctx, (void (*)()) & runfunc, 1, this);
}

void Coroutine::restart()
{
  GetThis();
}
void Coroutine::SetThis(Coroutine::ptr co)
{
  t_this_coroutine = std::move(co);
}

void Coroutine::swap_out()
{
  if (t_this_coroutine.get() != this) {
    MYAI_LOG_ERROR(g_logger)
        << "this id=" << m_id << ";"
        << "running id=" << GetId() << "."
        << "This coroutine must be a running coroutine for this thread.";
    return;
  }

  if (m_state & ~EXECUTING) {
    MYAI_LOG_ERROR(g_logger)
        << "id=" << m_id << ";"
        << "state=" << toString(m_state) << "."
        << "This coroutine need to be kept EXECUTING state.";
    return;
  }
  MYAI_LOG_DEBUG(g_logger)
      << "this id=" << m_id << ";"
      << "this state=" << toString(m_state) << ";"
      << "running id=" << GetId() << "."
      << "Coroutine Yield start.";

  swapcontext(&m_ctx, &t_main_coroutine->m_ctx);
}

}// namespace myai