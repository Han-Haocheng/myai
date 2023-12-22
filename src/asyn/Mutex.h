//
// Created by HanHaocheng on 2023/11/9.
//

#ifndef MY_AI_MUTEX_H
#define MY_AI_MUTEX_H
#include <memory>
#include <pthread.h>
#include <semaphore.h>
namespace myai
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

}// namespace myai

#endif//MY_AI_MUTEX_H
