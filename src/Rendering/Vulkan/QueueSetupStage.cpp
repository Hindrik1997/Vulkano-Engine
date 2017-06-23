//
// Created by hindr on 23-6-2017.
//

#include "QueueSetupStage.h"

QueueSetupStage::QueueSetupStage(VkPhysicalDevice physicalDevice) : m_PhysicalDevice(physicalDevice)
{
    initialize();
}

void QueueSetupStage::initialize()
{

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, nullptr);

    vector<VkQueueFamilyProperties> queueFamilies;
    vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, queueFamilies.data());

    vector<VkQueueFamilyProperties> graphicsSupport;
    vector<VkQueueFamilyProperties> graphicsAndCompute;
    vector<VkQueueFamilyProperties> computeOnly;
    vector<VkQueueFamilyProperties> transferOnly;

    //assign the queue families in the proper sets.
    for(auto& fam : queueFamilies)
    {
        if(fam.queueCount > 0 && fam.queueFlags & VK_QUEUE_GRAPHICS_BIT && !(fam.queueFlags & VK_QUEUE_COMPUTE_BIT))
        {
            graphicsSupport.push_back(fam);
            continue;
        }
        if(fam.queueCount > 0 && fam.queueFlags & VK_QUEUE_GRAPHICS_BIT && fam.queueFlags & VK_QUEUE_COMPUTE_BIT)
        {
            graphicsAndCompute.push_back(fam);
            continue;
        }
        if(fam.queueCount > 0 && fam.queueFlags & VK_QUEUE_COMPUTE_BIT && !(fam.queueFlags & VK_QUEUE_GRAPHICS_BIT))
        {
            computeOnly.push_back(fam);
            continue;
        }
        if(fam.queueFlags > 0 && fam.queueFlags & VK_QUEUE_TRANSFER_BIT && !(fam.queueFlags & VK_QUEUE_GRAPHICS_BIT) && !(fam.queueFlags & VK_QUEUE_COMPUTE_BIT))
        {
            transferOnly.push_back(fam);
        }
    }







}
