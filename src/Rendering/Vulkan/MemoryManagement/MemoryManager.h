//
// Created by hindrik on 16-6-17.
//

#ifndef VULKANOENGINE_MEMORYMANAGER_H
#define VULKANOENGINE_MEMORYMANAGER_H


#include "../VulkanPlatforms/VulkanPlatform.h"
#include "../Classes/CommandPool.h"
#include "../../Utilities/VulkanStructs.h"
#include "../../../Utility Classes/ThreadedResourceSet.h"
#include "../../../Core/AwaitableTask.h"
#include "../../../Data Storage/MemBlockAllocator.h"
#include "MemoryBlock.h"
#include "BuddyAllocator.h"
#include <utility>
#include <vector>

using std::vector;
using std::pair;

class Instance;
class PresentDevice;

class MemoryManager final
{
private:
    PresentDevice*          m_Device;
    vector<MemoryBlock>     m_DeviceLocalMemoryBlocks;
    vector<MemoryBlock>     m_DeviceLocalHostVisibleMemoryBlocks;
    vector<MemoryBlock>     m_HostLocalHostCoherentHostCachedMemoryBlocks;
    vector<MemoryBlock>     m_HostLocalHostCoherentMemoryBlocks;
    vector<MemoryBlock>     m_HostLocalCachedMemoryBlocks;

    vector<BuddyAllocator>  m_Allocators;
public:
    MemoryManager()  = delete;
    MemoryManager(PresentDevice& presentDevice);
    ~MemoryManager() = default;

    MemoryManager(const MemoryManager&) = delete;
    MemoryManager(MemoryManager&&)      = default;

    MemoryManager& operator=(const MemoryManager&)   = delete;
    MemoryManager& operator=(MemoryManager&&)        = default;
private:
    static vector<vk_memory_heap> getMemoryInfo(VkPhysicalDevice device);
    static vector<MemoryBlock>    allocateMemblocks(VkDevice device, const vk_memory_heap &heap, VkDeviceSize allocateSize, uint32_t memoryTypeIndex, uint32_t blockCount);
    static vector<MemoryBlock>    fillMemoryHeap(VkDevice device, const vk_memory_heap& heap, VkDeviceSize allocateSize, uint32_t memoryTypeIndex, VkDeviceSize& alreadyAllocatedAmount);
    void   initializeMemorySystem();
    void   setupDeviceLocalHeap(const vk_memory_heap &heap);
    void   setupHostLocalHeap  (const vk_memory_heap& heap);
};


#endif //VULKANOENGINE_MEMORYMANAGER_H
