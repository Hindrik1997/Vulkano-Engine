//
// Created by hindrik on 16-6-17.
//

#ifndef VULKANOENGINE_MEMORYMANAGER_H
#define VULKANOENGINE_MEMORYMANAGER_H


#include "../VulkanPlatforms/VulkanPlatform.h"
#include "../Classes/CommandPool.h"
#include "../../Utilities/VulkanStructs.h"
#include "../../../Utility Classes/ThreadedResourceSet.h"
#include "../../../Core/AwaitableTask.h"
#include <utility>
#include <vector>

using std::vector;
using std::pair;

class Instance;
class PresentDevice;

class MemoryManager final
{
private:
    PresentDevice* m_Device;
public:
    MemoryManager()  = delete;
    MemoryManager(PresentDevice& presentDevice);
    ~MemoryManager() = default;

    MemoryManager(const MemoryManager&) = delete;
    MemoryManager(MemoryManager&&)      = default;

    MemoryManager& operator=(const MemoryManager&)   = delete;
    MemoryManager& operator=(MemoryManager&&)        = default;
private:
    static vector<vk_memory_heap> getMemoryInfo(VkPhysicalDevice device);






};


#endif //VULKANOENGINE_MEMORYMANAGER_H
