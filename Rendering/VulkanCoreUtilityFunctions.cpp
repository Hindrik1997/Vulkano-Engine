//
// Created by hindrik on 27-1-17.
//

#include "VulkanCoreUtilityFunctions.h"

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

    o << "Graphics: "               << ((p.queueFlags & VK_QUEUE_GRAPHICS_BIT) ? "Yes" : "No")          << std::endl;
    o << "Compute: "               << ((p.queueFlags & VK_QUEUE_COMPUTE_BIT)  ? "Yes" : "No")          << std::endl;
    o << "Transfer: "               << ((p.queueFlags & VK_QUEUE_TRANSFER_BIT) ? "Yes" : "No")          << std::endl;
    o << "Sparse binding: "         << ((p.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) ? "Yes" : "No")    << std::endl;

    o << "------------------------------------" << std::endl;

    return o;
}
