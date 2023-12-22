//
// Created by HanHaocheng on 2023/11/9.
//

#include "Scheduler.h"

#include <utility>

namespace myai
{
thread_local Scheduler::ptr Scheduler::t_scheduler = nullptr;
Scheduler::ptr Scheduler::GetThis()
{
  return t_scheduler;
}
Scheduler::Scheduler(size_t thread_num, std::string name) : m_name(std::move(name)), m_threadCount(thread_num)
{
  Thread::SetName(m_name);
}

void Scheduler::schedule(const Coroutine::ptr &coroutine)
{
}
void Scheduler::schedule(const std::function<void()> &func)
{
}
void Scheduler::start()
{
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

void Scheduler::stop()
{
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

bool Scheduler::isStop()
{
  return m_isStopping &&
         m_coroutines.empty() &&
         m_activeThreadCount == 0;
}

bool Scheduler::idle()
{
  return false;
}

bool Scheduler::tickle()
{
  return false;
}

void Scheduler::_run()
{
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

void Scheduler::_schedule_base(const Scheduler::CoroutineOrFunction &cof)
{
}

}// namespace myai