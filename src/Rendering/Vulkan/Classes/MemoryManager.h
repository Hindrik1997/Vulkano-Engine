//
// Created by hindrik on 16-6-17.
//

#ifndef VULKANOENGINE_MEMORYMANAGER_H
#define VULKANOENGINE_MEMORYMANAGER_H


#include "../VulkanPlatforms/VulkanPlatform.h"
#include "CommandPool.h"
#include "../../Utilities/VulkanStructs.h"
#include "../../../Utility Classes/ThreadedResourceSet.h"
#include "../../../Core/AwaitableTask.h"
#include <utility>
#include <vector>

using std::vector;
using std::pair;

class VkCore;


class MemoryManager final
{
public:
    MemoryManager()  = delete;
    MemoryManager(VkDevice device, const vector<vk_queue>& transferQueues);
    ~MemoryManager() = default;

    MemoryManager(const MemoryManager&) = delete;
    MemoryManager(MemoryManager&&)      = default;

    MemoryManager& operator=(const MemoryManager&)   = delete;
    MemoryManager& operator=(MemoryManager&&)        = default;
private:
    VkDevice                                m_Device;

    vector<CommandPool>                                       m_TransferCommandPools                          = {};
    ThreadedResourceSet<pair< vk_queue, uint32_t> >           m_TransferQueues;
private:
    vector<pair<vk_queue, uint32_t>> createQueueSet(const vector<vk_queue> &vector);
public:
    void copyBuffer(VkBuffer src, VkBuffer dst, VkDeviceSize size, VkDeviceSize srcOffset, VkDeviceSize dstOffset);

    void transitionBufferOwnershipAndAccesFlags(VkBuffer buffer, VkDeviceSize size, VkDeviceSize offset, uint32_t srcQueueFamilyIndex, uint32_t dstQueueFamilyIndex,VkAccessFlagBits srcAccessFlags, VkAccessFlagBits dstAccessFlags);









};


#endif //VULKANOENGINE_MEMORYMANAGER_H
