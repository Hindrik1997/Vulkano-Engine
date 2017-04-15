//
// Created by hindrik on 15-4-17.
//

#include "StagingBuffer.h"

#include <cstring>
#include "StagingBuffer.h"
#include "../../../Utilities/VulkanUtilityFunctions.h"

StagingBuffer::StagingBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkDeviceSize bufferSize, const vector<uint32_t>& queueFamilies)
        : m_Buffer(
        device,
        physicalDevice,
        bufferSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_SHARING_MODE_EXCLUSIVE,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        queueFamilies
)
{
}

Buffer& StagingBuffer::buffer()
{
    return m_Buffer;
}