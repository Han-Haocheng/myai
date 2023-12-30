//
// Created by HanHaocheng on 2023/12/28.
//

#ifndef MY_LIB_ASYN_H
#define MY_LIB_ASYN_H

#include <atomic>
#include <csignal>
#include <cstdint>
#include <functional>
#include <memory>
#include <semaphore.h>
#include <string>
#include <vector>

namespace mylib
{

class Semaphore
{
public:
  explicit Semaphore(uint32_t count = 0);
  ~Semaphore();

  //等待
  void wait();
  //播放
  void notify();

private:
  sem_t m_semaphore{};
};

//=====================================================================================================================

template<typename MutexType>
class MutexLock
{
public:
  explicit MutexLock(MutexType &mutex) : m_mutex(mutex) { lock(); }
  ~MutexLock() { unlock(); }

  void lock()
  {
    if (m_locked) {
      return;
    }
    m_mutex.lock();
    m_locked = true;
  }

  void unlock()
  {
    if (!m_locked) {
      return;
    }
    m_mutex.unlock();
    m_locked = false;
  }

private:
  MutexType m_mutex;
  bool m_locked = false;
};

template<typename RWMutexType>
class RdMutexLock
{
public:
  explicit RdMutexLock(RWMutexType &mutex) : m_mutex(mutex) { lock(); }
  ~RdMutexLock() { unlock(); }

  void lock()
  {
    if (m_locked) {
      return;
    }
    m_mutex.rdlock();
    m_locked = true;
  }
  void unlock()
  {
    if (!m_locked) {
      return;
    }
    m_mutex.unlock();
    m_locked = false;
  }

private:
  RWMutexType m_mutex;
  bool m_locked = false;
};

template<typename RWMutexType>
class WrMutexLock
{
public:
  explicit WrMutexLock(RWMutexType &mutex) : m_mutex(mutex) { lock(); }
  ~WrMutexLock() { unlock(); }

  void lock()
  {
    if (m_locked) {
      return;
    }
    m_mutex.wrlock();
    m_locked = true;
  }
  void unlock()
  {
    if (!m_locked) {
      return;
    }
    m_mutex.unlock();
    m_locked = false;
  }

private:
  RWMutexType m_mutex;
  bool m_locked = false;
};

class Mutex
{
public:
  typedef class MutexLock<Mutex> Lock;

  Mutex();
  ~Mutex();
  void lock();
  void unlock();

private:
  pthread_mutex_t m_mutex;
};

class SpanMutex
{
public:
  typedef class MutexLock<SpanMutex> Lock;

  SpanMutex();
  ~SpanMutex();
  void lock();
  void unlock();

private:
  pthread_spinlock_t m_spinlock;
};

class RWMutex
{
public:
  typedef class WrMutexLock<RWMutex> ReadLock;
  typedef class RdMutexLock<RWMutex> WriteLock;

  RWMutex();
  ~RWMutex();

  void rdlock();
  void wrlock();
  void unlock();

private:
  pthread_rwlock_t m_rwmutex;
};

//=====================================================================================================================

class Thread : public std::enable_shared_from_this<Thread>
{
public:
  typedef std::shared_ptr<Thread> ptr;

  static pid_t GetId();
  static Thread::ptr GetThis();
  static std::string GetName();
  static void SetName(std::string name);

public:
  Thread(std::string name, std::function<void()> cb);
  Thread(const Thread &) = delete;
  Thread &operator=(const Thread &) = delete;

  ~Thread();

  void join() const;

  pid_t getId() const { return m_pid; }
  const std::string &getName() const { return m_name; }

private:
  static void SetThis(Thread::ptr thread);
  static void *run(void *avg);

private:
  pid_t m_pid = -1;
  pthread_t m_thread = 0;
  std::string m_name;
  std::function<void()> m_cb = nullptr;
  Semaphore m_semaphore;
  static thread_local Thread::ptr t_this_thread;
};

//=====================================================================================================================

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
  static void SetThis(Coroutine *co);

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
  static thread_local Coroutine *t_this_coroutine;
  static thread_local ptr t_root_coroutine;
  static std::atomic<size_t> s_id_alloc;
  static std::atomic<size_t> s_fiber_count;
};

//=====================================================================================================================

class Scheduler : public std::enable_shared_from_this<Scheduler>
{
public:
  using ptr = std::shared_ptr<Scheduler>;

private:
  struct CoroutineOrFunction {
    pid_t thread_id = -1;
    Coroutine::ptr coroutine = nullptr;
    std::function<void()> function = nullptr;
    CoroutineOrFunction() = default;
    CoroutineOrFunction(pid_t thread_id, Coroutine::ptr coroutine)
        : thread_id(thread_id), coroutine(std::move(coroutine)) {}
    CoroutineOrFunction(pid_t thread_id, std::function<void()> function)
        : thread_id(thread_id), function(std::move(function)) {}
    bool empty() const
    {
      return coroutine == nullptr && function == nullptr;
    }
    void clear()
    {
      thread_id = -1;
      coroutine = nullptr;
      function = nullptr;
    }
  };

  static ptr GetThis();

public:
  Scheduler(size_t thread_num, std::string name);

  void schedule(const Coroutine::ptr &coroutine);
  void schedule(const std::function<void()> &func);
  void start();//启动
  void stop(); //停止

protected:
  virtual bool isStop();
  virtual bool idle();
  virtual bool tickle();

private:
  void _run();
  void _schedule_base(const CoroutineOrFunction &cof);

private:
  bool m_isStopped = false;
  bool m_isStopping = false;
  std::string m_name;
  size_t m_threadCount = 0;//线程数量

  std::atomic<size_t> m_activeThreadCount = {0};//活跃线程数
  std::atomic<size_t> m_idleThreadCount = {0};  //空闲线程数

  std::vector<Thread::ptr> m_threads;
  std::vector<CoroutineOrFunction> m_coroutines;

private:
  static thread_local Scheduler::ptr t_scheduler;
};
//=====================================================================================================================

}// namespace mylib

#endif//MY_AI_ASYN_H
