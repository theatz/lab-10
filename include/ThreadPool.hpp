//
// Created by mrbgn on 4/21/21.
//

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

#ifndef TEMPLATE_THREADPOOL_HPP
#define TEMPLATE_THREADPOOL_HPP
class ThreadPool {
 public:
  ThreadPool(size_t);
  template<class Task>
  void enqueue(Task&& task);
//  void Stop();
  ~ThreadPool();
 private:
  std::vector< std::thread > workers;
  std::queue< std::function<void()> > tasks;

  std::mutex queue_mutex;
  std::condition_variable condition;
  bool stop;
};

inline ThreadPool::ThreadPool(size_t threads)
    :   stop(false)
{
  for(size_t i = 0;i<threads;++i)
    workers.emplace_back(
        [this]
        {
          for(;;)
          {
            std::function<void()> task;

            {
              std::unique_lock<std::mutex> lock(this->queue_mutex);
              this->condition.wait(lock,
                                   [this]{ return this->stop || !this->tasks.empty(); });
              if(this->stop && this->tasks.empty())
                return;
              task = std::move(this->tasks.front());
              this->tasks.pop();
            }

            task();
          }
        }
    );
}

template<class Task>
void ThreadPool::enqueue(Task&& worker_task)
{
  {
    std::unique_lock<std::mutex> lock(queue_mutex);

    // don't allow enqueueing after stopping the pool
    if(stop)
      throw std::runtime_error("enqueue on stopped ThreadPool");

    tasks.emplace([worker_task] { (worker_task)(); });
  }
  condition.notify_one();
}

ThreadPool::~ThreadPool()
{
  {
    std::unique_lock<std::mutex> lock(queue_mutex);
    stop = true;
  }
  condition.notify_all();
  for(std::thread &worker: workers)
    worker.join();
}


#endif  // TEMPLATE_THREADPOOL_HPP
