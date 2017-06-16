//
// Created by hindrik on 16-6-17.
//

#include "SynchedTask.h"
#include "Threadpool.h"

SynchedTask::SynchedTask(std::function<void()> func) : m_Func(func)
{
}

void SynchedTask::wait()
{
    std::unique_lock<std::mutex> lk(m_Mutex);
    m_Condition.wait(lk, [&]{ return m_IsDone; });
}

void SynchedTask::enqueue(ThreadPool &pool)
{
    pool.enqueue([this](){

        std::unique_lock<std::mutex> lk(m_Mutex);
        m_Func();
        m_IsDone = true;
        lk.unlock();
        m_Condition.notify_one();
    });
}

bool SynchedTask::isTaskDone() {
    return m_IsDone;
}

void SynchedTask::reset()
{
    m_IsDone = false;
}
