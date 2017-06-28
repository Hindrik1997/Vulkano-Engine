//
// Created by hindrik on 16-6-17.
//

#include "MemoryManager.h"
#include "../PresentDevice.h"
#include "../../Utilities/VulkanUtilityFunctions.h"

MemoryManager::MemoryManager(PresentDevice& presentDevice) : m_Device(&presentDevice)
{
    getMemoryInfo(m_Device->physicalDevice());
}

vector<vk_memory_heap> MemoryManager::getMemoryInfo(VkPhysicalDevice device)
{
    vector<vk_memory_heap> heaps = {};

    VkPhysicalDeviceMemoryProperties memoryProperties = {};
    vkGetPhysicalDeviceMemoryProperties(device, &memoryProperties);

    for(uint32_t i = 0; i < memoryProperties.memoryHeapCount; ++i)
    {

        vk_memory_heap heap = {};
        heap.m_MemoryHeap = memoryProperties.memoryHeaps[i];

        heap.m_IsDeviceLocal = (heap.m_MemoryHeap.flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) != 0;

#ifndef NDEBUG
        
        std::stringstream str;
        str << std::endl << heap;

        Logger::log(str.str());

#endif

    }






    return heaps;
}