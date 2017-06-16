//
// Created by hindrik on 16-6-17.
//

#ifndef VULKANOENGINE_SYNCHEDTASK_H
#define VULKANOENGINE_SYNCHEDTASK_H


#include <functional>
#include <mutex>
#include <condition_variable>

class ThreadPool;

class SynchedTask
{
private:
    std::function<void()>   m_Func;
    std::mutex              m_Mutex;
    std::condition_variable m_Condition;
    bool                    m_IsDone        = false;
public:
    SynchedTask(std::function<void()> func);
    ~SynchedTask() = default;

    SynchedTask(SynchedTask&&)      = default;
    SynchedTask(const SynchedTask&) = delete;

    SynchedTask& operator=(const SynchedTask&) = delete;
    SynchedTask& operator=(SynchedTask&&)      = default;
public:
    void enqueue(ThreadPool& pool);
    void wait();
    bool isTaskDone();
    void reset();
};


#endif //VULKANOENGINE_SYNCHEDTASK_H
