//
// Created by HanHaocheng on 2023/11/9.
//

#ifndef MY_AI_THREAD_H
#define MY_AI_THREAD_H

#include "Mutex.h"
#include <functional>
#include <memory>
#include <string>
namespace myai
{

class Thread : public std::enable_shared_from_this<Thread>
{
public:
  typedef std::shared_ptr<Thread> ptr;

  static pid_t GetId();
  static Thread::ptr GetThis();
  static std::string GetName();

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

}// namespace myai

#endif//MY_AI_THREAD_H
