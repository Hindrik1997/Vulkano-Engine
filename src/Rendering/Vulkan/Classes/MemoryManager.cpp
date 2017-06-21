//
// Created by hindrik on 16-6-17.
//

#include "MemoryManager.h"
#include "../VkCore.h"

MemoryManager::MemoryManager(VkDevice device, const vector<vk_queue>& transferQueues) : m_Device(device), m_TransferQueues(createQueueSet(transferQueues))
{

}

vector< pair<vk_queue, uint32_t> > MemoryManager::createQueueSet(const vector<vk_queue> &vector)
{
    std::vector< std::pair<vk_queue, uint32_t > > returnVal;

    uint32_t i = 0;
    for(auto vk : vector)
    {
        m_TransferCommandPools.push_back(CommandPool(m_Device, vk.m_FamilyIndex, true, false));
        returnVal.emplace_back(std::make_pair(vk, i));
        ++i;
    }

    return returnVal;
}

void MemoryManager::copyBuffer(VkBuffer src, VkBuffer dst, VkDeviceSize size, VkDeviceSize srcOffset, VkDeviceSize dstOffset)
{
    m_TransferQueues.acquireAndRun([&, src,dst,size,srcOffset,dstOffset](pair<vk_queue, uint32_t > resource)
    {
        VkCommandBuffer buffer = m_TransferCommandPools[resource.second].allocateCommandBuffer(CommandBufferLevel::Primary);

        VkFenceCreateInfo fenceCreateInfo = {};
        fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceCreateInfo.pNext = nullptr;
        fenceCreateInfo.flags = 0;

        VkUniqueHandle<VkFence> fence = {m_Device, vkDestroyFence};
        vkCreateFence(m_Device, &fenceCreateInfo, nullptr, fence.reset());

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



        vkQueueSubmit(resource.first.m_Queue, 1, &submitInfo, fence);

        //Wait for transfer operation to be completed...
        vkWaitForFences(m_Device, 1, &fence, VK_TRUE, UINT64_MAX);

        m_TransferCommandPools[resource.second].deallocateCommandBuffer(buffer);
    });
}

void MemoryManager::transitionBufferOwnershipAndAccesFlags(VkBuffer buffer, VkDeviceSize size, VkDeviceSize offset,  uint32_t srcQueueFamilyIndex, uint32_t dstQueueFamilyIndex, VkAccessFlagBits srcAccessFlags, VkAccessFlagBits dstAccessFlags)
{
    // set up the structure
    VkBufferMemoryBarrier barrier   = {};
    barrier.sType                   = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
    barrier.pNext                   = nullptr;
    barrier.offset                  = offset;
    barrier.size                    = size;
    barrier.srcAccessMask           = srcAccessFlags;
    barrier.dstAccessMask           = dstAccessFlags;
    barrier.srcQueueFamilyIndex     = srcQueueFamilyIndex;
    barrier.dstQueueFamilyIndex     = dstQueueFamilyIndex;

    // execute the release of the resource
    m_TransferQueues.acquireAndRun([&, buffer](pair<vk_queue, uint32_t > resource){

        VkCommandBuffer cBuff = m_TransferCommandPools[resource.second].allocateCommandBuffer(CommandBufferLevel::Primary);

        VkFenceCreateInfo fenceCreateInfo = {};
        fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceCreateInfo.pNext = nullptr;
        fenceCreateInfo.flags = 0;

        VkUniqueHandle<VkFence> fence = {m_Device, vkDestroyFence};
        vkCreateFence(m_Device, &fenceCreateInfo, nullptr, fence.reset());

        VkCommandBufferBeginInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        info.pNext = nullptr;
        info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.pNext = nullptr;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &cBuff;

        vkBeginCommandBuffer(cBuff, &info);

        vkCmdPipelineBarrier(cBuff, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, 0,nullptr,1, &barrier, 0, nullptr);

        vkEndCommandBuffer(cBuff);

        vkQueueSubmit(resource.first.m_Queue, 1, &submitInfo, fence);

        //Wait for transfer operation to be completed...
        vkWaitForFences(m_Device, 1, &fence, VK_TRUE, UINT64_MAX);

        m_TransferCommandPools[resource.second].deallocateCommandBuffer(cBuff);

    });

    //Aqcuire it again at the destination queue family

}

