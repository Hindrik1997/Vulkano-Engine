//
// Created by hindrik on 29-11-16.
//

#ifndef VULKANO_ENGINE_POOL_H
#define VULKANO_ENGINE_POOL_H

#include <array>
#include "PoolItem.h"

using std::array;

template<typename T,uint16_t SIZE>
class CacheOptimizedStorage;

template<typename T, uint16_t SIZE = (UINT16_MAX - 1)>
class Pool final
{
    static_assert(SIZE <= (UINT16_MAX-1) && SIZE >= 2, "You must bind a max_size between (and including) MIN_SIZE and (including) MAX_SIZE!");
public:
    Pool();

    inline T& operator[] (const uint16_t index);
    inline T& at(const uint16_t index);


    Pool(const Pool&)               = default;
    Pool(Pool&&)                    = default;
    Pool& operator=(const Pool&)    = default;
    Pool& operator=(Pool&&)         = default;

    template<typename... Args>
    uint16_t getNewItem(Args... args);

    void removeItem(const uint16_t item);
    
    inline uint16_t size();
    inline uint16_t max_size();

    inline const uint16_t getFirstFreeIndex() const;

    void reset();
private:
    uint16_t m_InUseCounter = 0;
    uint16_t m_FirstFreeIndex = 0;
    array<PoolItem<T>, SIZE> m_Storage;
};


template<typename T, uint16_t SIZE>
inline uint16_t Pool<T, SIZE>::size()
{
    return m_InUseCounter;
}

template<typename T, uint16_t SIZE>
inline const uint16_t Pool<T, SIZE>::getFirstFreeIndex() const
{
    return m_FirstFreeIndex;
}

template<typename T, uint16_t SIZE>
inline Pool<T,SIZE>::Pool()
{
    for(uint16_t i = 0; i < max_size(); ++i)
    {
        if(i != max_size() - 1)
            m_Storage[i].m_CurrentState.m_NextItemIndex = (i + 1);
        else
            m_Storage[i].m_CurrentState.m_NextItemIndex = -1;
    }
}

template<typename T, uint16_t SIZE>
template<typename... Args>
uint16_t Pool<T, SIZE>::getNewItem(Args... args) {

    if(m_FirstFreeIndex == -1)
        throw "No free item left in the pool!";
    uint16_t tempIndex = getFirstFreeIndex();
    m_FirstFreeIndex = m_Storage[m_FirstFreeIndex].m_CurrentState.m_NextItemIndex;
    m_Storage[tempIndex].m_IsUsed = true;
    m_Storage[tempIndex].reset(args...);
    m_InUseCounter++;
    return tempIndex;
}


template<typename T, uint16_t SIZE>
void Pool<T, SIZE>::removeItem(const uint16_t item)
{
    if(item < 0 || item > size())
        throw "Out of range exception!";

    m_Storage[item].m_CurrentState.m_NextItemIndex = getFirstFreeIndex();
    m_FirstFreeIndex = item;
    m_Storage[item].m_IsUsed = false;
    m_Storage[item].cleanUp();
    m_InUseCounter--;
}

template<typename T, uint16_t SIZE>
inline T& Pool<T,SIZE>::operator[](const uint16_t index)
{
    return m_Storage[index].m_CurrentState.m_Object;
}

template<typename T, uint16_t SIZE>
inline T& Pool<T,SIZE>::at(const uint16_t index)
{
    if(index > 0 && index < size())
        return m_Storage[index];
    throw "Out of range exception!";
}

template<typename T, uint16_t SIZE>
uint16_t Pool<T,SIZE>::max_size() {
    return SIZE;
}

template<typename T, uint16_t SIZE>
void Pool<T,SIZE>::reset()
{
    for(uint16_t i = 0; i < max_size(); ++i)
    {
        if(i != max_size() - 1)
            m_Storage[i].m_CurrentState.m_NextItemIndex = (i + 1);
        else
            m_Storage[i].m_CurrentState.m_NextItemIndex = -1;
    }

    for(uint16_t i = 0; i < max_size(); ++i)
    {
        m_Storage[i].m_IsUsed = false;
    }
};

#undef MAX_SIZE
#undef MIN_SIZE
#undef DEFAULT_SIZE

#endif //VULKANO_ENGINE_POOL_H