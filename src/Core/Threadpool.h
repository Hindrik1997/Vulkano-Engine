//
// Created by hindrik on 16-4-17.
//

#ifndef VULKANOENGINE_THREADPOOL_H
#define VULKANOENGINE_THREADPOOL_H

#include <thread>
#include <mutex>
#include <iostream>
#include <vector>
#include <deque>
#include <condition_variable>

class ThreadPool final
{
public:
    ThreadPool(size_t threads);
    template<class F> void enqueue(F f);
    ~ThreadPool();
private:
    friend class ThreadWorker;

    std::vector<std::thread> workers;
    std::deque<std::function<void()>> tasks;
    std::condition_variable cond;
    std::mutex queue_mutex;
    bool stop;
};

template<class F>
void ThreadPool::enqueue(F f)
{
    std::unique_lock<std::mutex> lock(queue_mutex);
    tasks.push_back(std::function<void()>(f));
    cond.notify_one();
}


#endif //VULKANOENGINE_THREADPOOL_H
