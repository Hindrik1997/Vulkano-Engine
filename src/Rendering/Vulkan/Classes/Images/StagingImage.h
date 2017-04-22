//
// Created by hindrik on 22-4-17.
//

#ifndef VULKANOENGINE_STAGINGIMAGE_H
#define VULKANOENGINE_STAGINGIMAGE_H


#include "Image.h"

class StagingImage
{
private:
    Image m_Image;
public:
    StagingImage(void* pixelData, VkDeviceSize imageSize, VkDevice device, VkPhysicalDevice physicalDevice, VkImageType imageType, uint32_t width, uint32_t height, uint32_t depth, uint32_t mipCount, VkFormat format, VkSampleCountFlagBits sampleCount, const vector<uint32_t>& queueFamilies);

public:
    const Image& image() const;

};


#endif //VULKANOENGINE_STAGINGIMAGE_H
