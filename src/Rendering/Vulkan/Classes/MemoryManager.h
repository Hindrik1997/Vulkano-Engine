//
// Created by hindrik on 16-6-17.
//

#ifndef VULKANOENGINE_MEMORYMANAGER_H
#define VULKANOENGINE_MEMORYMANAGER_H


#include "../VulkanPlatforms/VulkanPlatform.h"
#include "CommandPool.h"

class VkCore;


class MemoryManager final
{
public:
    MemoryManager()  = delete;
    MemoryManager(VkCore& vkCore);
    ~MemoryManager() = default;

    MemoryManager(const MemoryManager&) = delete;
    MemoryManager(MemoryManager&&)      = default;

    MemoryManager& operator=(const MemoryManager&)   = delete;
    MemoryManager& operator=(MemoryManager&&)        = default;
private:
    VkCore&                                 m_VkCore;
    vector<CommandPool>                     m_TransferCommandPools                          = {};









};


#endif //VULKANOENGINE_MEMORYMANAGER_H
