//
// Created by hindrik on 16-6-17.
//

#include "AwaitableTask.h"
#include "Threadpool.h"
#include "Logger.h"

AwaitableTask::AwaitableTask(std::function<void()> func) : m_Func(func)
{
}

void AwaitableTask::wait()
{
    std::unique_lock<std::mutex> lk(m_Mutex);
    m_Condition.wait(lk, [&]{ return m_IsDone; });
}

void AwaitableTask::enqueue(ThreadPool &pool)
{
    if(!m_Func)
        Logger::error("No task assigned to execute!");

    pool.enqueue([this](){

        std::unique_lock<std::mutex> lk(m_Mutex);
        m_Func();
        m_IsDone = true;
        lk.unlock();
        m_Condition.notify_one();
    });
}

bool AwaitableTask::isTaskDone()
{
    return m_IsDone;
}

void AwaitableTask::resetCurrentTask()
{
    m_IsDone = false;
}

void AwaitableTask::assignNewTask(std::function<void()> func)
{
    m_Func = func;
}
