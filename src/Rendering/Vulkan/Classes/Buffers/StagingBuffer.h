//
// Created by hindrik on 15-4-17.
//

#ifndef VULKANOENGINE_STAGINGBUFFER_H
#define VULKANOENGINE_STAGINGBUFFER_H

#include "../../VkUniqueHandle.h"
#include "Buffer.h"
#include <vector>

class StagingBuffer final
{
private:
    Buffer m_Buffer;
public:
    StagingBuffer(VkDevice device,VkPhysicalDevice physicalDevice, VkDeviceSize bufferSize, const vector<uint32_t>& queueFamilies);

    StagingBuffer()  = delete;
    ~StagingBuffer() = default;

    StagingBuffer(const StagingBuffer&)   = delete;
    StagingBuffer(StagingBuffer&&)        = default;

    StagingBuffer& operator=(const StagingBuffer&)    = delete;
    StagingBuffer& operator=(StagingBuffer&&)         = default;

public:
    Buffer& buffer();
};


#endif //VULKANOENGINE_STAGINGBUFFER_H
