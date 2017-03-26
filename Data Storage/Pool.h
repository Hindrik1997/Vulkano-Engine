//
// Created by hindrik on 29-11-16.
//

#ifndef VULKANO_ENGINE_POOL_H
#define VULKANO_ENGINE_POOL_H

#include <array>
#include "PoolItem.h"
#include "../Utility Classes/NonCopyable.h"
#include "../Utility Classes/NonMovable.h"

using std::array;

#define DEFAULT_SIZE 65534
#define MIN_SIZE 1
#define  MAX_SIZE 2000000

template<typename T, int SIZE = DEFAULT_SIZE>
class Pool : public NonCopyable, public NonMovable {
    static_assert(SIZE <= MAX_SIZE && SIZE >= MIN_SIZE, "You must bind a max_size between (and including) MIN_SIZE and (including) MAX_SIZE!");

public:
    Pool();

    inline T& operator[] (const size_t index);
    inline T& At(const size_t index);

    template<typename... Args>
    int getNewItem(Args... args);

    inline const array<PoolItem<T>, SIZE>& getStorageRef() const;

    void removeItem(int item);

    inline int getItemInUseCount();
    inline size_t size();

    inline const int getFirstFreeIndex() const;
private:
    int m_inUseCounter = 0;
    int m_firstFreeIndex = 0;
    int m_maxSize = SIZE;
    array<PoolItem<T>, SIZE> storage;
};


template<typename T, int SIZE>
inline int Pool<T, SIZE>::getItemInUseCount()
{
    return m_inUseCounter;
}

template<typename T, int SIZE>
inline size_t Pool<T, SIZE>::size()
{
    return static_cast<size_t>(m_maxSize);
}

template<typename T, int SIZE>
inline const int Pool<T, SIZE>::getFirstFreeIndex() const
{
    return m_firstFreeIndex;
}

template<typename T, int SIZE>
inline Pool<T,SIZE>::Pool()
{
    for(size_t i = 0; i < size(); ++i)
    {
        if(i != size() - 1)
            storage[static_cast<int>(i)].m_currentState.m_nextItemIndex = (static_cast<int>(i) + 1);
        else
            storage[static_cast<int>(i)].m_currentState.m_nextItemIndex = -1;
    }
}

template<typename T, int SIZE>
template<typename... Args>
int Pool<T, SIZE>::getNewItem(Args... args) {

    if(m_firstFreeIndex == -1)
        throw "No free item left in the pool!";
    int tempIndex = m_firstFreeIndex;
    m_firstFreeIndex = storage[m_firstFreeIndex].m_currentState.m_nextItemIndex;
    storage[tempIndex].m_isUsed = true;
    storage[tempIndex].reset(args...);
    m_inUseCounter++;
    return tempIndex;
}

template<typename T, int SIZE>
inline const array<PoolItem<T>, SIZE>& Pool<T, SIZE>::getStorageRef() const
{
    return storage;
}

template<typename T, int SIZE>
void Pool<T, SIZE>::removeItem(int item)
{
    if(item < 0 || item > static_cast<int>(size()))
        throw "Out of range exception!";

    storage[item].m_currentState.m_nextItemIndex = m_firstFreeIndex;
    m_firstFreeIndex = item;
    storage[item].m_isUsed = false;
    storage[item].cleanUp();
    m_inUseCounter--;
}

template<typename T, int SIZE>
inline T& Pool<T,SIZE>::operator[](const size_t index)
{
    return storage[index].m_currentState.m_object;
}

template<typename T, int SIZE>
inline T& Pool<T,SIZE>::At(const size_t index)
{
    if(index > 0 && index < size())
        return storage[index];
    throw "Out of range exception!";
};

#undef MAX_SIZE
#undef MIN_SIZE
#undef DEFAULT_SIZE

#endif //VULKANO_ENGINE_POOL_H
