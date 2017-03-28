//
// Created by hindrik on 27-1-17.
//

#include <iostream>
#include "VulkanCore.h"
#include "../Core/Console.h"
#include "Utilities/Pipeline.h"



VulkanCore::VulkanCore(string applicationName,
                       vector<const char*> enabledKHRInstanceExtensions,
                       vector<const char*> enabledInstanceValidationLayers,
                       vector<const char*> enabledDeviceExtensions,
                       bool enableDebugLayers)
        : m_ApplicationName(applicationName), m_EnabledInstanceKHRExtensionNames(enabledKHRInstanceExtensions), m_EnabledInstanceValidationLayerNames(enabledInstanceValidationLayers), m_IsDebugEnabled(enableDebugLayers), m_EnabledDeviceExtensions(enabledDeviceExtensions)
{
    loadLayersAndExtensions();
    vkInit();
    Console::printLine("Initialized Vulkan API succesfully.");
}

VulkanCore::~VulkanCore() {

    vkDeviceWaitIdle(m_Device);

    cleanUpDebugFacilities();
    cleanUpShaderModules();
    cleanUpSwapchainImageViews();
    cleanUpSwapchainFrameBuffers();

    if(m_ImageAvailableSemaphore != VK_NULL_HANDLE)
        vkDestroySemaphore(m_Device, m_ImageAvailableSemaphore, nullptr);

    if(m_RenderingFinsihedSemaphore != VK_NULL_HANDLE)
        vkDestroySemaphore(m_Device, m_RenderingFinsihedSemaphore, nullptr);

    if(m_CommandPool != VK_NULL_HANDLE)
        vkDestroyCommandPool(m_Device, m_CommandPool, nullptr);

    if(m_Pipeline != VK_NULL_HANDLE)
        vkDestroyPipeline(m_Device, m_Pipeline, nullptr);

    if(m_PipelineLayout != VK_NULL_HANDLE)
        vkDestroyPipelineLayout(m_Device, m_PipelineLayout, nullptr);

    if(m_RenderPass != VK_NULL_HANDLE)
        vkDestroyRenderPass(m_Device, m_RenderPass, nullptr);

    if(m_Swapchain != VK_NULL_HANDLE)
        vkDestroySwapchainKHR(m_Device, m_Swapchain, nullptr);

    if(m_Surface != VK_NULL_HANDLE)
        vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);

    if(m_Device != VK_NULL_HANDLE)
        vkDestroyDevice(m_Device, nullptr);

    if(m_Instance != VK_NULL_HANDLE)
        vkDestroyInstance(m_Instance,nullptr);
}

bool VulkanCore::processPlatformAPI(float deltaTime) {
    return m_Platform.processAPI(deltaTime);
}

void VulkanCore::cleanUpSwapchainFrameBuffers()
{
    for(uint32_t i = static_cast<uint32_t >(m_SwapchainFrameBuffers.size()); i > 0; --i )
    {
        VkFramebuffer buffer = m_SwapchainFrameBuffers.back();
        m_SwapchainFrameBuffers.pop_back();

        vkDestroyFramebuffer(m_Device, buffer, nullptr);
    }
    m_SwapchainFrameBuffers.clear();
}

void VulkanCore::cleanUpShaderModules() {
    m_ShaderModules.clear();
}

void VulkanCore::cleanUpSwapchainImageViews() {

    for(uint32_t i = static_cast<uint32_t >(m_SwapchainImageViews.size()); i > 0; --i )
    {
        VkImageView view = m_SwapchainImageViews.back();
        m_SwapchainImageViews.pop_back();

        vkDestroyImageView(m_Device, view, nullptr);
    }
    m_SwapchainImageViews.clear();
}