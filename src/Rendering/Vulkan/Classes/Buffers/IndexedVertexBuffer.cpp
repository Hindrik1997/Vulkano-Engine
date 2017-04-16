//
// Created by hindrik on 16-4-17.
//

#include <cstring>
#include "IndexedVertexBuffer.h"

Buffer &IndexedVertexBuffer::buffer()
{
    return m_Buffer;
}

IndexedVertexBuffer::IndexedVertexBuffer(VkDevice device, VkPhysicalDevice physicalDevice, const vector<Vertex> &vertices, const vector<uint16_t> &indices, VkCore &core, const vector<uint32_t> &queueFamilies)
        : m_Buffer(
        device,
        physicalDevice,
        static_cast<VkDeviceSize>(sizeof(Vertex) * vertices.size() + sizeof(uint16_t) * indices.size()),
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_SHARING_MODE_CONCURRENT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, queueFamilies
)
{
    m_IndicesCount = static_cast<uint32_t >(indices.size());
    m_IndexOffset = static_cast<VkDeviceSize>(sizeof(Vertex) * vertices.size());
    StagingBuffer stagingBuffer(device, physicalDevice, m_Buffer.bufferSize(), queueFamilies);
    //Map memory
    void* data;
    vkMapMemory(stagingBuffer.buffer().device(), stagingBuffer.buffer().deviceMemory(), 0, stagingBuffer.buffer().memoryRequirements().size, 0, &data);

    //Copy data to memory
    memcpy(data, vertices.data(), m_IndexOffset);
    memcpy(static_cast<uint8_t*>(data) + m_IndexOffset, indices.data(), (sizeof(uint16_t) * indices.size()));

    //Unmap it again
    vkUnmapMemory(stagingBuffer.buffer().device(), stagingBuffer.buffer().deviceMemory());

    core.copyBuffer(stagingBuffer.buffer().buffer(), m_Buffer.buffer(), m_Buffer.bufferSize());
}

IndexedVertexBuffer::IndexedVertexBuffer(VkDevice device, VkPhysicalDevice physicalDevice, const vector<Vertex> &vertices, const vector<uint16_t> &indices)
        : m_Buffer(
        device,
        physicalDevice,
        static_cast<VkDeviceSize>(sizeof(Vertex) * vertices.size()),
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        VK_SHARING_MODE_EXCLUSIVE,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
)
{
    m_IndicesCount = static_cast<uint32_t >(indices.size());
    m_IndexOffset = static_cast<VkDeviceSize>(sizeof(Vertex) * vertices.size());

    //Map memory
    void* data;
    vkMapMemory(m_Buffer.device(), m_Buffer.deviceMemory(), 0, m_Buffer.memoryRequirements().size, 0, &data);

    //Copy data to memory
    memcpy(data, vertices.data(), m_IndexOffset);
    memcpy(static_cast<uint8_t*>(data) + m_IndexOffset, indices.data(), (sizeof(uint16_t) * indices.size()));

    //Unmap it again
    vkUnmapMemory(m_Buffer.device(), m_Buffer.deviceMemory());

}

VkDeviceSize IndexedVertexBuffer::indexOffset()
{
    return m_IndexOffset;
}

uint32_t IndexedVertexBuffer::indicesCount()
{
    return m_IndicesCount;
}
