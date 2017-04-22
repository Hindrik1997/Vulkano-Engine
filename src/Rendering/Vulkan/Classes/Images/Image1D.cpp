//
// Created by hindrik on 22-4-17.
//

#include "Image1D.h"

Image1D::Image1D(VkDevice device, VkPhysicalDevice physicalDevice, uint32_t width, uint32_t mipCount, VkFormat format,
                 VkSampleCountFlagBits sampleCount)
        : m_Image(device,physicalDevice, VK_IMAGE_TYPE_1D, VkExtent3D{width,1,1}, mipCount,1, format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, sampleCount, VK_IMAGE_LAYOUT_PREINITIALIZED, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)

{

}

Image1D::Image1D(VkDevice device, VkPhysicalDevice physicalDevice, uint32_t width, uint32_t mipCount, VkFormat format,
                 VkSampleCountFlagBits sampleCount, const vector<uint32_t>& queueFamilies)
        : m_Image(device,physicalDevice, VK_IMAGE_TYPE_1D, VkExtent3D{width,1,1}, mipCount,1, format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, sampleCount, VK_IMAGE_LAYOUT_PREINITIALIZED, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, queueFamilies)

{

}

const Image& Image1D::image() const
{
    return m_Image;
}
