//
// Created by hindrik on 10-4-17.
//

#include "CommandPool.h"
#include "../../Utilities/VulkanUtilityFunctions.h"
#include "../../../Core/Logger.h"

CommandPool::CommandPool(VkDevice device, uint32_t queueFamilyIndex, bool isTransient, bool individualReset) : m_IsIndividualRessetable(individualReset), m_IsTransient(isTransient), m_QueueFamilyIndex(queueFamilyIndex), m_Device(device), m_Commandpool({m_Device, vkDestroyCommandPool})
{
    VkCommandPoolCreateInfo info = {};

    info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    info.pNext = nullptr;
    info.queueFamilyIndex = m_QueueFamilyIndex;

    if(m_IsTransient && m_IsIndividualRessetable)
        info.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    else if(m_IsTransient)
        info.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
    else if(m_IsIndividualRessetable)
        info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    else
        info.flags = 0;


    VkResult result = vkCreateCommandPool(m_Device, &info, nullptr, m_Commandpool.reset());
    vkIfFailThrowMessage(result, "Failed to create command pool!");
}

vector<VkCommandBuffer> CommandPool::allocateCommandBuffers(uint32_t amount, CommandBufferLevel level)
{
    VkCommandBufferAllocateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    info.pNext = nullptr;
    info.commandBufferCount = amount;
    info.commandPool = m_Commandpool;


    vector<VkCommandBuffer> commandBuffers;
    commandBuffers.resize(amount);

    VkResult result = vkAllocateCommandBuffers(m_Device, &info, commandBuffers.data());
    vkIfFailThrowMessage(result, "Command buffer allocation failed!");

    return commandBuffers;
}

VkCommandBuffer CommandPool::allocateCommandBuffer(CommandBufferLevel level)
{
    vector<VkCommandBuffer> buf = allocateCommandBuffers(1,level);
    return buf.front();
}

bool CommandPool::isIndividualRessetable() const
{
    return m_IsIndividualRessetable;
}

bool CommandPool::isTransient() const
{
    return m_IsTransient;
}

uint32_t CommandPool::queueFamilyIndex() const
{
    return m_QueueFamilyIndex;
}

VkDevice CommandPool::device() const
{
    return m_Device;
}

void CommandPool::deallocateCommandBuffers(const vector<VkCommandBuffer> buffers)
{
    if(buffers.size() > 0)
    {
        vkFreeCommandBuffers(m_Device, m_Commandpool, static_cast<uint32_t >(buffers.size()), buffers.data());
    }
}

void CommandPool::deallocateCommandBuffer(VkCommandBuffer buffer)
{
    vector<VkCommandBuffer> vec = {buffer};
    deallocateCommandBuffers(vec);
}

VkCommandPool CommandPool::commandPool() const
{
    return m_Commandpool;
}
