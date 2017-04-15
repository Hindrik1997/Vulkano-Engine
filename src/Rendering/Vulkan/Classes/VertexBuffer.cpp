//
// Created by hindrik on 15-4-17.
//

#include <cstring>
#include "VertexBuffer.h"
#include "../../Utilities/VulkanUtilityFunctions.h"

VertexBuffer::VertexBuffer(VkDevice device, VkPhysicalDevice physicalDevice, const vector<Vertex> &vertices)
        : m_Buffer(
        device,
        physicalDevice,
        static_cast<VkDeviceSize>(sizeof(Vertex) * vertices.size()),
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_SHARING_MODE_EXCLUSIVE,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        )
{
    //Map memory
    void* data;
    vkMapMemory(m_Buffer.device(), m_Buffer.deviceMemory(), 0, m_Buffer.memoryRequirements().size, 0, &data);

    //Copy data to memory
    memcpy(data, vertices.data(), (sizeof(Vertex) * vertices.size()));

    //Unmap it again
    vkUnmapMemory(m_Buffer.device(), m_Buffer.deviceMemory());

    //flush it?
}

Buffer& VertexBuffer::buffer()
{
    return m_Buffer;
}