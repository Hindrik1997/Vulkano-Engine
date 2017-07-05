//
// Created by hindrik on 4-7-2017.
//

#include <unordered_map>
#include "BuddyAllocator.h"
#include <algorithm>

BuddyAllocator::BuddyAllocator(uint16_t levels, MemoryBlock&& memoryBlock) : IMemBlockAllocator(std::forward<MemoryBlock&&>(memoryBlock)), m_Subdivisions(levels)
{
    assert(levels != 0);
    setupTree();
    setupFreeLists();
}

IMemBlockAllocator::MemHandle BuddyAllocator::allocate(VkDeviceSize size)
{
    if(size > m_MemoryBlock.allocatedSize())
        return UINT16_MAX; //Won't fit!

    uint16_t level = getFittingLevel(size);

    if(!(level >= 0 && level <= m_Subdivisions))
        return UINT16_MAX; //Won't fit!

    if(m_FreeLists[level].size() != 0)
    {
        return allocateBlock(level);
    }
        if(level == 0) //Won't ever fit!
            return UINT16_MAX;

        uint16_t firstFreeLevel = level;
        //get level up and subdivide it if possible!
        for(uint16_t i = level; i >= 0; --i)
        {
            if(m_FreeLists[i].size() != 0)
            {
                firstFreeLevel = i;
                break;
            }
            if(i == 0)
                return UINT16_MAX;
        }
        if(firstFreeLevel == level)
            return UINT16_MAX; //No suitable place!

        for(; firstFreeLevel != level; firstFreeLevel++)
        {
            breakBlockFromLevel(firstFreeLevel);
        }

        if(m_FreeLists[level].size() == 0)
            Logger::failure("Something went really wrong yo!");

        return allocateBlock(level);
}

void BuddyAllocator::free(IMemBlockAllocator::MemHandle handle)
{
    if(handle == UINT16_MAX)
    {
        Logger::warn("Invalid handle passed to memory allocator!");
        return;
    }

    pair<vk_memory_allocation,MemAllocInfo> allocation = m_Allocations[handle];
    m_Allocations.removeItem(handle);

    uint16_t bHIndex = static_cast<uint16_t >(std::pow(2,allocation.second.m_Level) - 1) + allocation.second.m_LevelIndex;

    MemBlock& block = m_BinaryTree[bHIndex];
    block.m_Allocation = UINT16_MAX;
    block.m_InFreeList = UINT16_MAX - 1;
    m_FreeLists[allocation.second.m_Level].push_back(allocation.second.m_LevelIndex);

    uint16_t levelIndex = allocation.second.m_LevelIndex;
    for(uint16_t l = allocation.second.m_Level; l > 0; --l)
    {
        int32_t coalesce = coalesceBlockFromLevel(l, levelIndex);
        if(coalesce == -1)
            break;
        levelIndex = static_cast<uint16_t >(coalesce);
    }


}

const vk_memory_allocation& BuddyAllocator::operator[](const IMemBlockAllocator::MemHandle handle)
{
    return m_Allocations[handle].first;
}

void BuddyAllocator::setupTree()
{
    VkDeviceSize totalMemSize = m_MemoryBlock.allocatedSize();

    m_BinaryTree.push_back(MemBlock{ totalMemSize, UINT16_MAX, 0, 0, UINT16_MAX - 1 });

    for(uint16_t sdv = 1; sdv <= m_Subdivisions; sdv++)
    {
        uint16_t subdivisonsAtLevel = static_cast<uint16_t>(std::pow(2,sdv));

        for(uint16_t s = 0; s < subdivisonsAtLevel; ++s)
        {
            m_BinaryTree.emplace_back(MemBlock{ totalMemSize / subdivisonsAtLevel, UINT16_MAX, sdv, s, UINT16_MAX    });
        }
    }

    assert(m_BinaryTree.size() == static_cast<size_t>(std::pow(2, m_Subdivisions) + (std::pow(2, m_Subdivisions) - 1)));
    m_BinaryTree.shrink_to_fit();
}

void BuddyAllocator::setupFreeLists()
{
    m_FreeLists.reserve(m_Subdivisions + 1);
    for(uint16_t i = 0; i <= m_Subdivisions; ++i)
    {
        vector<uint16_t> vec = {};
        vec.reserve(static_cast<uint64_t >(std::pow(2,i)));
        m_FreeLists.push_back(vec);
    }
    m_FreeLists.shrink_to_fit();
    m_FreeLists[0].push_back(0);
}

uint16_t BuddyAllocator::getFittingLevel(VkDeviceSize size)
{
    uint16_t level = 0;
    VkDeviceSize maxSize = m_MemoryBlock.allocatedSize();

    for(uint16_t i = 0; i <= m_Subdivisions; ++i)
    {
        VkDeviceSize cSize = (maxSize / static_cast<uint64_t >(std::pow(2,i)));
        if(size <= cSize)
        {
            level = i;
            continue;
        }
        else
        {
            break;
        }
    }
    return level;
}

