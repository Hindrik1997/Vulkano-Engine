//
// Created by hindrik on 16-4-17.
//

#ifndef VULKANOENGINE_UNIFORMBUFFER_H
#define VULKANOENGINE_UNIFORMBUFFER_H

#include "../Vertex.h"
#include "../../VkUniqueHandle.h"
#include "Buffer.h"
#include "StagingBuffer.h"
#include "../../Instance.h"
#include <vector>

class UniformBuffer final
{
private:
    StagingBuffer   m_StagingBuffer;
    Buffer          m_Buffer;
    Instance&         m_VkCore;
public:
    UniformBuffer(VkDevice device,VkPhysicalDevice physicalDevice, VkDeviceSize bufferSize, Instance& core, const vector<uint32_t>& queueFamilies);

    UniformBuffer()  = delete;
    ~UniformBuffer() = default;

    UniformBuffer(const UniformBuffer&)   = delete;
    UniformBuffer(UniformBuffer&&)        = default;

    UniformBuffer& operator=(const UniformBuffer&)    = delete;
    UniformBuffer& operator=(UniformBuffer&&)         = default;

public:
    Buffer& buffer();
    StagingBuffer& stagingBuffer();

    void updateGPUBuffer(void* data, VkDeviceSize size);


};


#endif //VULKANOENGINE_UNIFORMBUFFER_H
