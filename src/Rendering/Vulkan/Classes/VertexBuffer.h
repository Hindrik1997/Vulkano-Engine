//
// Created by hindrik on 15-4-17.
//

#ifndef VULKANOENGINE_VERTEXBUFFER_H
#define VULKANOENGINE_VERTEXBUFFER_H


#include "Vertex.h"
#include "../VkUniqueHandle.h"
#include "Buffer.h"
#include <vector>

using std::vector;

class VertexBuffer final
{
private:
    Buffer m_Buffer;
public:
    VertexBuffer(VkDevice device,VkPhysicalDevice physicalDevice, const vector<Vertex>& vertices);

    VertexBuffer()  = delete;
    ~VertexBuffer() = default;

    VertexBuffer(const VertexBuffer&)   = delete;
    VertexBuffer(VertexBuffer&&)        = default;

    VertexBuffer& operator=(const VertexBuffer&)    = delete;
    VertexBuffer& operator=(VertexBuffer&&)         = default;

public:
    Buffer& buffer();
};


#endif //VULKANOENGINE_VERTEXBUFFER_H
