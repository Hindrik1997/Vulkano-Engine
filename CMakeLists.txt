cmake_minimum_required(VERSION 3.6)
project(VulkanoEngine)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES

        "Rendering/VulkanPlatform.h"
        "Rendering/VulkanCore.h"
        "Rendering/VulkanCore.cpp"
        "Rendering/VulkanCoreStructs.h"
        "Rendering/VulkanCoreUtilityFunctions.h"
        "Rendering/VulkanCoreUtilityFunctions.cpp"

        "Data Storage/CacheOptimizedStorage.h"
        "Data Storage/Pool.h"
        "Data Storage/PoolItem.h"
        "Data Storage/StorageItem.h"

        "Core/Engine.h"
        "Core/Engine.cpp"
        "Core/Clock.h"
        "Core/Clock.cpp"
        "Core/Console.h"

        Rendering/Renderer.cpp Rendering/Renderer.h Rendering/VulkanPlatforms/VK_XCB_PLATFORM.cpp Rendering/VulkanPlatforms/VK_XCB_PLATFORM.h)

link_libraries(vulkan xcb)

add_library(VulkanoEngine ${SOURCE_FILES})

link_libraries(VulkanoEngine)

add_executable(VulkanoLauncher main.cpp)