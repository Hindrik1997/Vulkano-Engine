//
// Created by hindrik on 16-4-17.
//

#include "Threadpool.h"
#include "ThreadWorker.h"

ThreadPool::ThreadPool(size_t threads) : stop(false)
{
    for (size_t i = 0; i < threads; ++i)
        workers.push_back(std::thread(ThreadWorker(*this)));
}

ThreadPool::~ThreadPool()
{
    stop = true;
    cond.notify_all();
    for (auto &thread : workers)
        thread.join();
}

