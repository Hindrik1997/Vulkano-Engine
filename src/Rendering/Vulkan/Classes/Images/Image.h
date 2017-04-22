//
// Created by hindrik on 22-4-17.
//

#ifndef VULKANOENGINE_IMAGE_H
#define VULKANOENGINE_IMAGE_H

#include "../../VulkanPlatforms/VulkanPlatform.h"
#include "../../VkUniqueHandle.h"

class Image
{
private:
    VkUniqueHandle<VkImage>         m_Image;
    VkUniqueHandle<VkDeviceMemory>  m_Memory;
    VkImageType                     m_Type;
    VkExtent3D                      m_Extent;
    uint32_t                        m_MipLevels;
    uint32_t                        m_ArrayLayers;
    VkFormat                        m_Format;
    VkImageTiling                   m_Tiling;
    VkImageUsageFlags               m_UsageFlags;
    VkSharingMode                   m_SharingMode;
    VkSampleCountFlagBits           m_SampleCount;
    VkDevice                        m_Device;
    VkPhysicalDevice                m_PhysicalDevice;
    VkMemoryPropertyFlags           m_MemoryPropertyFlags;
public:
    Image(VkDevice device, VkPhysicalDevice physicalDevice, VkImageType imageType, VkExtent3D imageSize, uint32_t mipCount, uint32_t arrayLayers, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkSampleCountFlagBits sampleCount, VkImageLayout initialLayout, VkMemoryPropertyFlags memoryPropertyFlags);
    Image(VkDevice device, VkPhysicalDevice physicalDevice, VkImageType imageType, VkExtent3D imageSize, uint32_t mipCount, uint32_t arrayLayers, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkSampleCountFlagBits sampleCount, VkImageLayout initialLayout, VkMemoryPropertyFlags memoryPropertyFlags, const vector<uint32_t>& queueFamilies);

    ~Image() = default;
    Image(const Image&) = delete;
    Image(Image&&) = default;
    Image& operator=(const Image&) = delete;
    Image& operator=(Image&&) = default;
};


#endif //VULKANOENGINE_IMAGE_H
