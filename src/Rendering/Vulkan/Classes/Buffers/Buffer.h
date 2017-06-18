//
// Created by hindrik on 15-4-17.
//

#ifndef VULKANOENGINE_BUFFER_H
#define VULKANOENGINE_BUFFER_H

#include "../../VkUniqueHandle.h"
#include <vector>

using std::vector;

class Buffer final
{
private:
    VkUniqueHandle<VkBuffer> m_Buffer;
    VkUniqueHandle<VkDeviceMemory> m_DeviceAllocatedMemory;
    VkDevice m_Device;
    VkPhysicalDevice m_PhysicalDevice;
    VkDeviceSize m_BufferSize;
    VkBufferUsageFlags m_UsageFlags;
    VkMemoryPropertyFlags m_MemoryFlags;
    VkSharingMode m_SharingMode;
    VkMemoryRequirements m_AllocatedMemoryRequirements;
public:
    Buffer(VkDevice device, VkPhysicalDevice physicalDevice, VkDeviceSize bufferSize, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryFlags);
    Buffer(VkDevice device, VkPhysicalDevice physicalDevice, VkDeviceSize bufferSize, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryFlags, const vector<uint32_t>& queueFamilies);


    Buffer()  = delete;
    ~Buffer() = default;

    Buffer(const Buffer&)   = delete;
    Buffer(Buffer&&)        = default;

    Buffer& operator=(const Buffer&)    = delete;
    Buffer& operator=(Buffer&&)         = default;



public:
    VkBuffer buffer();
    VkDeviceMemory deviceMemory();
    VkDevice device();
    VkPhysicalDevice physicalDevice();
    VkDeviceSize bufferSize();
    VkBufferUsageFlags usageFlags();
    VkMemoryPropertyFlags memoryFlags();
    VkSharingMode sharingMode();
    VkMemoryRequirements memoryRequirements();
};


#endif //VULKANOENGINE_BUFFER_H
