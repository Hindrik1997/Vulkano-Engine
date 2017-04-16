//
// Created by hindrik on 16-4-17.
//

#ifndef VULKANOENGINE_THREADWORKER_H
#define VULKANOENGINE_THREADWORKER_H

#include <thread>
#include <mutex>
#include <iostream>
#include <vector>
#include <deque>

class ThreadPool;

class ThreadWorker final
{
public:
    ThreadWorker(ThreadPool &s) : pool(s) { }
    void operator()();
private:
    ThreadPool &pool;
};





#endif //VULKANOENGINE_THREADWORKER_H
