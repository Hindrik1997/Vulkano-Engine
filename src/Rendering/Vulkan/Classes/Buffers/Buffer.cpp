//
// Created by hindrik on 15-4-17.
//

#include "Buffer.h"
#include "../../../Utilities/VulkanUtilityFunctions.h"

VkBuffer Buffer::buffer()
{
    return m_Buffer;
}

VkDeviceMemory Buffer::deviceMemory()
{
    return m_DeviceAllocatedMemory;
}

VkDevice Buffer::device()
{
    return m_Device;
}

VkPhysicalDevice Buffer::physicalDevice()
{
    return m_PhysicalDevice;
}

VkDeviceSize Buffer::bufferSize()
{
    return m_BufferSize;
}

VkMemoryRequirements Buffer::memoryRequirements()
{
    return m_AllocatedMemoryRequirements;
}

VkSharingMode Buffer::sharingMode()
{
    return m_SharingMode;
}

VkMemoryPropertyFlags Buffer::memoryFlags()
{
    return m_MemoryFlags;
}

VkBufferUsageFlags Buffer::usageFlags()
{
    return m_UsageFlags;
}

Buffer::Buffer(VkDevice device, VkPhysicalDevice physicalDevice, VkDeviceSize bufferSize, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryFlags)
        : m_Buffer({device, vkDestroyBuffer}),
          m_DeviceAllocatedMemory({device, vkFreeMemory}),
          m_Device(device),
          m_PhysicalDevice(physicalDevice),
          m_BufferSize(bufferSize),
          m_UsageFlags(usageFlags),
          m_SharingMode(VK_SHARING_MODE_EXCLUSIVE),
          m_MemoryFlags(memoryFlags)
{
    //Create buffer

    VkBufferCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    info.pNext = nullptr;
    info.size = m_BufferSize;
    info.usage = m_UsageFlags;
    info.sharingMode = m_SharingMode;
    info.flags = 0;

    VkResult result = vkCreateBuffer(m_Device, &info, nullptr, m_Buffer.reset());
    vkIfFailThrowMessage(result, "Vertex buffer creation failed!");

    //Allocate device memory

    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(m_Device, m_Buffer, &memoryRequirements);

    m_AllocatedMemoryRequirements = memoryRequirements;

    uint32_t memoryTypeIndex = getSuitableMemoryType(m_PhysicalDevice, memoryRequirements.memoryTypeBits, m_MemoryFlags);

    VkMemoryAllocateInfo memoryAllocateInfo = {};
    memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocateInfo.pNext = nullptr;
    memoryAllocateInfo.allocationSize = m_AllocatedMemoryRequirements.size;
    memoryAllocateInfo.memoryTypeIndex = memoryTypeIndex;

    result = vkAllocateMemory(m_Device, &memoryAllocateInfo, nullptr, m_DeviceAllocatedMemory.reset());
    vkIfFailThrowMessage(result, "PresentDevice memory allocation failed!");

    //Bind the memory to the buffer
    vkBindBufferMemory(m_Device, m_Buffer, m_DeviceAllocatedMemory, 0);
}

Buffer::Buffer(VkDevice device, VkPhysicalDevice physicalDevice, VkDeviceSize bufferSize, VkBufferUsageFlags usageFlags,
               VkMemoryPropertyFlags memoryFlags, const vector<uint32_t>& queueFamilies)
        : m_Buffer({device, vkDestroyBuffer}),
          m_DeviceAllocatedMemory({device, vkFreeMemory}),
          m_Device(device),
          m_PhysicalDevice(physicalDevice),
          m_BufferSize(bufferSize),
          m_UsageFlags(usageFlags),
          m_SharingMode(VK_SHARING_MODE_CONCURRENT),
          m_MemoryFlags(memoryFlags)
{
    //Create buffer

    VkBufferCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    info.pNext = nullptr;
    info.size = m_BufferSize;
    info.usage = m_UsageFlags;
    info.sharingMode = m_SharingMode;
    info.queueFamilyIndexCount = static_cast<uint32_t >(queueFamilies.size());
    info.pQueueFamilyIndices = queueFamilies.data();
    info.flags = 0;

    VkResult result = vkCreateBuffer(m_Device, &info, nullptr, m_Buffer.reset());
    vkIfFailThrowMessage(result, "Vertex buffer creation failed!");

    //Allocate device memory

    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(m_Device, m_Buffer, &memoryRequirements);

    m_AllocatedMemoryRequirements = memoryRequirements;

    uint32_t memoryTypeIndex = getSuitableMemoryType(m_PhysicalDevice, memoryRequirements.memoryTypeBits, m_MemoryFlags);

    VkMemoryAllocateInfo memoryAllocateInfo = {};
    memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocateInfo.pNext = nullptr;
    memoryAllocateInfo.allocationSize = m_AllocatedMemoryRequirements.size;
    memoryAllocateInfo.memoryTypeIndex = memoryTypeIndex;

    result = vkAllocateMemory(m_Device, &memoryAllocateInfo, nullptr, m_DeviceAllocatedMemory.reset());
    vkIfFailThrowMessage(result, "PresentDevice memory allocation failed!");

    //Bind the memory to the buffer
    vkBindBufferMemory(m_Device, m_Buffer, m_DeviceAllocatedMemory, 0);
}
