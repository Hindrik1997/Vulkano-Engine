//
// Created by hindrik on 29-11-16.
//

#ifndef VULKANO_ENGINE_COMPONENTSTORAGE_H
#define VULKANO_ENGINE_COMPONENTSTORAGE_H

#include "StorageItem.h"
#include "Pool.h"
#include "../Utility Classes/NonCopyableNonMovable.h"

#define DEFAULT_SIZE 65534
#define MIN_SIZE 1
#define MAX_SIZE 65534



template <typename T, uint16_t SIZE = DEFAULT_SIZE>
class CacheOptimizedStorage : NonCopyableNonMovable {
    static_assert(SIZE <= MAX_SIZE && SIZE >= MIN_SIZE, "You must bind a max_size between (and including) #MIN_SIZE and (including) #MAX_SIZE!");
public:
    CacheOptimizedStorage();
private:
    array <StorageItem<T>, SIZE> m_items;
    array <uint16_t, SIZE> m_mappedIndices;
    Pool  <uint16_t, SIZE> m_indices;
    uint16_t m_inUseCounter = 0;
public:
    template<typename... Args>
    uint16_t getNewItem(Args... args);

    void removeItem(const uint16_t index);

    T& operator[] (const uint16_t index);


    T* begin();
    T* end();

    uint16_t size();
    uint16_t max_size();

};

template<typename T, uint16_t SIZE>
template<typename... Args>
uint16_t CacheOptimizedStorage<T, SIZE>::getNewItem(Args... args) {

    uint16_t newIndex = static_cast<uint16_t>(m_indices.getNewItem(m_inUseCounter));
    m_items[newIndex].reset(args...);
    m_inUseCounter++;
    m_indices[newIndex] = newIndex;
    m_mappedIndices[newIndex] = newIndex;
    return newIndex;
}

template<typename T, uint16_t SIZE>
void CacheOptimizedStorage<T, SIZE>::removeItem(const uint16_t nonMappedIndex) {
    if(nonMappedIndex == 65535)
        throw "Invalid index";

    uint16_t mappedIndex = m_indices[nonMappedIndex];

    if(mappedIndex == m_inUseCounter - 1)
    {
        m_items[mappedIndex].cleanUp();
        m_mappedIndices[nonMappedIndex] = 65535;
        m_indices.removeItem(nonMappedIndex);
        m_inUseCounter--;
    }
    else
    {
        //Oude wegsmijten
        m_items[mappedIndex].cleanUp();
        m_mappedIndices[nonMappedIndex] = 65535;
        m_indices.removeItem(nonMappedIndex);

        //Laatste plaatsen op mappedIndex
        uint16_t nonMappedLastIndex = m_inUseCounter - static_cast<uint16_t >(1);
        uint16_t mappedLastIndex = m_indices[nonMappedLastIndex];

        //laatste moven en index setten
        m_indices[mappedLastIndex] = mappedIndex;
        m_items[mappedIndex] = std::move(m_items[nonMappedLastIndex]);

        m_inUseCounter--;
    }
}

template<typename T, uint16_t SIZE>
CacheOptimizedStorage<T,SIZE>::CacheOptimizedStorage() {
    for(uint16_t i = 0; i < SIZE; ++i)
    {
        m_mappedIndices[i] = 65535;
    }
}

template<typename T, uint16_t SIZE>
uint16_t CacheOptimizedStorage<T, SIZE>::max_size() {
    return SIZE;
}

template<typename T, uint16_t SIZE>
T& CacheOptimizedStorage<T,SIZE>::operator[](const uint16_t index) {
    uint16_t mappedIndex = m_indices[index];
    return m_items[mappedIndex].m_Object.m_Object;
}

template<typename T, uint16_t SIZE>
T* CacheOptimizedStorage<T,SIZE>::begin() {
    return &(m_items.begin()->m_Object.m_Object);
}

template<typename T, uint16_t SIZE>
T* CacheOptimizedStorage<T,SIZE>::end() {
    return &(m_items.begin() + size())->m_Object.m_Object;
}

template<typename T, uint16_t SIZE>
uint16_t CacheOptimizedStorage<T, SIZE>::size() {
    return m_inUseCounter;
}


#undef MAX_SIZE
#undef MIN_SIZE
#undef DEFAULT_SIZE

#endif //VULKANO_ENGINE_COMPONENTSTORAGE_H