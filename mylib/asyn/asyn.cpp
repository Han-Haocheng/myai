//
// Created by HanHaocheng on 2023/12/28.
//

#include "asyn.h"
#include <atomic>

#if __linux__
#include <syscall.h>
#include <ucontext.h>
#elif _WIN32
#include <WinBase.h>
#endif
#include "../log/log.h"

namespace mylib {
thread_local Thread::ptr Thread::t_this_thread = nullptr;
auto g_logger = MYLIB_LOGGER_NAME("system");

//=====================================================================================================================

Semaphore::Semaphore(uint32_t count)

{
#if __linux__
  if (sem_init(&m_semaphore, 0, count)) {
    MYLIB_LOG_ERROR(g_logger) << "sem_init failed";
  }
#elif _WIN32
  m_ghSemaphore = CreateSemaphore(NULL, count, count, NULL);
  if (m_ghSemaphore == NULL) {
    MYLIB_LOG_ERROR(g_logger) << "CreateSemaphore error:" << GetLastError();
  }
#endif// __linux__
}
Semaphore::~Semaphore() {
#if __linux__
  if (sem_destroy(&m_semaphore)) {
    MYLIB_LOG_ERROR(g_logger) << "sem_destroy failed";
  }
#elif _WIN32
  CloseHandle(m_ghSemaphore);
#endif
}

void Semaphore::wait() {
#if __linux__
  if (sem_wait(&m_semaphore)) {
    MYLIB_LOG_ERROR(g_logger) << "sem_wait failed";
  }
#elif _WIN32
  DWORD dwWaitResult;
  do {
    dwWaitResult = WaitForSingleObject(m_ghSemaphore, 1L);
  } while (dwWaitResult == WAIT_TIMEOUT);
  if (dwWaitResult == WAIT_FAILED) {
    MYLIB_LOG_ERROR(g_logger) << "WaitForSingleObject failed";
  }
#endif
}
void Semaphore::notify() {
#if __linux__
  if (sem_post(&m_semaphore)) {
    MYLIB_LOG_ERROR(g_logger) << "sem_post failed";
  }
#elif _WIN32
  if (!ReleaseSemaphore(m_ghSemaphore, 1, NULL)) {
    MYLIB_LOG_ERROR(g_logger) << "ReleaseSemaphore error:" << GetLastError();
  }
#endif
}

Mutex::Mutex() : m_mutex() {
  // m_mutex = CreateMutexExA();
  pthread_mutex_init(&m_mutex, nullptr);
}
Mutex::~Mutex() { /*pthread_mutex_destroy(&m_mutex);*/
}
void Mutex::lock() { pthread_mutex_lock(&m_mutex); }
void Mutex::unlock() { pthread_mutex_unlock(&m_mutex); }

SpanMutex::SpanMutex() : m_spinlock() { pthread_spin_init(&m_spinlock, 0); }
SpanMutex::~SpanMutex() { pthread_spin_destroy(&m_spinlock); }
void SpanMutex::lock() { pthread_spin_lock(&m_spinlock); }
void SpanMutex::unlock() { pthread_spin_unlock(&m_spinlock); }

RWMutex::RWMutex() : m_rwmutex() { pthread_rwlock_init(&m_rwmutex, nullptr); }
RWMutex::~RWMutex() { pthread_rwlock_destroy(&m_rwmutex); }
void RWMutex::rdlock() { pthread_rwlock_rdlock(&m_rwmutex); }
void RWMutex::wrlock() { pthread_rwlock_wrlock(&m_rwmutex); }
void RWMutex::unlock() { pthread_rwlock_unlock(&m_rwmutex); }

//=====================================================================================================================

tid_t Thread::GetId() {
#if __linux__
  return (tid_t) syscall(SYS_gettid);
#elif _WIN32
  return GetCurrentThreadId();
#endif// __linux__
}

std::string Thread::GetName() { return t_this_thread->m_name; }
void Thread::SetName(std::string name) { t_this_thread->m_name = std::move(name); }
void Thread::SetThis(Thread::ptr thread) { t_this_thread = std::move(thread); }
Thread::ptr Thread::GetThis() { return t_this_thread; }

Thread::Thread(std::string name, std::function<void()> cb)
    : m_name(std::move(name)), m_cb(std::move(cb)), m_semaphore(0) {
  if (t_this_thread != nullptr) {
    MYLIB_LOG_ERROR(MYLIB_LOGGER_ROOT) << "thread create failed, thread is exist. "
                                       << "id=" << t_this_thread->getId() << " "
                                       << "name=" << t_this_thread->getName() << std::endl;
    return;
  }

  t_this_thread = shared_from_this();

#if __linux__
  int rt = pthread_create(&m_thread, nullptr, &Thread::run, this);
  if (rt) {
    MYLIB_LOG_ERROR(MYLIB_LOGGER_ROOT) << "thread create failed, "
                                       << "rt=" << rt;
    throw std::logic_error("pthread_create error");
  }
  pthread_setname_np(m_thread, m_name.data());
#elif _WIN32
  m_thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) &Thread::run, NULL, 0, NULL);
  if (m_thread == NULL) {
    MYLIB_LOG_ERROR(MYLIB_LOGGER_ROOT) << "thread create failed, "
                                       << "errno=" << GetLastError();
    throw std::logic_error("pthread_create error");
  }

