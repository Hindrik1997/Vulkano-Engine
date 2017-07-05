//
// Created by hindrik on 2-7-2017.
//

#include "MemoryBlock.h"
#include "../PresentDevice.h"
#include "../../Utilities/VulkanUtilityFunctions.h"

MemoryBlock::MemoryBlock(VkDevice device, VkDeviceSize allocateAmount, uint32_t memoryTypeIndex) : m_Device(device), m_AllocatedSize(allocateAmount), m_MemoryTypeIndex(memoryTypeIndex), m_AllocatedMemory({device, vkFreeMemory})
{
    allocateMemory();
}

void MemoryBlock::allocateMemory()
{
    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.pNext = nullptr;
    allocInfo.allocationSize = m_AllocatedSize;
    allocInfo.memoryTypeIndex = m_MemoryTypeIndex;

    VkResult result = vkAllocateMemory(m_Device,&allocInfo, nullptr, m_AllocatedMemory.reset());

    vkIfFailThrowMessage(result, "Error, could not allocate memory!");
}

MemoryBlock::MemoryBlock(VkDevice device, VkDeviceSize allocateAmount, uint32_t memoryTypeIndex, VkDeviceMemory memory) : m_Device(device), m_AllocatedSize(allocateAmount), m_MemoryTypeIndex(memoryTypeIndex), m_AllocatedMemory({memory, device, vkFreeMemory})
{

}

VkDeviceMemory MemoryBlock::memory() const {
    return m_AllocatedMemory;
}

uint32_t MemoryBlock::memoryTypeIndex() const {
    return m_MemoryTypeIndex;
}

VkDeviceSize MemoryBlock::allocatedSize() const {
    return m_AllocatedSize;
}

VkDevice MemoryBlock::device() const {
    return m_Device;
}
