//
// Created by hindrik on 22-4-17.
//

#include "Image3D.h"

Image3D::Image3D(VkDevice device, VkPhysicalDevice physicalDevice, uint32_t width, uint32_t height, uint32_t depth, uint32_t mipCount,
                 VkFormat format, VkSampleCountFlagBits sampleCount)
            : m_Image(device,physicalDevice, VK_IMAGE_TYPE_3D, VkExtent3D{width,height,depth}, mipCount,1, format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT, sampleCount, VK_IMAGE_LAYOUT_PREINITIALIZED, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
{

}

Image3D::Image3D(VkDevice device, VkPhysicalDevice physicalDevice, uint32_t width, uint32_t height, uint32_t depth,
                 uint32_t mipCount, VkFormat format, VkSampleCountFlagBits sampleCount,
                 const vector<uint32_t> &queueFamilies) : m_Image(device,physicalDevice, VK_IMAGE_TYPE_3D, VkExtent3D{width,height,depth}, mipCount,1, format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT, sampleCount, VK_IMAGE_LAYOUT_PREINITIALIZED, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, queueFamilies)
{

}
