//
// Created by hindrik on 22-4-17.
//

#ifndef VULKANOENGINE_IMAGE1D_H
#define VULKANOENGINE_IMAGE1D_H


#include "Image.h"

class Image1D
{
private:
    Image m_Image;
public:
    Image1D(VkDevice device, VkPhysicalDevice physicalDevice, uint32_t width, uint32_t mipCount, VkFormat format, VkSampleCountFlagBits sampleCount);
    Image1D(VkDevice device, VkPhysicalDevice physicalDevice, uint32_t width, uint32_t mipCount, VkFormat format, VkSampleCountFlagBits sampleCount, const vector<uint32_t>& queueFamilies);
public:
    const Image& image() const;
};


#endif //VULKANOENGINE_IMAGE1D_H