#endif// __linux__
  m_semaphore.notify();
}
Thread::~Thread() {
#if __linux__
  if (m_thread) {
    pthread_detach(m_thread);
  }
#elif _WIN32
  CloseHandle(m_thread);
#endif// __linux__
}

void Thread::join() const {
#if __linux__
  pthread_join(m_thread, nullptr);
#elif _WIN32
  if (WAIT_FAILED == WaitForSingleObject(m_thread, INFINITE)) {
    MYLIB_LOG_ERROR(MYLIB_LOGGER_ROOT) << "WaitForSingleObject failed."
                                       << "erron=" << GetLastError();
  }

#endif// __linux__
}
void *Thread::run(void *avg) {
  auto *thread = (Thread *) avg;

  thread->m_pid = Thread::GetId();
  thread->m_semaphore.wait();

  std::function<void()> cb = nullptr;
  cb.swap(thread->m_cb);
  thread->m_semaphore.notify();
  cb();

  return nullptr;
}

//=====================================================================================================================
std::atomic<size_t> Coroutine::s_id_alloc = {0};
std::atomic<size_t> Coroutine::s_fiber_count = {0};
thread_local Coroutine *Coroutine::t_this_coroutine = nullptr;
thread_local Coroutine::ptr Coroutine::t_root_coroutine = nullptr;

