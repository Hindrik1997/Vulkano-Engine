cmake_minimum_required(VERSION 3.6)
project(VulkanoEngine)

if (WIN32)

	include_directories($ENV{VK_SDK_PATH}/include)
	link_directories($ENV{VK_SDK_PATH}/lib)

    include_directories("libraries/glfw/include")

	if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")

        link_directories("libraries/glfw/lib-vc2015")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std:c++latest")

	elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")

        link_directories("libraries/glfw/lib-mingw-w64")

        set(CMAKE_SHARED_LINKER_FLAGS -static -static-libgcc -static-libstdc++)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static -static-libgcc -static-libstdc++")

	endif()

else(WIN32)

include_directories($ENV{VK_SDK_PATH}/include)
link_directories($ENV{VK_SDK_PATH}/lib)

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

        src/Rendering/Renderer.cpp src/Rendering/Renderer.h src/Rendering/Vulkan/Classes/ShaderModule.cpp src/Rendering/Vulkan/Classes/ShaderModule.h src/Rendering/Utilities/VulkanEnums.h src/Rendering/Vulkan/VulkanPlatforms/VK_GLFW_PLATFORM.cpp src/Rendering/Vulkan/VulkanPlatforms/VK_GLFW_PLATFORM.h "src/Utility Classes/NonCopyable.h" "src/Utility Classes/NonMovable.h" "src/Utility Classes/Macros.h"
        src/Rendering/Vulkan/Instance.cpp src/Rendering/Vulkan/Instance.h src/Rendering/Vulkan/Classes/Swapchain.cpp src/Rendering/Vulkan/Classes/Swapchain.h "src/Utility Classes/NonCopyableNonMovable.h" src/Rendering/Vulkan/VkUniqueHandle.h src/Rendering/Vulkan/Classes/PipelineStateObject.cpp src/Rendering/Vulkan/Classes/PipelineStateObject.h src/Rendering/Vulkan/Classes/Renderpass.cpp src/Rendering/Vulkan/Classes/Renderpass.h src/Rendering/Vulkan/Classes/PipelineStateDescriptor.cpp src/Rendering/Vulkan/Classes/PipelineStateDescriptor.h src/Rendering/Vulkan/RenderOutput.cpp src/Rendering/Vulkan/RenderOutput.h src/Rendering/RenderModes/ForwardRenderMode.cpp src/Rendering/RenderModes/ForwardRenderMode.h src/Rendering/Vulkan/VulkanPlatforms/WindowHandleWrapper.h src/Rendering/Vulkan/Classes/ViewportSet.h src/Rendering/Vulkan/Classes/Framebuffer.cpp src/Rendering/Vulkan/Classes/Framebuffer.h src/Rendering/Vulkan/Classes/CommandPool.cpp src/Rendering/Vulkan/Classes/CommandPool.h src/Rendering/Vulkan/VulkanPlatforms/FunctionWrapper.h src/Rendering/Vulkan/Classes/Buffers/VertexBuffer.cpp src/Rendering/Vulkan/Classes/Buffers/VertexBuffer.h src/Rendering/Vulkan/Classes/Vertex.h src/Rendering/Vulkan/Classes/Buffers/Buffer.cpp src/Rendering/Vulkan/Classes/Buffers/Buffer.h src/Rendering/Vulkan/Classes/Buffers/StagingBuffer.cpp src/Rendering/Vulkan/Classes/Buffers/StagingBuffer.h src/Core/ThreadWorker.h src/Core/Threadpool.h src/Core/ThreadWorker.cpp src/Core/Threadpool.cpp src/Rendering/Vulkan/Classes/Buffers/IndexedVertexBuffer.cpp src/Rendering/Vulkan/Classes/Buffers/IndexedVertexBuffer.h src/GameObjects/GameObject.cpp src/GameObjects/GameObject.h src/GameObjects/Transform.cpp src/GameObjects/Transform.h src/Rendering/UniformBuffers/WVP.h src/Rendering/Vulkan/Classes/Buffers/UniformBuffer.cpp src/Rendering/Vulkan/Classes/Buffers/UniformBuffer.h src/Rendering/Vulkan/Classes/Images/Image.cpp src/Rendering/Vulkan/Classes/Images/Image.h src/Rendering/Vulkan/Classes/Images/Image2D.cpp src/Rendering/Vulkan/Classes/Images/Image2D.h src/Rendering/Vulkan/Classes/Images/Image1D.cpp src/Rendering/Vulkan/Classes/Images/Image1D.h src/Rendering/Vulkan/Classes/Images/Image3D.cpp src/Rendering/Vulkan/Classes/Images/Image3D.h src/Rendering/Vulkan/Classes/Images/StagingImage.cpp src/Rendering/Vulkan/Classes/Images/StagingImage.h src/Core/Logger.cpp src/Core/Logger.h src/Rendering/Vulkan/MemoryManagement/MemoryManager.cpp src/Rendering/Vulkan/MemoryManagement/MemoryManager.h src/Core/AwaitableTask.cpp src/Core/AwaitableTask.h src/Core/ConfigurationLoader.cpp src/Core/ConfigurationLoader.h "src/Utility Classes/Nullable.h" "src/Utility Classes/ThreadedResourceSet.h" src/Rendering/Vulkan/PresentDevice.cpp src/Rendering/Vulkan/PresentDevice.h src/Rendering/Vulkan/Classes/ResourceOperationManager.cpp src/Rendering/Vulkan/Classes/ResourceOperationManager.h src/Rendering/Vulkan/Classes/FenceWaiter.cpp src/Rendering/Vulkan/Classes/FenceWaiter.h "src/Data Storage/MemBlockAllocator.h" src/Rendering/Vulkan/MemoryManagement/MemoryBlock.cpp src/Rendering/Vulkan/MemoryManagement/MemoryBlock.h src/Rendering/Vulkan/MemoryManagement/BuddyAllocator.cpp src/Rendering/Vulkan/MemoryManagement/BuddyAllocator.h src/Rendering/Vulkan/MemoryManagement/IMemBlockAllocator.h)

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
        ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:VulkanoLauncher>/shaders)
add_custom_command(TARGET VulkanoLauncher PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/textures $<TARGET_FILE_DIR:VulkanoLauncher>/textures)

add_custom_command(TARGET VulkanoLauncher PRE_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/configs $<TARGET_FILE_DIR:VulkanoLauncher>/)