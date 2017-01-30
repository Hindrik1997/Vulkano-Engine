//
// Created by hindrik on 29-11-16.
//

#ifndef VULKANO_ENGINE_COMPONENTSTORAGE_H
#define VULKANO_ENGINE_COMPONENTSTORAGE_H

#include "StorageItem.h"
#include "Pool.h"

#define DEFAULT_SIZE 65534
#define MIN_SIZE 1
#define MAX_SIZE 65534



template <typename T, uint16_t SIZE = DEFAULT_SIZE>
class CacheOptimizedStorage {
    static_assert(SIZE <= MAX_SIZE && SIZE >= MIN_SIZE, "You must bind a size between (and including) #MIN_SIZE and (including) #MAX_SIZE!");
public:
    CacheOptimizedStorage();
private:
    array <StorageItem<T>, SIZE> m_items;
    array <uint16_t, SIZE> m_mappedIndices;
    Pool  <uint16_t, SIZE> m_indices;
    uint16_t m_inUseCounter = 0;
public:
    template<typename... Args>
    uint16_t getNewItem(Args... _args);

    void removeItem(uint16_t _index);

    uint16_t size();

    array<StorageItem<T>, SIZE>& getStorageRef() const;

};

template<typename T, uint16_t SIZE>
template<typename... Args>
uint16_t CacheOptimizedStorage<T, SIZE>::getNewItem(Args... _args) {

    uint16_t newIndex = static_cast<uint16_t>(m_indices.getNewItem(m_inUseCounter));
    m_items[newIndex].reset(_args...);
    m_inUseCounter++;
    m_indices[newIndex] = newIndex;
    m_mappedIndices[newIndex] = newIndex;
    return newIndex;
}

template<typename T, uint16_t SIZE>
void CacheOptimizedStorage<T, SIZE>::removeItem(uint16_t _nonmappedIndex) {
    if(_nonmappedIndex == 65535)
        throw "Invalid index";

    uint16_t mappedIndex = m_indices[_nonmappedIndex];

    if(mappedIndex == m_inUseCounter - 1)
    {
        m_items[mappedIndex].cleanUp();
        m_mappedIndices[_nonmappedIndex] = 65535;
        m_indices.removeItem(_nonmappedIndex);
        m_inUseCounter--;
    }
    else
    {
        //Oude wegsmijten
        m_items[mappedIndex].cleanUp();
        m_mappedIndices[_nonmappedIndex] = 65535;
        m_indices.removeItem(_nonmappedIndex);

        //Laatste plaatsen op mappedIndex
        uint16_t _nonmappedLastIndex = m_inUseCounter - (uint16_t)1;
        uint16_t mappedLastIndex = m_indices[_nonmappedLastIndex];

        //laatste moven en index setten
        m_indices[mappedLastIndex] = mappedIndex;
        m_items[mappedIndex] = std::move(m_items[_nonmappedLastIndex]);

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
uint16_t CacheOptimizedStorage<T, SIZE>::size() {
    return SIZE;
}

template<typename T, uint16_t SIZE>
array<StorageItem<T>, SIZE> &CacheOptimizedStorage<T, SIZE>::getStorageRef() const {
    return m_items;
}

#undef MAX_SIZE
#undef MIN_SIZE
#undef DEFAULT_SIZE

#endif //VULKANO_ENGINE_COMPONENTSTORAGE_H