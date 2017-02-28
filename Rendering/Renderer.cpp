//
// Created by hindrik on 30-1-17.
//

#include <limits>
#include "Renderer.h"

#ifdef NDEBUG
    const bool enableDebugLayers = false;
#else
    const bool enableDebugLayers = true;
#endif


void Renderer::render(float deltaTime)
{

    uint32_t imageIndex;
    vkAcquireNextImageKHR(m_VulkanCore.m_Device, m_VulkanCore.m_Swapchain, std::numeric_limits<uint64_t >::max(), m_VulkanCore.m_ImageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

    VkSubmitInfo submitInfo                     = {};
    submitInfo.sType                            = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[]                = { m_VulkanCore.m_ImageAvailableSemaphore };
    VkPipelineStageFlags waitStages[]           = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount               = 1;
    submitInfo.pWaitSemaphores                  = waitSemaphores;
    submitInfo.pWaitDstStageMask                = waitStages;

    submitInfo.commandBufferCount               = 1;
    submitInfo.pCommandBuffers                  = &m_VulkanCore.m_CommandBuffers[imageIndex];

    VkSemaphore signalSemaphores[]              = { m_VulkanCore.m_RenderingFinsihedSemaphore};
    submitInfo.signalSemaphoreCount             = 1;
    submitInfo.pSignalSemaphores                = signalSemaphores;

    if (vkQueueSubmit(m_VulkanCore.m_GraphicsQueue.m_Queue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
        throw std::runtime_error("Failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo                = {};
    presentInfo.sType                           = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount              = 1;
    presentInfo.pWaitSemaphores                 = signalSemaphores;


    VkSwapchainKHR swapChains[]                 = { m_VulkanCore.m_Swapchain };
    presentInfo.swapchainCount                  = 1;
    presentInfo.pSwapchains                     = swapChains;
    presentInfo.pImageIndices                   = &imageIndex;

    presentInfo.pResults                        = nullptr;

    vkQueuePresentKHR(m_VulkanCore.m_GraphicsQueue.m_Queue, &presentInfo);
}

bool Renderer::processAPI(float deltaTime)
{
    return m_VulkanCore.processPlatformAPI(deltaTime);
}

Renderer::Renderer() : m_VulkanCore("Vulkano Engine", enabledKHRInstanceExtensions, enabledInstanceValidationLayers, enabledDeviceExtensions, enableDebugLayers)
{

}

Renderer::~Renderer() {




}