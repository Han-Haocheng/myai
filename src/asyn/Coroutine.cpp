//
// Created by HanHaocheng on 2023/11/9.
//

#include "Coroutine.h"

#include "../log/Logger.h"
#include "../log/LoggerManager.h"

namespace myai
{
static Logger::ptr g_logger = MYAI_LOGGER_NAME("system");

std::atomic<size_t> Coroutine::s_id_alloc = {0};
std::atomic<size_t> Coroutine::s_fiber_count = {0};
thread_local Coroutine *Coroutine::t_this_coroutine = nullptr;
thread_local Coroutine::ptr Coroutine::t_root_coroutine = nullptr;

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
  static void deallocate(void **ptr)
  {
    if (*ptr) {
      free(*ptr);
      *ptr = nullptr;
    }
  }
};
Coroutine::Coroutine()
    : m_ctx()
{
  ++s_fiber_count;

  m_state = Coroutine::EXECUTING;
  t_this_coroutine = this;

  if (getcontext(&m_ctx)) {
    MYAI_LOG_ERROR(g_logger) << "context careat faild.";
  }

  MYAI_LOG_DEBUG(g_logger) << "root id =" << m_id;
}

Coroutine::Coroutine(const std::function<void()> &cb, size_t stacksize)
    : m_id(++s_id_alloc),
      m_cb(cb),
      m_stacksize(stacksize == 0 ? DEF_STACK_SIZE : stacksize), m_ctx()
{
  ++s_fiber_count;

  m_stack = MemeoryAlloc::allocate(m_stacksize);
  m_ctx = {
      .uc_link = nullptr,
      .uc_stack = {
          .ss_sp = m_stack,
          .ss_size = m_stacksize,
      },
  };

  if (int rt = getcontext(&m_ctx)) {
    MYAI_LOG_ERROR(g_logger) << "context careat faild,"
                             << "rt=" << rt;
    throw std::runtime_error("context careat faild");
  }

  makecontext(&m_ctx, &Coroutine::_run, 1, this);
  m_state = State::READY;
}

Coroutine::~Coroutine() noexcept(false)
{
  --s_fiber_count;
  if (m_stack) {
    if (m_state & ~(Coroutine::INITIAL | Coroutine::EXCEPT | Coroutine::TERMINATED)) {
      MYAI_LOG_ERROR(g_logger) << "state=" << m_state << "."
                               << "state error.";
    }

    MemeoryAlloc::deallocate(&m_stack);
  } else {
    if (m_cb) {
      MYAI_LOG_ERROR(g_logger) << "has cb.";
    }
    if (m_state & ~(Coroutine::EXECUTING)) {
      MYAI_LOG_ERROR(g_logger) << "state=" << m_state << "."
                               << "state error.";
    }

    auto *cur = t_this_coroutine;
    if (cur == this) {
      t_this_coroutine = nullptr;
    }
  }
}

void Coroutine::resume()
{
  if (t_this_coroutine == this) {
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
    return;
  }
  MYAI_LOG_DEBUG(g_logger) << "Coroutine resume start.";
  auto old_root = t_root_coroutine;
  t_root_coroutine = GetThis();
  m_state = EXECUTING;
  swap_in();
  t_root_coroutine = old_root;
  MYAI_LOG_DEBUG(g_logger) << "Coroutine resume end.";
}

void Coroutine::Yield()
{
  auto this_co = t_this_coroutine;
  if (this_co->m_state & ~EXECUTING) {
    MYAI_LOG_ERROR(g_logger)
        << "id=" << this_co->m_id << ";"
        << "state=" << toString(this_co->m_state) << "."
        << "This coroutine need to be kept EXECUTING state.";
    return;
  }

  MYAI_LOG_DEBUG(g_logger) << "Coroutine Yield start.";

  this_co->m_state = HOLD;
  this_co->swap_out();

  MYAI_LOG_DEBUG(g_logger) << "Coroutine Yield end.";
}

void Coroutine::_run()
{
  auto this_co = GetThis();
  std::function<void()> cb = nullptr;

  try {
    cb.swap(this_co->m_cb);
    cb();
    this_co->m_state = State::TERMINATED;
  } catch (std::exception &e) {
    this_co->m_state = State::EXCEPT;
    MYAI_LOG_ERROR(g_logger)
        << "id=" << this_co->m_id << ";"
        << "except info=" << e.what() << "."
        << "Coroutine run exception.";
  } catch (...) {
    this_co->m_state = State::EXCEPT;
    MYAI_LOG_ERROR(g_logger)
        << "id=" << this_co->m_id << "."
        << "Coroutine run exception.";
  }

  auto raw_ptr = this_co.get();
  this_co.reset();
  raw_ptr->swap_out();//将当前协程退出

  MYAI_LOG_ERROR(g_logger) << "Coroutine can't run this.";
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
  m_ctx = {
      .uc_link = nullptr,
      .uc_stack = {
          .ss_sp = m_stack,
          .ss_size = m_stacksize,
      },
  };

  if (getcontext(&m_ctx)) {
    MYAI_LOG_ERROR(g_logger) << "getcontext falid";
  }
  makecontext(&m_ctx, &Coroutine::_run, 0);
}

void Coroutine::Restart()
{
  GetThis();
}
void Coroutine::SetThis(Coroutine *co)
{
  t_this_coroutine = co;
}

void Coroutine::swap_out()
{
  SetThis(t_root_coroutine.get());
  if (int rt = swapcontext(&m_ctx, &t_root_coroutine->m_ctx)) {
    MYAI_LOG_ERROR(g_logger) << "rt=" << rt << "getcontext falid";
  }
}
Coroutine::ptr Coroutine::GetThis()
{
  if (!t_this_coroutine) {
    t_root_coroutine.reset(new Coroutine{});
  }
  return t_this_coroutine->shared_from_this();
}
size_t Coroutine::GetId()
{
  return t_this_coroutine ? t_this_coroutine->m_id : 0;
}
void Coroutine::swap_in()
{
  SetThis(this);
  if (int rt = swapcontext(&t_root_coroutine->m_ctx, &m_ctx)) {
    MYAI_LOG_ERROR(g_logger) << "rt=" << rt << "getcontext falid";
  }
}

}// namespace myai