std::string
Coroutine::toString(Coroutine::State state) {
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

class MemoryAlloc {
public:
  static void *allocate(size_t size) {
    return malloc(size);
  }
  static void deallocate(void **ptr) {
    if (*ptr) {
      free(*ptr);
      *ptr = nullptr;
    }
  }
};
Coroutine::Coroutine()
    : m_ctx() {
  ++s_fiber_count;

  m_state = Coroutine::EXECUTING;
  t_this_coroutine = this;

  if (getcontext(&m_ctx)) {
    MYLIB_LOG_ERROR(g_logger) << "context create failed.";
  }

  MYLIB_LOG_DEBUG(g_logger) << "root id =" << m_id;
}

Coroutine::Coroutine(const std::function<void()> &cb, size_t stacksize)
    : m_id(++s_id_alloc),
      m_cb(cb),
      m_stacksize(stacksize == 0 ? DEF_STACK_SIZE : stacksize), m_ctx() {
  ++s_fiber_count;

  m_stack = MemoryAlloc::allocate(m_stacksize);
  m_ctx = {
      .uc_link = nullptr,
      .uc_stack = {
          .ss_sp = m_stack,
          .ss_size = m_stacksize,
      },
  };

  if (int rt = getcontext(&m_ctx)) {
    MYLIB_LOG_ERROR(g_logger) << "context create failed,"
                              << "rt=" << rt;
    throw std::runtime_error("context create failed");
  }

  makecontext(&m_ctx, &Coroutine::_run, 1, this);
  m_state = State::READY;
}

Coroutine::~Coroutine() noexcept(false) {
  --s_fiber_count;
  if (m_stack) {
    if (m_state & ~(Coroutine::INITIAL | Coroutine::EXCEPT | Coroutine::TERMINATED)) {
      MYLIB_LOG_ERROR(g_logger) << "state=" << m_state << "."
                                << "state error.";
    }

    MemoryAlloc::deallocate(&m_stack);
  } else {
    if (m_cb) {
      MYLIB_LOG_ERROR(g_logger) << "has cb.";
    }
    if (m_state & ~(Coroutine::EXECUTING)) {
      MYLIB_LOG_ERROR(g_logger) << "state=" << m_state << "."
                                << "state error.";
    }

    auto *cur = t_this_coroutine;
    if (cur == this) {
      t_this_coroutine = nullptr;
    }
  }
}

void Coroutine::resume() {
  if (t_this_coroutine == this) {
    MYLIB_LOG_ERROR(g_logger)
        << "id=" << m_id << "."
        << "When resuming a coroutine, This coroutine can't be a running coroutine for this thread.";
    return;
  }
  if (m_state & ~(HOLD | READY)) {
    MYLIB_LOG_ERROR(g_logger)
        << "id=" << m_id << ";"
        << "state=" << toString(m_state) << "."
        << "When resuming a coroutine, this coroutine need to be kept EXECUTING state.";
    return;
  }
  MYLIB_LOG_DEBUG(g_logger) << "Coroutine resume start.";
  auto old_root = t_root_coroutine;
  t_root_coroutine = GetThis();
  m_state = EXECUTING;
  swap_in();
  t_root_coroutine = old_root;
  MYLIB_LOG_DEBUG(g_logger) << "Coroutine resume end.";
}

void Coroutine::YieldCoroutine() {
  auto this_co = t_this_coroutine;
  if (this_co->m_state & ~EXECUTING) {
    MYLIB_LOG_ERROR(g_logger)
        << "id=" << this_co->m_id << ";"
        << "state=" << toString(this_co->m_state) << "."
        << "This coroutine need to be kept EXECUTING state.";
    return;
  }

  MYLIB_LOG_DEBUG(g_logger) << "Coroutine Yield start.";

  this_co->m_state = HOLD;
  this_co->swap_out();

  MYLIB_LOG_DEBUG(g_logger) << "Coroutine Yield end.";
}

void Coroutine::_run() {
  auto this_co = GetThis();
  std::function<void()> cb = nullptr;

  try {
    cb.swap(this_co->m_cb);
    cb();
    this_co->m_state = State::TERMINATED;
  } catch (std::exception &e) {
    this_co->m_state = State::EXCEPT;
    MYLIB_LOG_ERROR(g_logger)
        << "id=" << this_co->m_id << ";"
        << "except info=" << e.what() << "."
        << "Coroutine run exception.";
  } catch (...) {
    this_co->m_state = State::EXCEPT;
    MYLIB_LOG_ERROR(g_logger)
        << "id=" << this_co->m_id << "."
        << "Coroutine run exception.";
  }

  auto raw_ptr = this_co.get();
  this_co.reset();
  raw_ptr->swap_out();//将当前协程退出

  MYLIB_LOG_ERROR(g_logger) << "Coroutine can't run this.";
}

Coroutine::State Coroutine::state() { return m_state; }

void Coroutine::reset(const std::function<void()> &cb) {
  if (!m_stack) {
    MYLIB_LOG_ERROR(g_logger) << "m_stack is null";
  }
  if (m_state & ~(Coroutine::TERMINATED | Coroutine::INITIAL | Coroutine::EXCEPT)) {
    MYLIB_LOG_ERROR(g_logger) << "state is not";
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
    MYLIB_LOG_ERROR(g_logger) << "getcontext failed";
  }
  makecontext(&m_ctx, &Coroutine::_run, 0);
}

void Coroutine::Restart() {
  GetThis();
}
void Coroutine::SetThis(Coroutine *co) {
  t_this_coroutine = co;
}

void Coroutine::swap_out() {
  SetThis(t_root_coroutine.get());
  if (int rt = swapcontext(&m_ctx, &t_root_coroutine->m_ctx)) {
    MYLIB_LOG_ERROR(g_logger) << "rt=" << rt << "getcontext failed";
  }
}
Coroutine::ptr Coroutine::GetThis() {
  if (!t_this_coroutine) {
    t_root_coroutine.reset(new Coroutine{});
  }
  return t_this_coroutine->shared_from_this();
}
size_t Coroutine::GetId() {
  return t_this_coroutine ? t_this_coroutine->m_id : 0;
}
void Coroutine::swap_in() {
  SetThis(this);
  if (int rt = swapcontext(&t_root_coroutine->m_ctx, &m_ctx)) {
    MYLIB_LOG_ERROR(g_logger) << "rt=" << rt << "getcontext failed";
  }
}

//=====================================================================================================================

thread_local Scheduler::ptr Scheduler::t_scheduler = nullptr;
Scheduler::ptr Scheduler::GetThis() { return t_scheduler; }
Scheduler::Scheduler(size_t thread_num, std::string name) : m_name(std::move(name)), m_threadCount(thread_num) { Thread::SetName(m_name); }

void Scheduler::schedule(const Coroutine::ptr &coroutine) {}
void Scheduler::schedule(const std::function<void()> &func) {}

void Scheduler::start() {
  if (!m_isStopped) {
    return;
  }
  m_isStopped = false;

  m_threads.reserve(m_threadCount);
  for (size_t i = 0; i < m_threadCount; ++i) {
    m_threads.emplace_back(new Thread{m_name + "_" + std::to_string(i),
                                      std::bind(&Scheduler::_run, this)});
  }
}

void Scheduler::stop() {
  if (m_isStopped || m_isStopping) {
    return;
  }
  m_isStopping = true;

  for (const auto &thr: m_threads) {
    thr->join();
  }

  m_isStopping = false;
  m_isStopped = true;
}

bool Scheduler::isStop() { return m_isStopping && m_coroutines.empty() && m_activeThreadCount == 0; }

bool Scheduler::idle() { return false; }

bool Scheduler::tickle() { return false; }

void Scheduler::_run() {
  t_scheduler = shared_from_this();
  const Coroutine::ptr idleCoroutine{new Coroutine{std::bind(&Scheduler::idle, this)}};
  Coroutine::ptr tempCoroutine{nullptr};
  CoroutineOrFunction tempCOF{};
  while (!isStop()) {
    bool isTickled = false;
    bool isActive = false;

    for (auto iter = m_coroutines.begin(); iter != m_coroutines.end(); ++iter) {
      if (iter->thread_id != -1 && iter->thread_id == Thread::GetId()) {
        isTickled = true;
      }
      if (iter->coroutine && (iter->coroutine->state() & Coroutine::EXECUTING)) {
        continue;
      }
      tempCOF = *iter;
      m_coroutines.erase(iter);
      isActive = true;
      ++m_activeThreadCount;
      break;
    }

    if (isTickled) {
      tickle();
    }

    if (tempCOF.coroutine) {
      tempCOF.coroutine->resume();
      if (tempCOF.coroutine->state() & (Coroutine::READY | Coroutine::HOLD)) {
        schedule(tempCOF.coroutine);
      } else {
        tempCOF.clear();
      }
    } else if (tempCOF.function) {
      if (tempCoroutine) {
        tempCoroutine.reset(new Coroutine{tempCOF.function});
      } else {
        tempCoroutine->reset(tempCOF.function);
      }
      tempCoroutine->resume();
      if (tempCoroutine->state() & Coroutine::READY) {
        schedule(tempCOF.function);
      } else {
        tempCOF.clear();
      }
    } else {
      if (isActive) {
        --m_activeThreadCount;
      }

      ++m_idleThreadCount;
      idleCoroutine->resume();
      --m_idleThreadCount;
    }
  }
}

void Scheduler::_schedule_base(const Scheduler::CoroutineOrFunction &cof) {
}

//=====================================================================================================================
}// namespace mylib
