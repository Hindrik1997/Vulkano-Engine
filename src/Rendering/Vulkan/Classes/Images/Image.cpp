//
// Created by hindrik on 22-4-17.
//

#include "Image.h"
#include "../../../Utilities/VulkanUtilityFunctions.h"

Image::Image(VkDevice device, VkPhysicalDevice physicalDevice, VkImageType imageType, VkExtent3D imageSize, uint32_t mipCount, uint32_t arrayLayers, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,  VkSampleCountFlagBits sampleCount, VkImageLayout initialLayout, VkMemoryPropertyFlags memoryPropertyFlags, const vector<uint32_t>& queueFamilies)
    : m_Type(imageType),
      m_Extent(imageSize),
      m_MipLevels(mipCount),
      m_ArrayLayers(arrayLayers),
      m_Format(format),
      m_Tiling(tiling),
      m_UsageFlags(usage),
      m_SharingMode(VK_SHARING_MODE_CONCURRENT),
      m_SampleCount(sampleCount),
      m_Image({device, vkDestroyImage}),
      m_Memory({device, vkFreeMemory}),
      m_Device(device),
      m_PhysicalDevice(physicalDevice),
      m_MemoryPropertyFlags(memoryPropertyFlags)
{
    VkImageCreateInfo imageCreateInfo = {};

    imageCreateInfo.imageType = m_Type;
    imageCreateInfo.extent = m_Extent;
    imageCreateInfo.mipLevels = m_MipLevels;
    imageCreateInfo.arrayLayers = m_ArrayLayers;
    imageCreateInfo.format = m_Format;
    imageCreateInfo.tiling = m_Tiling;
    imageCreateInfo.initialLayout = initialLayout;
    imageCreateInfo.usage = m_UsageFlags;
    imageCreateInfo.sharingMode = m_SharingMode;
    imageCreateInfo.pQueueFamilyIndices = queueFamilies.data();
    imageCreateInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamilies.size());
    imageCreateInfo.samples = m_SampleCount;
    imageCreateInfo.flags = 0;

    VkResult result = vkCreateImage(m_Device, &imageCreateInfo, nullptr, m_Image.reset());
    vkIfFailThrowMessage(result, "Image creation failed!");

    VkMemoryRequirements memoryRequirements = {};
    vkGetImageMemoryRequirements(m_Device, m_Image, &memoryRequirements);
    m_MemoryRequirements = memoryRequirements;

    VkMemoryAllocateInfo allocateInfo = {};
    allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocateInfo.allocationSize = memoryRequirements.size;
    allocateInfo.memoryTypeIndex = getSuitableMemoryType(m_PhysicalDevice, memoryRequirements.memoryTypeBits, m_MemoryPropertyFlags);

    result = vkAllocateMemory(m_Device, &allocateInfo, nullptr, m_Memory.reset());
    vkIfFailThrowMessage(result, "Memory allocation failed!");

    vkBindImageMemory(m_Device, m_Image, m_Memory, 0);
}

Image::Image(VkDevice device, VkPhysicalDevice physicalDevice, VkImageType imageType, VkExtent3D imageSize, uint32_t mipCount, uint32_t arrayLayers, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,  VkSampleCountFlagBits sampleCount, VkImageLayout initialLayout, VkMemoryPropertyFlags memoryPropertyFlags)
        : m_Type(imageType),
          m_Extent(imageSize),
          m_MipLevels(mipCount),
          m_ArrayLayers(arrayLayers),
          m_Format(format),
          m_Tiling(tiling),
          m_UsageFlags(usage),
          m_SharingMode(VK_SHARING_MODE_EXCLUSIVE),
          m_SampleCount(sampleCount),
          m_Image({device, vkDestroyImage}),
          m_Memory({device, vkFreeMemory}),
          m_Device(device),
          m_PhysicalDevice(physicalDevice),
          m_MemoryPropertyFlags(memoryPropertyFlags)
{
    VkImageCreateInfo imageCreateInfo = {};

    imageCreateInfo.imageType = m_Type;
    imageCreateInfo.extent = m_Extent;
    imageCreateInfo.mipLevels = m_MipLevels;
    imageCreateInfo.arrayLayers = m_ArrayLayers;
    imageCreateInfo.format = m_Format;
    imageCreateInfo.tiling = m_Tiling;
    imageCreateInfo.initialLayout = initialLayout;
    imageCreateInfo.usage = m_UsageFlags;
    imageCreateInfo.sharingMode = m_SharingMode;
    imageCreateInfo.pQueueFamilyIndices = nullptr;
    imageCreateInfo.queueFamilyIndexCount = 0;
    imageCreateInfo.samples = m_SampleCount;
    imageCreateInfo.flags = 0;

    VkResult result = vkCreateImage(m_Device, &imageCreateInfo, nullptr, m_Image.reset());
    vkIfFailThrowMessage(result, "Image creation failed!");

    VkMemoryRequirements memoryRequirements = {};
    vkGetImageMemoryRequirements(m_Device, m_Image, &memoryRequirements);
    m_MemoryRequirements = memoryRequirements;

    VkMemoryAllocateInfo allocateInfo = {};
    allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocateInfo.allocationSize = memoryRequirements.size;
    allocateInfo.memoryTypeIndex = getSuitableMemoryType(m_PhysicalDevice, memoryRequirements.memoryTypeBits, m_MemoryPropertyFlags);

    result = vkAllocateMemory(m_Device, &allocateInfo, nullptr, m_Memory.reset());
    vkIfFailThrowMessage(result, "Memory allocation failed!");

    vkBindImageMemory(m_Device, m_Image, m_Memory, 0);
}

VkMemoryRequirements Image::memoryRequirements() const
{
    return m_MemoryRequirements;
}

VkMemoryPropertyFlags Image::memoryPropertyFlags() const
{
    return m_MemoryPropertyFlags;
}

VkPhysicalDevice Image::physicalDevice() const
{
    return m_PhysicalDevice;
}

VkDevice Image::device() const
{
    return m_Device;
}

VkSampleCountFlagBits Image::sampleCountFlagBits() const
{
    return m_SampleCount;
}

VkSharingMode Image::sharingMode() const
{
    return m_SharingMode;
}

VkImageUsageFlags Image::usageFlags() const
{
    return m_UsageFlags;
}

VkImageTiling Image::tiling() const
{
    return m_Tiling;
}

VkFormat Image::format() const
{
    return m_Format;
}

uint32_t Image::arrayLayers() const
{
    return m_ArrayLayers;
}

uint32_t Image::mipCount() const
{
    return m_MipLevels;
}

VkImageType Image::imageType() const
{
    return m_Type;
}

VkExtent3D Image::imageDimensions() const
{
    return m_Extent;
}

VkDeviceMemory Image::memory() const
{
    return m_Memory;
}

VkImage Image::image() const
{
    return m_Image;
}
