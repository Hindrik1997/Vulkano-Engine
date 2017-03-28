//
// Created by hindrik on 27-1-17.
//

#include <limits>
#include "VulkanUtilityFunctions.h"

ostream& operator<<(ostream& o, VkPhysicalDeviceType& t) {
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

ostream& operator<<(ostream& o, VkPhysicalDeviceProperties& p) {

    o << "---------------DEVICE---------------" << std::endl;

    o << "Name: "               << p.deviceName     << std::endl;
    o << "Type: "               << p.deviceType     << std::endl;
    o << "Vendor: "             << p.vendorID       << std::endl;
    o << "Device ID: "          << p.deviceID       << std::endl;
    o << "API Version: "        << p.apiVersion     << std::endl;
    o << "Driver Version: "     << p.driverVersion  << std::endl;

    o << "------------------------------------" << std::endl;

    return o;
}

ostream &operator<<(ostream &o, VkQueueFamilyProperties p) {

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

ostream& operator<<(ostream& o, vk_layer_extension_properties p)
{
    o << "------------------------------LAYER PROPERTIES------------------------------" << std::endl;
    o << "Layer name: "                     << p.m_layerProperties.layerName                << std::endl;
    o << "Layer description: "              << p.m_layerProperties.description              << std::endl;
    o << "Layer implementation version: "   << p.m_layerProperties.implementationVersion    << std::endl;
    o << "Layer spec version: "             << p.m_layerProperties.specVersion              << std::endl;
    o << "Extension count: "                << p.m_ExtensionProperties.size()               << std::endl;

    o << "------------LAYER EXTENSIONS: " << p.m_layerProperties.layerName << "------------" << std::endl;

    for(VkExtensionProperties ep : p.m_ExtensionProperties)
    {
        o << "Extension name: "             << ep.extensionName << std::endl;
        o << "Extension spec version: "     << ep.specVersion   << std::endl;
    }

    o << "------------------------------------------------------------------------------" << std::endl;

    return o;
}

ostream& operator<<(ostream &o, VkExtensionProperties p) {

    o << "------------------------------EXTENSION PROPERTIES------------------------------" << std::endl;
    o << "Extension name: "         << p.extensionName  << std::endl;
    o << "Extension spec version: " << p.specVersion    << std::endl;
    o << "------------------------------------------------------------------------------" << std::endl;

    return o;
}