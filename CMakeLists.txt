cmake_minimum_required(VERSION 3.6)
project(VulkanoEngine)

if (WIN32)

	include_directories($ENV{VK_SDK_PATH}/include)
	link_directories($ENV{VK_SDK_PATH}/lib)

	include_directories("G:/Libraries/glfw-3.2.1/include")

		#link_directories("G:/Libraries/glfw-3.2.1/x64-debug-vs2017")
		link_directories("G:/Libraries/glfw-3.2.1/x64-release-vs2017")

	link_directories("C:/VulkanSDK/1.0.39.1/Bin")



else(WIN32)

include_directories($ENV{VULKAN_SDK}/include)
link_directories($ENV{VULKAN_SDK}/lib)

endif(WIN32)



set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES

        "Rendering/Vulkan/VulkanPlatforms/VulkanPlatform.h"
        "Rendering/VulkanCore.h"
        "Rendering/VulkanCore.cpp"
        "Rendering/Utilities/VulkanStructs.h"
        "Rendering/Utilities/VulkanUtilityFunctions.h"
        "Rendering/Utilities/VulkanUtilityFunctions.cpp"

        "Data Storage/CacheOptimizedStorage.h"
        "Data Storage/Pool.h"
        "Data Storage/PoolItem.h"
        "Data Storage/StorageItem.h"

        "Core/Engine.h"
        "Core/Engine.cpp"
        "Core/Clock.h"
        "Core/Clock.cpp"
        "Core/Console.h"

        Rendering/Renderer.cpp Rendering/Renderer.h Rendering/VulkanCore/VulkanCoreDebugFacilities.cpp Rendering/VulkanCore/VulkanCoreLayersAndExtensions.cpp Rendering/VulkanCore/VulkanCoreInitialization.cpp Rendering/Utilities/ShaderModule.cpp Rendering/Utilities/ShaderModule.h Rendering/Utilities/VulkanEnums.h Rendering/Utilities/Pipeline.cpp Rendering/Utilities/Pipeline.h Rendering/Vulkan/VulkanPlatforms/VK_GLFW_PLATFORM.cpp Rendering/Vulkan/VulkanPlatforms/VK_GLFW_PLATFORM.h "Utility Classes/NonCopyable.h" "Utility Classes/NonMovable.h" "Utility Classes/Macros.h"
        Rendering/Vulkan/VkCore.cpp Rendering/Vulkan/VkCore.h Rendering/Vulkan/Classes/RenderTargetOutput.cpp Rendering/Vulkan/Classes/RenderTargetOutput.h)

#Rendering/VulkanPlatforms/VK_XCB_PLATFORM.cpp Rendering/VulkanPlatforms/VK_XCB_PLATFORM.h

if(WIN32)
	link_libraries(vulkan-1 glfw3)
else(WIN32)
	link_libraries(vulkan xcb glfw)
endif(WIN32)

add_library(VulkanoEngine ${SOURCE_FILES})

link_libraries(VulkanoEngine)

add_executable(VulkanoLauncher main.cpp)

if(WIN32)
	set_target_properties(VulkanoLauncher PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/")
endif(WIN32)

add_custom_command(TARGET VulkanoLauncher PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Shaders $<TARGET_FILE_DIR:VulkanoLauncher>/Shaders)