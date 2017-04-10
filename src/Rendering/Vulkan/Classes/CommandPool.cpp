//
// Created by hindrik on 10-4-17.
//

#include "CommandPool.h"
#include "../../Utilities/VulkanUtilityFunctions.h"

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


    if (vkCreateCommandPool(m_Device, &info, nullptr, m_Commandpool.reset()) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create the command pool!");
    }
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

    for(const auto& i : commandBuffers)
    {
        m_Commandbuffers.push_back(i);
    }
    return commandBuffers;
}

VkCommandBuffer CommandPool::allocateCommandBuffer(CommandBufferLevel level)
{
    vector<VkCommandBuffer> buf = allocateCommandBuffers(1,level);
    return buf.front();
}

vector<VkCommandBuffer> CommandPool::commandBuffers() const
{
    return m_Commandbuffers;
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
