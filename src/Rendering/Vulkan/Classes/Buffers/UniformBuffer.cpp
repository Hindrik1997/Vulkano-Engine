//
// Created by hindrik on 16-4-17.
//

#include <cstring>
#include "UniformBuffer.h"

UniformBuffer::UniformBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkDeviceSize bufferSize,  VkCore &core, const vector<uint32_t> &queueFamilies)
        : m_Buffer(device,physicalDevice, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, queueFamilies),
          m_StagingBuffer(device, physicalDevice, bufferSize, queueFamilies), m_VkCore(core)
{
}

StagingBuffer &UniformBuffer::stagingBuffer()
{
    return m_StagingBuffer;
}

Buffer &UniformBuffer::buffer()
{
    return m_Buffer;
}

void UniformBuffer::updateGPUBuffer(void* data, VkDeviceSize size)
{
    void* dst;
    vkMapMemory(m_StagingBuffer.buffer().device(), m_StagingBuffer.buffer().deviceMemory(), 0, size, 0, &dst);
    memcpy(dst, data, size);
    vkUnmapMemory(m_StagingBuffer.buffer().device(), m_StagingBuffer.buffer().deviceMemory());

    m_VkCore.copyBuffer(m_StagingBuffer.buffer().buffer(), m_Buffer.buffer(), size);
}
