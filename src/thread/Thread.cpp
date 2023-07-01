//
// Created by HanHaocheng on 2023/11/9.
//

#include "Thread.h"

#include "../log/LoggerManager.h"
#include <csignal>
#include <string>
#include <sys/syscall.h>
#include <utility>

namespace myai
{

pid_t Thread::GetId() { return (pid_t) syscall(SYS_gettid); }
std::string Thread::GetName() { return t_this_thread->m_name; }
void Thread::SetThis(Thread::ptr thread) { t_this_thread = std::move(thread); }
Thread::ptr Thread::GetThis() { return t_this_thread; }

Thread::Thread(std::string name, std::function<void()> cb)
    : m_name(std::move(name)), m_cb(std::move(cb)), m_semaphore(0)
{
  if (t_this_thread != nullptr) {
    MYAI_LOG_ERROR(MYAI_LOGGER_ROOT) << "thread create faild, thread is exist. "
                                     << "id=" << t_this_thread->getId() << " "
                                     << "name=" << t_this_thread->getName() << std::endl;
    return;
  }

  t_this_thread = shared_from_this();

  int rt = pthread_create(&m_thread, nullptr, &Thread::run, this);
  if (rt) {
    MYAI_LOG_ERROR(MYAI_LOGGER_ROOT) << "thread create faild, "
                                     << "rt=" << rt;
    throw std::logic_error("pthread_create error");
  }

  pthread_setname_np(m_thread, m_name.data());

  m_semaphore.notify();
}
Thread::~Thread()
{
  if (m_thread) {
    pthread_detach(m_thread);
  }
}

void Thread::join() const { pthread_join(m_thread, nullptr); }

void *Thread::run(void *avg)
{
  auto *thread = (Thread *) avg;

  thread->m_pid = Thread::GetId();
  thread->m_semaphore.wait();

  std::function<void()> cb = nullptr;
  cb.swap(thread->m_cb);
  thread->m_semaphore.notify();
  cb();

  return nullptr;
}

}// namespace myai