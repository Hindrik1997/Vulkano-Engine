//
// Created by hindrik on 29-11-16.
//

#ifndef VULKANO_ENGINE_POOLITEM_H
#define VULKANO_ENGINE_POOLITEM_H

#include <cstdint>

template<typename T>
class PoolItem {
public:
    PoolItem();
    ~PoolItem();

    union Data
    {
        Data(){};
        ~Data(){};

        T m_Object;
        int32_t m_NextItemIndex;
    } m_CurrentState;

    bool m_IsUsed = false;
    void cleanUp();

    template<typename... Args>
    void reset(Args... arguments);
};

template<typename T>
PoolItem<T>::PoolItem()
{
}

template<typename T>
inline PoolItem<T>::~PoolItem()
{
    if (m_IsUsed)
    {
        cleanUp();
    }
}

template<typename T>
inline void PoolItem<T>::cleanUp()
{
    m_CurrentState.m_Object.~T();
}

template<typename T>
template<typename... Args>
void PoolItem<T>::reset(Args... arguments)
{
    void* tVoid = &m_CurrentState;
    new (tVoid) T(arguments...);
}

#endif //VULKANO_ENGINE_POOLITEM_H