IMemBlockAllocator::MemHandle BuddyAllocator::allocateBlock(uint16_t level)
{
    //Allocate it!
    uint16_t blockIndex = m_FreeLists[level].back();
    m_FreeLists[level].pop_back();

    MemHandle handle = m_Allocations.getNewItem();

    uint16_t index = static_cast<uint16_t >(std::pow(2,level) - 1) + blockIndex;
    m_BinaryTree[index].m_Allocation = handle;
    m_BinaryTree[index].m_InFreeList = UINT16_MAX;
    pair<vk_memory_allocation, MemAllocInfo>& alloc = m_Allocations[handle];
    alloc.first.m_Size   = m_BinaryTree[index].m_Size;
    alloc.first.m_Memory = m_MemoryBlock.memory();
    alloc.first.m_Offset = blockIndex * alloc.first.m_Size;

    alloc.second.m_LevelIndex = blockIndex;
    alloc.second.m_Level = level;
    return handle;
}

bool BuddyAllocator::breakBlockFromLevel(uint16_t level)
{
    if(m_FreeLists[level].size() == 0) return false;

    uint16_t blockIndexToBreak = m_FreeLists[level].back();
    m_FreeLists[level].pop_back();

    uint16_t index = static_cast<uint16_t >(std::pow(2,level) - 1) + blockIndexToBreak;
    MemBlock& bBlock = m_BinaryTree[index];

    bBlock.m_Allocation = UINT16_MAX;
    bBlock.m_InFreeList = UINT16_MAX;

    uint16_t aIndex = static_cast<uint16_t >(std::pow(2,level+1) - 1) + (bBlock.m_LevelIndex + bBlock.m_LevelIndex);
    uint16_t bIndex = static_cast<uint16_t >(std::pow(2,level+1) - 1) + (bBlock.m_LevelIndex + bBlock.m_LevelIndex + static_cast<uint16_t>(1));

    MemBlock& a = m_BinaryTree[aIndex];
    MemBlock& b = m_BinaryTree[bIndex];
    a.m_InFreeList = UINT16_MAX - 1;
    b.m_InFreeList = UINT16_MAX - 1;
    a.m_Allocation = UINT16_MAX;
    b.m_Allocation = UINT16_MAX;

    m_FreeLists[level + 1].push_back(a.m_LevelIndex);
    m_FreeLists[level + 1].push_back(b.m_LevelIndex);

    return true;
}

int32_t BuddyAllocator::coalesceBlockFromLevel(uint16_t level, uint16_t levelIndex)
{
    uint16_t buddy = levelIndex % static_cast<uint16_t>(2) == 0 ? levelIndex + static_cast<uint16_t>(1) : levelIndex - static_cast<uint16_t>(1);

    if(!(buddy >= 0 && buddy < static_cast<uint16_t >(std::pow(2,level))))
    {
        Logger::failure("Some weird error occurred. Please check the code!");
        return -1;
    }
    uint16_t bBlock = static_cast<uint16_t >(std::pow(2,level) - 1) + buddy;
    uint16_t cBlock = static_cast<uint16_t >(std::pow(2,level) - 1) + levelIndex;

    MemBlock& cMB = m_BinaryTree[cBlock];
    MemBlock& bMB = m_BinaryTree[bBlock];

    if(bMB.m_InFreeList == UINT16_MAX)
        return -1; //Cannot coalesce

    //Coalesce time!

    //remove smaller blocks from free list
    m_FreeLists[level].erase(std::remove(std::begin(m_FreeLists[level]), std::end(m_FreeLists[level]), buddy), std::end(m_FreeLists[level]));
    m_FreeLists[level].erase(std::remove(std::begin(m_FreeLists[level]), std::end(m_FreeLists[level]), levelIndex), std::end(m_FreeLists[level]));

    //set their memblocks correctly...

    cMB.m_InFreeList = UINT16_MAX; bMB.m_InFreeList = UINT16_MAX;

    //get the parent
    uint16_t parentLevel = level - static_cast<uint16_t >(1);
    uint16_t parentLevelIndex = (buddy < levelIndex ? buddy : levelIndex) / static_cast<uint16_t >(2);
    uint16_t bParentBlockIndex = static_cast<uint16_t >(std::pow(2,parentLevel) - 1) + parentLevelIndex;

    MemBlock& block = m_BinaryTree[bParentBlockIndex];
    block.m_Allocation = UINT16_MAX;
    block.m_InFreeList = UINT16_MAX - 1;
    m_FreeLists[block.m_Level].push_back(block.m_LevelIndex);
    return block.m_LevelIndex;
}
