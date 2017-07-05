//
// Created by hindrik on 2-7-2017.
//

#ifndef VULKANOENGINE_VRAMBUFFER_H
#define VULKANOENGINE_VRAMBUFFER_H

#include "../VulkanPlatforms/VulkanPlatform.h"
#include "../VkUniqueHandle.h"
#include "../../Utilities/VulkanStructs.h"

class MemoryBlock final
{
private:
    VkUniqueHandle<VkDeviceMemory>  m_AllocatedMemory;
    VkDevice                        m_Device;
    VkDeviceSize                    m_AllocatedSize;
    uint32_t                        m_MemoryTypeIndex;
public:
    MemoryBlock(VkDevice device, VkDeviceSize allocateAmount, uint32_t memoryTypeIndex);
    MemoryBlock(VkDevice device, VkDeviceSize allocateAmount, uint32_t memoryTypeIndex, VkDeviceMemory memory);
    ~MemoryBlock() = default;

    MemoryBlock(const MemoryBlock&) = delete;
    MemoryBlock(MemoryBlock&&) = default;

    MemoryBlock& operator=(const MemoryBlock&) = delete;
    MemoryBlock& operator=(MemoryBlock&&) = default;


private:
    void allocateMemory();
public:
    VkDevice device() const;
    VkDeviceSize allocatedSize() const;
    uint32_t memoryTypeIndex() const;
    VkDeviceMemory memory() const;

};


#endif //VULKANOENGINE_VRAMBUFFER_H
