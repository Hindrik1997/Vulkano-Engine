//
// Created by hindrik on 16-6-17.
//

#ifndef VULKANOENGINE_SYNCHEDTASK_H
#define VULKANOENGINE_SYNCHEDTASK_H


#include <functional>
#include <mutex>
#include <condition_variable>

class ThreadPool;

class AwaitableTask
{
private:
    std::function<void()>   m_Func;
    std::mutex              m_Mutex;
    std::condition_variable m_Condition;
    bool                    m_IsDone        = false;
public:
    AwaitableTask() = default;
    AwaitableTask(std::function<void()> func);
    ~AwaitableTask() = default;

    AwaitableTask(AwaitableTask&&)      = delete;
    AwaitableTask(const AwaitableTask&) = delete;

    AwaitableTask& operator=(const AwaitableTask&) = delete;
    AwaitableTask& operator=(AwaitableTask&&)      = delete;
public:
    void enqueue(ThreadPool& pool);
    void wait();
    bool isTaskDone();
    void resetCurrentTask();
    void assignNewTask(std::function<void()> func);
};


#endif //VULKANOENGINE_SYNCHEDTASK_H
