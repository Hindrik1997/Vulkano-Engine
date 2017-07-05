//
// Created by hindrik on 26-6-2017.
//

#include "ResourceOperationManager.h"

ResourceOperationManager::ResourceOperationManager(PresentDevice& presentDevice) : m_PresentDevice(&presentDevice), m_CommandPool(presentDevice.device(), presentDevice.presentQueue().m_FamilyIndex, true, true)
{

}

void ResourceOperationManager::copyBufferOnPresentQueueBlocking(VkBuffer src, VkBuffer dst, VkDeviceSize size, VkDeviceSize srcOffset, VkDeviceSize dstOffset)
{
    VkCommandBuffer buffer = m_CommandPool.allocateCommandBuffer(CommandBufferLevel::Primary);

    VkFenceCreateInfo fenceCreateInfo = {};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.pNext = nullptr;
    fenceCreateInfo.flags = 0;

    VkUniqueHandle<VkFence> fence = {m_PresentDevice->device(), vkDestroyFence};
    vkCreateFence(m_PresentDevice->device(), &fenceCreateInfo, nullptr, fence.reset());

    VkCommandBufferBeginInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    info.pNext = nullptr;
    info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    VkBufferCopy copy = {};
    copy.srcOffset = srcOffset;
    copy.dstOffset = dstOffset;
    copy.size = size;

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = nullptr;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &buffer;


    vkBeginCommandBuffer(buffer, &info);

    vkCmdCopyBuffer(buffer, src, dst, 1, &copy);

    vkEndCommandBuffer(buffer);

    vkQueueSubmit(m_PresentDevice->presentQueue().m_Queue, 1, &submitInfo, fence);

    //Wait for transfer operation to be completed...
    vkWaitForFences(m_PresentDevice->device(), 1, &fence, VK_TRUE, UINT64_MAX);

    m_CommandPool.deallocateCommandBuffer(buffer);
}

FenceWaiter ResourceOperationManager::copyBufferOnPresentQueue(VkBuffer src, VkBuffer dst, VkDeviceSize size, VkDeviceSize srcOffset, VkDeviceSize dstOffset)
{
    VkCommandBuffer buffer = m_CommandPool.allocateCommandBuffer(CommandBufferLevel::Primary);

    VkFenceCreateInfo fenceCreateInfo = {};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.pNext = nullptr;
    fenceCreateInfo.flags = 0;

    VkUniqueHandle<VkFence> fence = {m_PresentDevice->device(), vkDestroyFence};
    vkCreateFence(m_PresentDevice->device(), &fenceCreateInfo, nullptr, fence.reset());

    VkCommandBufferBeginInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    info.pNext = nullptr;
    info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    VkBufferCopy copy = {};
    copy.srcOffset = srcOffset;
    copy.dstOffset = dstOffset;
    copy.size = size;

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = nullptr;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &buffer;


    vkBeginCommandBuffer(buffer, &info);

    vkCmdCopyBuffer(buffer, src, dst, 1, &copy);

    vkEndCommandBuffer(buffer);

    vkQueueSubmit(m_PresentDevice->presentQueue().m_Queue, 1, &submitInfo, fence);

    //Return sync object to caller
    FenceWaiter wf(m_PresentDevice->device(), fence, buffer, m_CommandPool.commandPool());
    FenceWaiter t = wf;
    return t;
}





//int main(int argc, char* argv[])
//{
//    int a = 0;
//    int b = 1;
//    int c = a + b;
//    printf("%d", c);
//}




