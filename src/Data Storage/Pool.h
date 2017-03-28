//
// Created by hindrik on 29-11-16.
//

#ifndef VULKANO_ENGINE_POOL_H
#define VULKANO_ENGINE_POOL_H

#include <array>
#include "PoolItem.h"
#include "../Utility Classes/NonCopyableNonMovable.h"

using std::array;

#define DEFAULT_SIZE 65534
#define MIN_SIZE 1
#define  MAX_SIZE 2000000

template<typename T, int32_t SIZE = DEFAULT_SIZE>
class Pool : NonCopyableNonMovable {
    static_assert(SIZE <= MAX_SIZE && SIZE >= MIN_SIZE, "You must bind a max_size between (and including) MIN_SIZE and (including) MAX_SIZE!");

public:
    Pool();

    inline T& operator[] (const int32_t index);
    inline T& at(const int32_t index);

    template<typename... Args>
    int32_t getNewItem(Args... args);

    void removeItem(const int32_t item);
    
    inline int32_t size();
    inline int32_t max_size();

    inline const int32_t getFirstFreeIndex() const;
private:
    int32_t m_InUseCounter = 0;
    int32_t m_FirstFreeIndex = 0;
    array<PoolItem<T>, SIZE> m_Storage;
};


template<typename T, int32_t SIZE>
inline int32_t Pool<T, SIZE>::size()
{
    return static_cast<int32_t>(m_InUseCounter);
}

template<typename T, int32_t SIZE>
inline const int32_t Pool<T, SIZE>::getFirstFreeIndex() const
{
    return m_FirstFreeIndex;
}

template<typename T, int32_t SIZE>
inline Pool<T,SIZE>::Pool()
{
    for(int32_t i = 0; i < size(); ++i)
    {
        if(i != size() - 1)
            m_Storage[i].m_CurrentState.m_NextItemIndex = (i + 1);
        else
            m_Storage[i].m_CurrentState.m_NextItemIndex = -1;
    }
}

template<typename T, int32_t SIZE>
template<typename... Args>
int32_t Pool<T, SIZE>::getNewItem(Args... args) {

    if(m_FirstFreeIndex == -1)
        throw "No free item left in the pool!";
    int32_t tempIndex = m_FirstFreeIndex;
    m_FirstFreeIndex = m_Storage[m_FirstFreeIndex].m_CurrentState.m_NextItemIndex;
    m_Storage[tempIndex].m_IsUsed = true;
    m_Storage[tempIndex].reset(args...);
    m_InUseCounter++;
    return tempIndex;
}


template<typename T, int32_t SIZE>
void Pool<T, SIZE>::removeItem(const int32_t item)
{
    if(item < 0 || item > size())
        throw "Out of range exception!";

    m_Storage[item].m_CurrentState.m_NextItemIndex = m_FirstFreeIndex;
    m_FirstFreeIndex = item;
    m_Storage[item].m_IsUsed = false;
    m_Storage[item].cleanUp();
    m_InUseCounter--;
}

template<typename T, int32_t SIZE>
inline T& Pool<T,SIZE>::operator[](const int32_t index)
{
    return m_Storage[index].m_CurrentState.m_Object;
}

template<typename T, int32_t SIZE>
inline T& Pool<T,SIZE>::at(const int32_t index)
{
    if(index > 0 && index < size())
        return m_Storage[index];
    throw "Out of range exception!";
}

template<typename T, int32_t SIZE>
int32_t Pool<T,SIZE>::max_size() {
    return SIZE;
};

#undef MAX_SIZE
#undef MIN_SIZE
#undef DEFAULT_SIZE

#endif //VULKANO_ENGINE_POOL_H
