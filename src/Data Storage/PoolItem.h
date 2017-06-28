//
// Created by hindrik on 29-11-16.
//

#ifndef VULKANO_ENGINE_POOLITEM_H
#define VULKANO_ENGINE_POOLITEM_H

#include <cstdint>
#include "../Utility Classes/NonCopyable.h"

template<typename T>
class PoolItem final
{
public:
    PoolItem();
    ~PoolItem();

    PoolItem(const PoolItem& rhs) = delete;
    PoolItem&operator=(const PoolItem& rhs) = delete;

    PoolItem(PoolItem&& rhs);
    PoolItem& operator=(PoolItem&& rhs);

    union Data
    {
        Data(){};
        ~Data(){};

        Data(const Data&) = delete;
        Data(Data&&) = delete;

        Data& operator=(const Data&) = delete;
        Data& operator=(Data&&) = delete;

        T m_Object;
        int32_t m_NextItemIndex = 0;

    } m_CurrentState;

    bool m_IsUsed = false;
    void cleanUp();

    template<typename... Args>
    void reset(Args&&... arguments);
};

template<typename T>
PoolItem<T>::PoolItem()
{
}

template<typename T>
inline PoolItem<T>::~PoolItem()
{
    cleanUp();
}

template<typename T>
inline void PoolItem<T>::cleanUp()
{
    if(m_IsUsed)
    {
        m_CurrentState.m_Object.~T();
        m_IsUsed = false;
    }
}

template<typename T>
template<typename... Args>
void PoolItem<T>::reset(Args&&... arguments)
{
    cleanUp();
    void* tVoid = &m_CurrentState;
    new (tVoid) T(std::forward<Args>(arguments)...);
    m_IsUsed = true;
}


template<typename T>
PoolItem<T>::PoolItem(PoolItem<T> && rhs)
{
    cleanUp();
    if(!rhs.m_IsUsed)
    {
        m_IsUsed = false;
        m_CurrentState.m_NextItemIndex = rhs.m_CurrentState.m_NextItemIndex;
        rhs.m_CurrentState.m_NextItemIndex = -1;
    }
    else
    {
        m_IsUsed = true;
        m_CurrentState.m_Object = std::move(rhs.m_CurrentState.m_Object);
    }
}

template<typename T>
PoolItem<T>& PoolItem<T>::operator=(PoolItem<T> && rhs)
{
    cleanUp();
    if(!rhs.m_IsUsed)
    {
        m_CurrentState.m_NextItemIndex = rhs.m_CurrentState.m_NextItemIndex;
        rhs.m_CurrentState.m_NextItemIndex = -1;
        m_IsUsed = false;
    }
    else
    {
        m_CurrentState.m_Object = std::move(rhs.m_CurrentState.m_Object);
        m_IsUsed = true;
    }
    return *this;
}

#endif //VULKANO_ENGINE_POOLITEM_H
