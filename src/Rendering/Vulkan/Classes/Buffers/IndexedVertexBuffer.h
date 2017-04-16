//
// Created by hindrik on 16-4-17.
//

#ifndef VULKANOENGINE_INDEXEDVERTEXBUFFER_H
#define VULKANOENGINE_INDEXEDVERTEXBUFFER_H

#include "../Vertex.h"
#include "../../VkUniqueHandle.h"
#include "Buffer.h"
#include "StagingBuffer.h"
#include "../../VkCore.h"
#include <vector>

using std::vector;

class IndexedVertexBuffer final
{
private:
    Buffer m_Buffer;
    VkDeviceSize m_IndexOffset;
    uint32_t m_IndicesCount;
public:
    IndexedVertexBuffer(VkDevice device,VkPhysicalDevice physicalDevice, const vector<Vertex>& vertices, const vector<uint16_t>& indices);
    IndexedVertexBuffer(VkDevice device,VkPhysicalDevice physicalDevice, const vector<Vertex>& vertices, const vector<uint16_t>& indices, VkCore& core, const vector<uint32_t>& queueFamilies);

    IndexedVertexBuffer()  = delete;
    ~IndexedVertexBuffer() = default;

    IndexedVertexBuffer(const IndexedVertexBuffer&)   = delete;
    IndexedVertexBuffer(IndexedVertexBuffer&&)        = default;

    IndexedVertexBuffer& operator=(const IndexedVertexBuffer&)    = delete;
    IndexedVertexBuffer& operator=(IndexedVertexBuffer&&)         = default;
public:
    Buffer& buffer();
    VkDeviceSize indexOffset();
    uint32_t indicesCount();
};


#endif //VULKANOENGINE_INDEXEDVERTEXBUFFER_H
