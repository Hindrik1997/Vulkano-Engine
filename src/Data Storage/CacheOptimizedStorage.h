//
// Created by hindrik on 29-11-16.
//

#ifndef VULKANO_ENGINE_COMPONENTSTORAGE_H
#define VULKANO_ENGINE_COMPONENTSTORAGE_H

#include "StorageItem.h"
#include "Pool.h"
#include "../Utility Classes/NonCopyable.h"

template <typename T, uint16_t SIZE = (UINT16_MAX-1)>
class CacheOptimizedStorage final
{
    static_assert(SIZE <= (UINT16_MAX-1) && SIZE >= 2, "You must bind a max_size between (and including) #MIN_SIZE and (including) #MAX_SIZE!");
public:
    CacheOptimizedStorage();
private:
    array <StorageItem<T>, SIZE> m_Items;
    array <uint16_t, SIZE> m_MappedIndices;
    Pool  <uint16_t, SIZE> m_Indices;
    uint16_t m_InUseCounter = 0;
public:

    CacheOptimizedStorage(const CacheOptimizedStorage& rhs);
    CacheOptimizedStorage(CacheOptimizedStorage&& rhs);
    CacheOptimizedStorage& operator=(CacheOptimizedStorage&& rhs);
    CacheOptimizedStorage& operator=(const CacheOptimizedStorage& rhs);

    template<typename... Args>
    uint16_t getNewItem(Args... args);

    void removeItem(const uint16_t index);

    T& operator[] (const uint16_t index);

    T* begin();
    T* end();

    uint16_t size() const;
    uint16_t max_size() const;

    void reset();
};

template<typename T, uint16_t SIZE>
template<typename... Args>
uint16_t CacheOptimizedStorage<T, SIZE>::getNewItem(Args... args) {

    uint16_t newIndex = static_cast<uint16_t>(m_Indices.getNewItem(m_InUseCounter));
    m_Items[newIndex].reset(args...);
    m_InUseCounter++;
    m_Indices[newIndex] = newIndex;
    m_MappedIndices[newIndex] = newIndex;
    return newIndex;
}

template<typename T, uint16_t SIZE>
void CacheOptimizedStorage<T, SIZE>::removeItem(const uint16_t nonMappedIndex) {
    if(nonMappedIndex == 65535)
        throw "Invalid index";

    uint16_t mappedIndex = m_Indices[nonMappedIndex];

    if(mappedIndex == m_InUseCounter - 1)
    {
        m_Items[mappedIndex].cleanUp();
        m_MappedIndices[nonMappedIndex] = 65535;
        m_Indices.removeItem(nonMappedIndex);
        m_InUseCounter--;
    }
    else
    {
        //Oude wegsmijten
        m_Items[mappedIndex].cleanUp();
        m_MappedIndices[nonMappedIndex] = 65535;
        m_Indices.removeItem(nonMappedIndex);

        //Laatste plaatsen op mappedIndex
        uint16_t nonMappedLastIndex = m_InUseCounter - static_cast<uint16_t >(1);
        uint16_t mappedLastIndex = m_Indices[nonMappedLastIndex];

        //laatste moven en index setten
        m_Indices[mappedLastIndex] = mappedIndex;
        m_Items[mappedIndex].m_Object.m_Object = std::move(m_Items[nonMappedLastIndex].m_Object.m_Object);

        m_InUseCounter--;
    }
}

template<typename T, uint16_t SIZE>
CacheOptimizedStorage<T,SIZE>::CacheOptimizedStorage() {
    for(uint16_t i = 0; i < SIZE; ++i)
    {
        m_MappedIndices[i] = 65535;
    }
}

template<typename T, uint16_t SIZE>
uint16_t CacheOptimizedStorage<T, SIZE>::max_size() const {
    return SIZE;
}

template<typename T, uint16_t SIZE>
T& CacheOptimizedStorage<T,SIZE>::operator[](const uint16_t index) {
    uint16_t mappedIndex = m_Indices[index];
    return m_Items[mappedIndex].m_Object.m_Object;
}

template<typename T, uint16_t SIZE>
T* CacheOptimizedStorage<T,SIZE>::begin() {
    return &(m_Items.begin()->m_Object.m_Object);
}

template<typename T, uint16_t SIZE>
T* CacheOptimizedStorage<T,SIZE>::end() {
    return &(m_Items.begin() + size())->m_Object.m_Object;
}

template<typename T, uint16_t SIZE>
uint16_t CacheOptimizedStorage<T, SIZE>::size() const {
    return m_InUseCounter;
}

template<typename T, uint16_t SIZE>
CacheOptimizedStorage<T,SIZE>::CacheOptimizedStorage(const CacheOptimizedStorage<T,SIZE> & rhs)
{
    m_Indices = rhs.m_Indices;
    m_InUseCounter = rhs.m_InUseCounter;
    m_MappedIndices = rhs.m_MappedIndices;


    for(uint16_t i = 0; i < rhs.size(); ++i)
    {
        m_Items[i].m_Object.m_Object = rhs.m_Items[i].m_Object.m_Object;
    }
}

template<typename T, uint16_t SIZE>
CacheOptimizedStorage<T,SIZE>::CacheOptimizedStorage(CacheOptimizedStorage<T,SIZE> && rhs)
{
    m_Indices = rhs.m_Indices;
    m_InUseCounter = rhs.m_InUseCounter;
    m_MappedIndices = rhs.m_MappedIndices;


    for(uint16_t i = 0; i < rhs.size(); ++i)
    {
        m_Items[i].m_Object.m_Object = std::move(rhs.m_Items[i].m_Object.m_Object);
    }
    rhs.reset();
}

template<typename T, uint16_t SIZE>
CacheOptimizedStorage<T,SIZE>& CacheOptimizedStorage<T,SIZE>::operator=(CacheOptimizedStorage<T,SIZE> && rhs)
{
    m_Indices = rhs.m_Indices;
    m_InUseCounter = rhs.m_InUseCounter;
    m_MappedIndices = rhs.m_MappedIndices;


    for(uint16_t i = 0; i < rhs.size(); ++i)
    {
        m_Items[i].m_Object.m_Object = std::move(rhs.m_Items[i].m_Object.m_Object);
    }
    rhs.reset();
    return *this;
}

template<typename T, uint16_t SIZE>
CacheOptimizedStorage<T,SIZE>& CacheOptimizedStorage<T,SIZE>::operator=(const CacheOptimizedStorage<T,SIZE> & rhs)
{
    m_Indices = rhs.m_Indices;
    m_InUseCounter = rhs.m_InUseCounter;
    m_MappedIndices = rhs.m_MappedIndices;


    for(uint16_t i = 0; i < rhs.size(); ++i)
    {
        m_Items[i].m_Object.m_Object = rhs.m_Items[i].m_Object.m_Object;
    }
    return *this;
}

template<typename T, uint16_t SIZE>
void CacheOptimizedStorage<T,SIZE>::reset()
{
    for(uint16_t i = 0; i < SIZE; ++i)
    {
        m_MappedIndices[i] = 65535;
    }

    m_Indices.reset();

    m_InUseCounter = 0;
}

#endif //VULKANO_ENGINE_COMPONENTSTORAGE_H