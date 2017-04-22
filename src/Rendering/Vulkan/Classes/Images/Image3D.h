//
// Created by hindrik on 22-4-17.
//

#ifndef VULKANOENGINE_IMAGE3D_H
#define VULKANOENGINE_IMAGE3D_H


#include "Image.h"

class Image3D
{
private:
    Image m_Image;
public:
    Image3D(VkDevice device, VkPhysicalDevice physicalDevice, uint32_t width, uint32_t height, uint32_t depth, uint32_t mipCount, VkFormat format, VkSampleCountFlagBits sampleCount);
    Image3D(VkDevice device, VkPhysicalDevice physicalDevice, uint32_t width, uint32_t height, uint32_t depth, uint32_t mipCount, VkFormat format, VkSampleCountFlagBits sampleCount, const vector<uint32_t>& queueFamilies);
public:
    Image& image() const;
};


#endif //VULKANOENGINE_IMAGE3D_H
