//
// Created by hindr on 23-6-2017.
//

#ifndef VULKANOENGINE_QUEUEMANAGER_H
#define VULKANOENGINE_QUEUEMANAGER_H

#include "VulkanPlatforms/VulkanPlatform.h"

class QueueSetupStage final
{
private:
    VkDevice m_Device;
    VkPhysicalDevice m_PhysicalDevice;

    vector<VkQueueFamilyProperties> m_Graphics;
    vector<VkQueueFamilyProperties> m_Compute;
    vector<VkQueueFamilyProperties> m_Transfer;

public:
    QueueSetupStage(VkPhysicalDevice physicalDevice);
private:
    void initialize();


};


#endif //VULKANOENGINE_QUEUEMANAGER_H
