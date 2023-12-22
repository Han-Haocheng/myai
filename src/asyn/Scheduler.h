//
// Created by HanHaocheng on 2023/11/9.
//

#ifndef MY_AI_SCHEDULER_H
#define MY_AI_SCHEDULER_H

#include "Coroutine.h"
#include "Thread.h"
#include <atomic>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>
namespace myai
{

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

}// namespace myai

#endif//MY_AI_SCHEDULER_H
