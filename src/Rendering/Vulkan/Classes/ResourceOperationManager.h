//
// Created by hindr on 26-6-2017.
//

#ifndef VULKANOENGINE_RESOUREOPERATIONMANAGER_H
#define VULKANOENGINE_RESOUREOPERATIONMANAGER_H


#include "../PresentDevice.h"
#include "CommandPool.h"
#include "FenceWaiter.h"


class ResourceOperationManager final
{
private:
    PresentDevice* m_PresentDevice;
    CommandPool    m_CommandPool;
public:
    ResourceOperationManager(PresentDevice& presentDevice);

    void copyBufferOnPresentQueueBlocking(VkBuffer src, VkBuffer dst, VkDeviceSize size, VkDeviceSize srcOffset, VkDeviceSize dstOffset);
    FenceWaiter copyBufferOnPresentQueue (VkBuffer src, VkBuffer dst, VkDeviceSize size, VkDeviceSize srcOffset, VkDeviceSize dstOffset);

};


#endif //VULKANOENGINE_RESOUREOPERATIONMANAGER_H
