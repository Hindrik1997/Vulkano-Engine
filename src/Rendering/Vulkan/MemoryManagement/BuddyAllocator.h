//
// Created by hindrik on 4-7-2017.
//

#ifndef VULKANOENGINE_BUDDYALLOCATOR_H
#define VULKANOENGINE_BUDDYALLOCATOR_H


#include "MemoryBlock.h"
#include "IMemBlockAllocator.h"
#include <utility>

using std::pair;

class BuddyAllocator : IMemBlockAllocator
{
public:
    BuddyAllocator(uint16_t levels, MemoryBlock&& memBlock);
    BuddyAllocator() = delete;
    ~BuddyAllocator() = default;

    BuddyAllocator(const BuddyAllocator&) = delete;
    BuddyAllocator(BuddyAllocator&&) = default;

    BuddyAllocator& operator=(const BuddyAllocator&) = delete;
    BuddyAllocator& operator=(BuddyAllocator&&) = default;
private:
    struct MemAllocInfo
    {
        uint16_t m_Level;
        uint16_t m_LevelIndex;
    };

    CacheOptimizedStorage<pair<vk_memory_allocation, MemAllocInfo>, UINT16_MAX-1> m_Allocations;
    uint16_t m_Subdivisions;

    struct MemBlock
    {
        const VkDeviceSize  m_Size          = 0;
        MemHandle           m_Allocation    = UINT16_MAX;
        uint16_t            m_Level         = UINT16_MAX;
        uint16_t            m_LevelIndex    = UINT16_MAX;
        uint16_t            m_InFreeList    = UINT16_MAX;
    };

    vector<MemBlock>            m_BinaryTree; //Preallocated memblock structs. Ordered from 1,2,4,8 etc.
    vector<vector<uint16_t>>    m_FreeLists;  //FreeLists
private:
    void setupTree();
    void setupFreeLists();
    uint16_t getFittingLevel(VkDeviceSize size);
    MemHandle allocateBlock(uint16_t level);
    bool breakBlockFromLevel(uint16_t level);
    int32_t coalesceBlockFromLevel(uint16_t level, uint16_t levelIndex);
public:
    MemHandle allocate(VkDeviceSize size) override;

    void free(MemHandle handle) override;

    const vk_memory_allocation& operator[](const MemHandle handle) override;


};


#endif //VULKANOENGINE_BUDDYALLOCATOR_H
