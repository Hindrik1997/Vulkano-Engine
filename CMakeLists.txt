cmake_minimum_required(VERSION 3.6)
project(VulkanoEngine)

include_directories($ENV{VULKAN_SDK}/include)
link_directories($ENV{VULKAN_SDK}/lib)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES

        "Rendering/VulkanPlatforms/VulkanPlatform.h"
        "Rendering/VulkanCore.h"
        "Rendering/VulkanCore.cpp"
        "Rendering/Utilities/VulkanCoreStructs.h"
        "Rendering/Utilities/VulkanCoreUtilityFunctions.h"
        "Rendering/Utilities/VulkanCoreUtilityFunctions.cpp"

        "Data Storage/CacheOptimizedStorage.h"
        "Data Storage/Pool.h"
        "Data Storage/PoolItem.h"
        "Data Storage/StorageItem.h"

        "Core/Engine.h"
        "Core/Engine.cpp"
        "Core/Clock.h"
        "Core/Clock.cpp"
        "Core/Console.h"

        Rendering/Renderer.cpp Rendering/Renderer.h Rendering/VulkanPlatforms/VK_XCB_PLATFORM.cpp Rendering/VulkanPlatforms/VK_XCB_PLATFORM.h Rendering/Utilities/VulkanLayerAndExtensionManager.cpp Rendering/Utilities/VulkanLayerAndExtensionManager.h Rendering/Utilities/VulkanLayerAndExtensionManagerDebug.cpp Rendering/Utilities/VulkanLayerAndExtensionManagerDebug.h Rendering/VulkanCoreStaticFunctions.cpp)

link_libraries(vulkan xcb)

add_library(VulkanoEngine ${SOURCE_FILES})

link_libraries(VulkanoEngine)

add_executable(VulkanoLauncher main.cpp)