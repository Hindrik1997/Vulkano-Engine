//
// Created by hindr on 26-6-2017.
//

#include "FenceWaiter.h"

FenceWaiter::FenceWaiter(VkDevice device, VkFence fence, VkCommandBuffer buffer, VkCommandPool pool) : m_Device(device), m_Fence(fence), m_CmdBuffer(buffer), m_Pool(pool)
{
}

void FenceWaiter::wait()
{
    vkWaitForFences(m_Device,1,&m_Fence,VK_TRUE,UINT64_MAX);
    vkFreeCommandBuffers(m_Device, m_Pool,1, &m_CmdBuffer);
}
