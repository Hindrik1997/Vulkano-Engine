//
// Created by hindrik on 26-6-2017.
//

#ifndef VULKANOENGINE_FENCEWAITER_H
#define VULKANOENGINE_FENCEWAITER_H

#include "../VulkanPlatforms/VulkanPlatform.h"


class FenceWaiter final
{
private:
    VkDevice        m_Device;
    VkFence         m_Fence;
    VkCommandBuffer m_CmdBuffer;
    VkCommandPool   m_Pool;
public:
    FenceWaiter(VkDevice device, VkFence fence, VkCommandBuffer buffer, VkCommandPool pool);
    FenceWaiter() = delete;
    ~FenceWaiter() = default;

    FenceWaiter(FenceWaiter&&)      = default;
    FenceWaiter(const FenceWaiter&) = default;

    FenceWaiter& operator=(const FenceWaiter&)  = default;
    FenceWaiter& operator=(FenceWaiter&&)       = default;

    void wait();
};


#endif //VULKANOENGINE_FENCEWAITER_H
