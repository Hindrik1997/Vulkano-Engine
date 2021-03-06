//
// Created by hindrik on 10-4-17.
//

#ifndef VULKANOENGINE_COMMANDPOOL_H
#define VULKANOENGINE_COMMANDPOOL_H

#include "../VulkanPlatforms/VulkanPlatform.h"
#include "../VkUniqueHandle.h"
#include "../../Utilities/VulkanEnums.h"

class CommandPool final
{
private:
    VkDevice m_Device;
    uint32_t m_QueueFamilyIndex;
    bool m_IsTransient;
    bool m_IsIndividualRessetable;
    VkUniqueHandle<VkCommandPool> m_Commandpool;
public:
    CommandPool(VkDevice device, uint32_t queueFamilyIndex, bool isTransient = false, bool individualReset = false);
    CommandPool(const CommandPool&) = delete;
    CommandPool(CommandPool&&) = default;
    CommandPool& operator=(const CommandPool&) = delete;
    CommandPool& operator=(CommandPool&&) = default;
    ~CommandPool() = default;
public:
    VkCommandBuffer         allocateCommandBuffer(CommandBufferLevel level);
    vector<VkCommandBuffer> allocateCommandBuffers(uint32_t amount, CommandBufferLevel level);

    void deallocateCommandBuffer(VkCommandBuffer buffer);
    void deallocateCommandBuffers(const vector<VkCommandBuffer> buffers);

    VkCommandPool commandPool() const;
    VkDevice device() const;
    uint32_t queueFamilyIndex() const;
    bool     isTransient() const;
    bool     isIndividualRessetable() const;
};


#endif //VULKANOENGINE_COMMANDPOOL_H
