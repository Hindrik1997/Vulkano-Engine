cmake_minimum_required(VERSION 3.6)
project(VulkanoEngine)

if (WIN32)

	include_directories($ENV{VK_SDK_PATH}/include)
	link_directories($ENV{VK_SDK_PATH}/lib)

	include_directories("G:/Libraries/glfw-3.2.1/include")

		#link_directories("G:/libraries/glfw-3.2.1/x64-debug-vs2017")
		link_directories("G:/Libraries/glfw-3.2.1/x64-release-vs2017")

	link_directories("C:/VulkanSDK/1.0.39.1/Bin")



else(WIN32)

include_directories($ENV{VULKAN_SDK}/include)
link_directories($ENV{VULKAN_SDK}/lib)

endif(WIN32)

include_directories("libraries/")

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES

        "src/Rendering/Vulkan/VulkanPlatforms/VulkanPlatform.h"
        "src/Rendering/Utilities/VulkanStructs.h"
        "src/Rendering/Utilities/VulkanUtilityFunctions.h"
        "src/Rendering/Utilities/VulkanUtilityFunctions.cpp"

        "src/Data Storage/CacheOptimizedStorage.h"
        "src/Data Storage/Pool.h"
        "src/Data Storage/PoolItem.h"
        "src/Data Storage/StorageItem.h"

        "src/Core/Engine.h"
        "src/Core/Engine.cpp"
        "src/Core/Clock.h"
        "src/Core/Clock.cpp"
        "src/Core/Console.h"

		src/Rendering/Renderer.cpp src/Rendering/Renderer.h src/Rendering/Vulkan/Classes/ShaderModule.cpp src/Rendering/Vulkan/Classes/ShaderModule.h src/Rendering/Utilities/VulkanEnums.h src/Rendering/Vulkan/VulkanPlatforms/VK_GLFW_PLATFORM.cpp src/Rendering/Vulkan/VulkanPlatforms/VK_GLFW_PLATFORM.h "src/Utility Classes/NonCopyable.h" "src/Utility Classes/NonMovable.h" "src/Utility Classes/Macros.h"
		src/Rendering/Vulkan/VkCore.cpp src/Rendering/Vulkan/VkCore.h src/Rendering/Vulkan/Classes/RenderTarget.cpp src/Rendering/Vulkan/Classes/RenderTarget.h src/Rendering/Vulkan/Classes/Swapchain.cpp src/Rendering/Vulkan/Classes/Swapchain.h "src/Utility Classes/NonCopyableNonMovable.h" src/Rendering/Vulkan/VkUniqueHandle.h src/Rendering/Vulkan/Classes/PipelineStateObject.cpp src/Rendering/Vulkan/Classes/PipelineStateObject.h src/Rendering/Vulkan/Classes/Renderpass.cpp src/Rendering/Vulkan/Classes/Renderpass.h src/Rendering/Vulkan/Classes/PipelineStateDescriptor.cpp src/Rendering/Vulkan/Classes/PipelineStateDescriptor.h src/Rendering/Vulkan/Classes/RenderMode.h src/Rendering/Vulkan/Classes/RenderTargetOutput.cpp src/Rendering/Vulkan/Classes/RenderTargetOutput.h src/Rendering/RenderModes/ForwardRenderMode.cpp src/Rendering/RenderModes/ForwardRenderMode.h src/Rendering/Vulkan/VulkanPlatforms/WindowHandleWrapper.h src/Rendering/Vulkan/Classes/ViewportSet.h src/Rendering/Vulkan/Classes/Framebuffer.cpp src/Rendering/Vulkan/Classes/Framebuffer.h src/Rendering/Vulkan/Classes/CommandPool.cpp src/Rendering/Vulkan/Classes/CommandPool.h src/Rendering/Vulkan/VulkanPlatforms/FunctionWrapper.h src/Rendering/Vulkan/Classes/Buffers/VertexBuffer.cpp src/Rendering/Vulkan/Classes/Buffers/VertexBuffer.h src/Rendering/Vulkan/Classes/Vertex.h src/Rendering/Vulkan/Classes/Buffers/Buffer.cpp src/Rendering/Vulkan/Classes/Buffers/Buffer.h src/Rendering/Vulkan/Classes/Buffers/StagingBuffer.cpp src/Rendering/Vulkan/Classes/Buffers/StagingBuffer.h src/Core/ThreadWorker.h src/Core/Threadpool.h src/Core/ThreadWorker.cpp src/Core/Threadpool.cpp src/Rendering/Vulkan/Classes/Buffers/IndexedVertexBuffer.cpp src/Rendering/Vulkan/Classes/Buffers/IndexedVertexBuffer.h src/GameObjects/GameObject.cpp src/GameObjects/GameObject.h src/GameObjects/Transform.cpp src/GameObjects/Transform.h src/Rendering/UniformBuffers/WVP.h src/Rendering/Vulkan/Classes/Buffers/UniformBuffer.cpp src/Rendering/Vulkan/Classes/Buffers/UniformBuffer.h src/Rendering/Vulkan/Classes/Images/Image.cpp src/Rendering/Vulkan/Classes/Images/Image.h src/Rendering/Vulkan/Classes/Images/Image2D.cpp src/Rendering/Vulkan/Classes/Images/Image2D.h src/Rendering/Vulkan/Classes/Images/Image1D.cpp src/Rendering/Vulkan/Classes/Images/Image1D.h src/Rendering/Vulkan/Classes/Images/Image3D.cpp src/Rendering/Vulkan/Classes/Images/Image3D.h)

#Rendering/VulkanPlatforms/VK_XCB_PLATFORM.cpp Rendering/VulkanPlatforms/VK_XCB_PLATFORM.h

if(WIN32)
	link_libraries(vulkan-1 glfw3)
else(WIN32)
	link_libraries(vulkan xcb glfw pthread)
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