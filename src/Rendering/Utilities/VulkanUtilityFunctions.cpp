//
// Created by hindrik on 27-1-17.
//

#include <limits>
#include <tuple>
#include "VulkanUtilityFunctions.h"
#include "../../Core/Logger.h"

using std::tuple;
using std::make_tuple;

auto operator<<(ostream& o, VkPhysicalDeviceType& t) -> ostream&
{
    switch(t)
    {
        case VK_PHYSICAL_DEVICE_TYPE_OTHER:
            o << "Other";
            break;
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
            o << "Integrated GPU";
            break;
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
            o << "Discrete GPU";
            break;
        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
            o << "Virtual GPU";
            break;
        case VK_PHYSICAL_DEVICE_TYPE_CPU:
            o << "CPU";
            break;
        default:
            o << t;
            break;
    }
    return o;
}

auto operator<<(ostream& o, VkPhysicalDeviceProperties& p) -> ostream&
{

    string vendorName = getVendorNameForID(p.vendorID);

    o << "---------------DEVICE---------------" << std::endl;

    o << "Name: "               << p.deviceName     << std::endl;
    o << "Type: "               << p.deviceType     << std::endl;
    o << "Vendor: "             << vendorName       << std::endl;
    o << "PresentDevice ID: "          << p.deviceID       << std::endl;
    o << "API Version: "        << p.apiVersion     << std::endl;
    o << "Driver Version: "     << p.driverVersion  << std::endl;

    o << "------------------------------------"     << std::endl;

    return o;
}

auto operator<<(ostream &o, VkQueueFamilyProperties p) -> ostream&
{

    o << "------------QUEUE_FAMILY------------" << std::endl;

    o << "Queue count: " << p.queueCount << std::endl;

    o << "------------------------------------" << std::endl;
    o << "Graphics: "               << ((p.queueFlags & VK_QUEUE_GRAPHICS_BIT) ? "Yes" : "No")          << std::endl;
    o << "Compute: "                << ((p.queueFlags & VK_QUEUE_COMPUTE_BIT)  ? "Yes" : "No")          << std::endl;
    o << "Transfer: "               << ((p.queueFlags & VK_QUEUE_TRANSFER_BIT) ? "Yes" : "No")          << std::endl;
    o << "Sparse binding: "         << ((p.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) ? "Yes" : "No")    << std::endl;

    o << "------------------------------------" << std::endl;

    return o;
}

auto operator<<(ostream& o, vk_layer_extension_properties p) -> ostream&
{
    o << "------------------------------LAYER PROPERTIES------------------------------" << std::endl;
    o << "Layer name: "                     << p.m_layerProperties.layerName                << std::endl;
    o << "Layer description: "              << p.m_layerProperties.description              << std::endl;
    o << "Layer implementation version: "   << p.m_layerProperties.implementationVersion    << std::endl;
    o << "Layer spec version: "             << p.m_layerProperties.specVersion              << std::endl;
    o << "Extension count: "                << p.m_ExtensionProperties.size()               << std::endl;

    if(p.m_ExtensionProperties.size() != 0)
    {
        o << "------------LAYER EXTENSIONS: " << p.m_layerProperties.layerName << "------------" << std::endl;

        for(VkExtensionProperties ep : p.m_ExtensionProperties)
        {
            o << "Extension name: "             << ep.extensionName << std::endl;
            o << "Extension spec version: "     << ep.specVersion   << std::endl;
        }
    }

    o << "------------------------------------------------------------------------------" << std::endl;

    return o;
}

auto operator<<(ostream &o, VkExtensionProperties p) -> ostream&
{

    o << "------------------------------EXTENSION PROPERTIES------------------------------" << std::endl;
    o << "Extension name: "         << p.extensionName  << std::endl;
    o << "Extension spec version: " << p.specVersion    << std::endl;
    o << "------------------------------------------------------------------------------" << std::endl;

    return o;
}

auto operator<<(ostream &o, vk_memory_heap p) -> ostream &
{
    o << "------------------------------MEMORYHEAP PROPERTIES------------------------------" << std::endl;

    o << "Heap location: " << (p.m_IsDeviceLocal ? "DEVICE" : "HOST") << std::endl;
    o << "Heap size: " << p.m_MemoryHeap.size << " bytes" << std::endl;

    o << "---------------------------------------------------------------------------------" << std::endl;

    return o;
}



auto getVendorNameForID(uint32_t id) -> string
{
    vector<tuple<uint32_t,string>> names =
            {
                    make_tuple(0x1002, "AMD"),
                    make_tuple(0x1010, "ImgTec"),
                    make_tuple(0x10DE, "NVIDIA"),
                    make_tuple(0x13B5, "ARM"),
                    make_tuple(0x5143, "Qualcomm"),
                    make_tuple(0x8086, "Intel")

            };
    for(auto& t : names)
    {
        if(std::get<0>(t) == id)
        {
            return std::get<1>(t);
        }
    }
    Logger::error("No compatible vendor found for this id: " + std::to_string(id) +  "!");
    return std::to_string(id);
}

auto vkIfFailThrowMessage(VkResult result, std::string message) -> void
{
    if(result != VK_SUCCESS)
    {
        Logger::failure(message);
    }
}

auto getSuitableMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties) -> uint32_t
{
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }
    Logger::failure("failed to find suitable memory type!");
    throw;
}


