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

        T m_object;
        uint16_t m_nextItemIndex;
    } m_currentState;

    bool m_isUsed = false;
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
    if (m_isUsed)
    {
        cleanUp();
    }
}

template<typename T>
inline void PoolItem<T>::cleanUp()
{
    m_currentState.m_object.~T();
}

template<typename T>
template<typename... Args>
void PoolItem<T>::reset(Args... arguments)
{
    void* tVoid = &m_currentState;
    new (tVoid) T(arguments...);
}

#endif //VULKANO_ENGINE_POOLITEM_H
