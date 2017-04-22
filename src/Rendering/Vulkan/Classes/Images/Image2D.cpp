//
// Created by hindrik on 22-4-17.
//

#include "Image2D.h"
#include "StagingImage.h"

Image2D::Image2D(VkDevice device, VkPhysicalDevice physicalDevice, uint32_t width, uint32_t height, uint32_t mipCount, VkFormat format, VkSampleCountFlagBits sampleCount) :
        m_Image(device,physicalDevice, VK_IMAGE_TYPE_2D, VkExtent3D{width,height,1}, mipCount,1, format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, sampleCount, VK_IMAGE_LAYOUT_PREINITIALIZED, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
{

}

Image2D::Image2D(void* pixelData, VkDeviceSize imageSize, VkDevice device, VkPhysicalDevice physicalDevice, uint32_t width, uint32_t height, uint32_t mipCount, VkFormat format, VkSampleCountFlagBits sampleCount, const vector<uint32_t>& queueFamilies) :
        m_Image(device,physicalDevice, VK_IMAGE_TYPE_2D, VkExtent3D{width,height,1}, mipCount,1, format, VK_IMAGE_TILING_LINEAR, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, sampleCount, VK_IMAGE_LAYOUT_PREINITIALIZED, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, queueFamilies)
{
    StagingImage stagingImage(pixelData, imageSize,device,physicalDevice, VK_IMAGE_TYPE_2D, width, height, 1, mipCount, format,sampleCount, queueFamilies);




}

const Image& Image2D::image() const
{
    return m_Image;
}
