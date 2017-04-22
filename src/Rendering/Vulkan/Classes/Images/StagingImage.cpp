//
// Created by hindrik on 22-4-17.
//

#include "StagingImage.h"

const Image &StagingImage::image() const
{
    return m_Image;
}

//TODO: Add support for image arrays, 3D textures and mipmaps later!
StagingImage::StagingImage(void* pixelData, VkDeviceSize imageSize, VkDevice device, VkPhysicalDevice physicalDevice, VkImageType imageType, uint32_t width,
                           uint32_t height, uint32_t depth, uint32_t mipCount, VkFormat format,
                           VkSampleCountFlagBits sampleCount, const vector<uint32_t> &queueFamilies)
                : m_Image(device,physicalDevice, imageType, VkExtent3D{width,height,depth}, mipCount,1, format, VK_IMAGE_TILING_LINEAR, VK_IMAGE_USAGE_TRANSFER_SRC_BIT, sampleCount, VK_IMAGE_LAYOUT_PREINITIALIZED, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, queueFamilies)
{
    void* data;
    vkMapMemory(m_Image.device(), m_Image.memory(), 0, imageSize, 0, &data);

    VkImageSubresource subresource = {};
    subresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresource.mipLevel = 0;
    subresource.arrayLayer = 0;

    VkSubresourceLayout layout;
    vkGetImageSubresourceLayout(device, m_Image.image(), &subresource, &layout);

    //We can copy it directly
    if(layout.rowPitch == width * 4)
    {
        memcpy(data,pixelData, static_cast<size_t >(imageSize));
    }
    else //We have to take into account padding bytes
    {
        uint8_t* bytes = reinterpret_cast<uint8_t*>(data);

        for(int i = 0; i < height; ++i)
        {
            memcpy(&bytes[i * layout.rowPitch], &static_cast<uint8_t *>(pixelData)[i * width * 4], width * 4);
        }

    }

    vkUnmapMemory(m_Image.device(), m_Image.memory());
}
