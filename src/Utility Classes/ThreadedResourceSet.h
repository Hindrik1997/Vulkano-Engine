//
// Created by hindrik on 17-6-17.
//

#ifndef VULKANOENGINE_THREADEDRESOURCESET_H
#define VULKANOENGINE_THREADEDRESOURCESET_H

#include <deque>
#include <mutex>
#include <condition_variable>
#include <algorithm>

using std::deque;
using std::mutex;
using std::condition_variable;
using std::unique_lock;
using std::defer_lock;
using std::remove;
using std::begin;
using std::end;


template<typename T>
class ThreadedResourceSet
{
private:
    deque<T>            m_Resources;
    mutex               m_DequeMutex;

    condition_variable  m_ConditionVariable;
    int32_t             m_TasksCounter = 0;
public:
    ThreadedResourceSet(const vector<T>&  m_Objects);
    ThreadedResourceSet(vector<T>&& m_Objects);
    ~ThreadedResourceSet();

    ThreadedResourceSet(const ThreadedResourceSet&)     = default;
    ThreadedResourceSet(ThreadedResourceSet&&)          = default;

    ThreadedResourceSet& operator=(const ThreadedResourceSet&)  = default;
    ThreadedResourceSet& operator=(ThreadedResourceSet&&)       = default;
private:
    T acquireResource(bool isReserved = false);
    void returnResource(T resource);
public:
    void acquireAndRun(std::function<void(T resource)> function);
    void acquireAndRunReservedTask(std::function<void(T resource)> function);

    void reserveTasks(int32_t taskCount);

    void waitOnTasks();
    bool isResourceAvailable();
};


template<typename T>
T ThreadedResourceSet<T>::acquireResource(bool isReserved)
{
    unique_lock<mutex> lock(m_DequeMutex);
    m_ConditionVariable.wait(lock, [this]{ return isResourceAvailable(); });

    T resource = m_Resources.front();
    m_Resources.pop_front();

    if(!isReserved)
        m_TasksCounter++;

    lock.unlock();
    return resource;
}

template<typename T>
void ThreadedResourceSet<T>::returnResource(T resource)
{
    unique_lock<mutex> lock(m_DequeMutex);
    m_Resources.push_back(resource);

    m_TasksCounter--;

    lock.unlock();
    m_ConditionVariable.notify_one();
}

template<typename T>
bool ThreadedResourceSet<T>::isResourceAvailable()
{
    return m_Resources.size() != 0;
}

template<typename T>
ThreadedResourceSet<T>::ThreadedResourceSet(const vector<T>& m_Objects)
{
    for(auto& a : m_Objects)
    {
        m_Resources.push_back(a);
    }
}

template<typename T>
ThreadedResourceSet<T>::ThreadedResourceSet(vector<T>&& m_Objects)
{
    for(auto& a : m_Objects)
    {
        m_Resources.push_back(a);
    }
}

template<typename T>
ThreadedResourceSet<T>::~ThreadedResourceSet()
{
    waitOnTasks();
}

template<typename T>
void ThreadedResourceSet<T>::acquireAndRun(function<void(T)> func)
{
    //acquire resource
    T resource = acquireResource();

    //execute task
    func(resource);

    //return resource
    returnResource(resource);
}

template<typename T>
void ThreadedResourceSet<T>::waitOnTasks()
{
    unique_lock<mutex> lock(m_DequeMutex);
    m_ConditionVariable.wait(lock, [this]{ return m_TasksCounter == 0; });

    lock.unlock();
}

template<typename T>
void ThreadedResourceSet<T>::reserveTasks(int32_t taskCount)
{
    m_TasksCounter += taskCount;
}

template<typename T>
void ThreadedResourceSet<T>::acquireAndRunReservedTask(function<void(T)> func)
{
    //acquire resource
    T resource = acquireResource(true);

    //execute task
    func(resource);

    //return resource
    returnResource(resource);
}


#endif //VULKANOENGINE_THREADEDRESOURCESET_H
