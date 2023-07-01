//
// Created by HanHaocheng on 2023/11/9.
//

#include "Mutex.h"
#include <stdexcept>

namespace myai
{
Semaphore::Semaphore(uint32_t count)
{
  if (sem_init(&m_semaphore, 0, count)) {
    throw std::runtime_error("sem_init failed");
  }
}
Semaphore::~Semaphore()
{
  if (sem_destroy(&m_semaphore)) {
    throw std::runtime_error("sem_destroy failed");
  }
}
void Semaphore::wait()
{
  if (sem_wait(&m_semaphore)) {
    throw std::runtime_error("sem_wait failed");
  }
}
void Semaphore::notify()
{
  if (sem_post(&m_semaphore)) {
    throw std::runtime_error("sem_post failed");
  }
}

Mutex::Mutex() : m_mutex() { pthread_mutex_init(&m_mutex, nullptr); }
Mutex::~Mutex() { pthread_mutex_destroy(&m_mutex); }
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

}// namespace myai