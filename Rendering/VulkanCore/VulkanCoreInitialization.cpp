//
// Created by hindrik on 13-2-17.
//

#include <iostream>
#include "../VulkanCore.h"
#include "../../Core/Console.h"
#include "../Utilities/Pipeline.h"
#include "../Utilities/VulkanUtilityFunctions.cpp"
#include <algorithm>

void VulkanCore::vkInit() {

    vkInitInstance();

    //Enable debug if necessary
    if(m_IsDebugEnabled)
    {
        setupDebugFacilities();
    }

    vkInitCreateSurface();
    vkInitPhysicalDevice();
    vkInitLogicalDevice();
    vkInitAssignQqueues();
    vkInitCreateSwapchain();
    vkInitCreateSwapchainImageViews();
    vkInitCreateRenderPass();
    vkInitCreatePipeline();
    vkInitCreateFrameBuffers();
    vkInitCreateCommandPool();
    vkInitCreateCommandBuffers();
    vkInitCreateSwapchainSemaphores();
}

void VulkanCore::vkInitInstance() {

    VkResult result;

    VkApplicationInfo applicationInfo                   = {};

    applicationInfo.sType                               = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pNext                               = nullptr;
    applicationInfo.pApplicationName                    = m_ApplicationName.c_str();
    applicationInfo.applicationVersion                  = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.pEngineName                         = "Vulkano Engine";
    applicationInfo.engineVersion                       = VK_MAKE_VERSION(0, 0, 1);
    applicationInfo.apiVersion                          = VK_MAKE_VERSION(1, 0, 0);

    VkInstanceCreateInfo instanceCreateInfo             = {};
    instanceCreateInfo.sType                            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pNext                            = nullptr;
    instanceCreateInfo.flags                            = 0;
    instanceCreateInfo.pApplicationInfo                 = &applicationInfo;
    instanceCreateInfo.enabledExtensionCount            = static_cast<uint32_t >(m_EnabledInstanceKHRExtensionNames.size());
    instanceCreateInfo.ppEnabledExtensionNames          = m_EnabledInstanceKHRExtensionNames.data();
    instanceCreateInfo.enabledLayerCount                = static_cast<uint32_t >(m_EnabledInstanceValidationLayerNames.size());
    instanceCreateInfo.ppEnabledLayerNames              = m_EnabledInstanceValidationLayerNames.data();

    result = vkCreateInstance(&instanceCreateInfo, nullptr, &m_Instance);

    vk_if_fail_throw_message(result, "Error creating VkInstance.");
}

void VulkanCore::vkInitPhysicalDevice() {

    VkResult                    result;

    uint32_t                    physicalDeviceCount     = 0;
    vector<VkPhysicalDevice>    physicalDevices         = {};
    VkPhysicalDevice            selectedDevice          = VK_NULL_HANDLE;


    result = vkEnumeratePhysicalDevices(m_Instance, &physicalDeviceCount, nullptr);
    vk_if_fail_throw_message(result, "Error when retrieving amount of physical devices.");

    if(physicalDeviceCount == 0) {
        throw std::runtime_error("Error, no physical devices supporting Vulkan found!.");
    }

    physicalDevices.resize(physicalDeviceCount);

    result = vkEnumeratePhysicalDevices(m_Instance, &physicalDeviceCount, physicalDevices.data());
    vk_if_fail_throw_message(result, "Error when retrieving physical devices.");

    vector<VkPhysicalDevice> possibleGPUs;

    cout << "Possible GPU's found: " << std::endl;

    for(const auto& device : physicalDevices)
    {
        if(vkInitCheckDevice(device, m_EnabledDeviceExtensions, m_Surface))
        {
            possibleGPUs.push_back(device);
        }
    }

    if(possibleGPUs.size() == 0)
    {
        throw::std::runtime_error("Failed to find a suitable GPU. The GPU needs to be a discrete or integrated GPU, and needs to support geometry shaders and tesselation.");
    }

    vector<vk_device_rating> ratings;



    for(const auto& device : possibleGPUs)
    {
        ratings.push_back(vk_device_rating{device, vkInitSuitabilityRating(device)});
    }

    std::sort(ratings.begin(), ratings.end(), [](const vk_device_rating& left, const vk_device_rating& right){ return right.m_Rating < left.m_Rating; });

    selectedDevice = ratings.front().m_PhysicalDevice;

    if(selectedDevice == VK_NULL_HANDLE)
    {
        throw::std::runtime_error("Selected device is a VK_NULL_HANDLE! ");
    }

    m_PhysicalDevice = selectedDevice;


    VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties(selectedDevice, &props);


    std::cout << "Selected device: " << std::endl << props << std::endl;

    m_PhysicalDeviceInfo = vkInitGetQueueFamilies(selectedDevice);

    vkEnumerateDeviceExtensions(m_PhysicalDevice);
}

void VulkanCore::vkInitLogicalDevice() {

    VkResult result;

    m_QueueFamilies.clear();

    vkInitSetupQueueFamilies(m_PhysicalDeviceInfo.m_QueueFamilyProperties);

    vector<VkDeviceQueueCreateInfo> queueCreateInfos;

    if(m_QueueFamilies.size() > 0)
    {
        for(uint32_t i = 0; i < m_QueueFamilies.size(); ++i)
        {

            VkDeviceQueueCreateInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            info.pNext = nullptr;
            info.queueCount = static_cast<uint32_t >(m_QueueFamilies[i].m_QueueFamilyProperties.queueCount);
            info.queueFamilyIndex = m_QueueFamilies[i].m_Index;
            info.pQueuePriorities = m_QueueFamilies[i].m_Priorities.data();

            queueCreateInfos.push_back(info);
        }
    }

    VkPhysicalDeviceFeatures deviceFeatures = {};

    deviceFeatures.geometryShader = VK_TRUE;
    deviceFeatures.tessellationShader = VK_TRUE;

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = static_cast<uint32_t >(queueCreateInfos.size());
    createInfo.pEnabledFeatures = &deviceFeatures;

    //Device layers are deprecated, so we disable them
    createInfo.enabledLayerCount = 0;
    createInfo.ppEnabledLayerNames = nullptr;

    createInfo.enabledExtensionCount = static_cast<uint32_t >(m_EnabledDeviceExtensions.size());
    createInfo.ppEnabledExtensionNames = m_EnabledDeviceExtensions.data();


    result = vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_Device);
    vk_if_fail_throw_message(result, "Error creating device.");
}

void VulkanCore::vkInitSetupQueueFamilies(const vector<VkQueueFamilyProperties> &queueFamilies) {

    for(uint32_t i = 0;  i < static_cast<uint32_t >(queueFamilies.size()); ++i)
    {
        const VkQueueFamilyProperties& queueFamily = queueFamilies[i];

        vk_queue_family qf { queueFamily, i };

        for(uint32_t j = 0; j < queueFamily.queueCount; ++j)
            qf.m_Priorities.push_back(1.0f);

        m_QueueFamilies.push_back(qf);
    }
}

void VulkanCore::vkInitAssignQqueues() {

    m_GraphicsQueue = {};
    m_AdditionalGraphicsQueues.clear();
    m_ComputeQueues.clear();
    m_SparseBindingQueues.clear();
    m_TransferOnlyQueues.clear();

    bool graphicsAssigned = false;

    for(uint32_t i = 0; i < static_cast<uint32_t >(m_QueueFamilies.size()); ++i)
    {
        vk_queue_family& qf = m_QueueFamilies[i];

        if(qf.m_QueueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            for(uint32_t j = 0; j < qf.m_QueueFamilyProperties.queueCount; ++j)
            {
                if(!graphicsAssigned)
                {
                    //Assign as main graphics queue
                    VkBool32 presentSupport = VK_FALSE;
                    vkGetPhysicalDeviceSurfaceSupportKHR(m_PhysicalDevice, qf.m_Index, m_Surface, &presentSupport);
                    if(presentSupport == VK_FALSE)
                        continue;

                    vkGetDeviceQueue(m_Device, qf.m_Index, j, &m_GraphicsQueue.m_Queue);
                    m_GraphicsQueue.m_FamilyIndex = qf.m_Index;

                    graphicsAssigned = true;
                } else
                {
                    m_AdditionalGraphicsQueues.push_back(vk_queue{});
                    vkGetDeviceQueue(m_Device, qf.m_Index, j, &m_AdditionalGraphicsQueues.back().m_Queue);
                    m_AdditionalGraphicsQueues.back().m_FamilyIndex = qf.m_Index;
                }
            }
        }

        if(qf.m_QueueFamilyProperties.queueFlags & VK_QUEUE_COMPUTE_BIT)
        {
            for(uint32_t j = 0; j < qf.m_QueueFamilyProperties.queueCount; ++j)
            {
                m_ComputeQueues.push_back(vk_queue{});
                vkGetDeviceQueue(m_Device, qf.m_Index, j, &m_ComputeQueues.back().m_Queue);
                m_ComputeQueues.back().m_FamilyIndex = qf.m_Index;
            }
        }

        if(qf.m_QueueFamilyProperties.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT)
        {
            for(uint32_t j = 0; j < qf.m_QueueFamilyProperties.queueCount; ++j)
            {
                m_SparseBindingQueues.push_back(vk_queue{});
                vkGetDeviceQueue(m_Device, qf.m_Index, j, &m_SparseBindingQueues.back().m_Queue);
                m_SparseBindingQueues.back().m_FamilyIndex = qf.m_Index;
            }
        }

        if(qf.m_QueueFamilyProperties.queueFlags & VK_QUEUE_TRANSFER_BIT && !qf.m_QueueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT && !qf.m_QueueFamilyProperties.queueFlags & VK_QUEUE_COMPUTE_BIT)
        {
            for(uint32_t j = 0; j < qf.m_QueueFamilyProperties.queueCount; ++j)
            {
                m_TransferOnlyQueues.push_back(vk_queue{});
                vkGetDeviceQueue(m_Device, qf.m_Index, j, &m_TransferOnlyQueues.back().m_Queue);
                m_TransferOnlyQueues.back().m_FamilyIndex = qf.m_Index;
            }
        }
    }
}

void VulkanCore::vkInitCreateSurface() {

    VkResult result;

    result = m_Platform.CreateSurface(m_Instance, m_Surface);

    vk_if_fail_throw_message(result, "Error creating surface.");
}

bool VulkanCore::vkInitCheckDevice(const VkPhysicalDevice deviceToCheck, const vector<const char *> &deviceExtentions, const VkSurfaceKHR surface) {

    VkPhysicalDeviceProperties  deviceProperties;
    VkPhysicalDeviceFeatures    deviceFeatures;

    vkGetPhysicalDeviceProperties(deviceToCheck, &deviceProperties);
    vkGetPhysicalDeviceFeatures(deviceToCheck, &deviceFeatures);

    cout << deviceProperties << std::endl;

    vk_physical_device_info deviceInfo = vkInitGetQueueFamilies(deviceToCheck);

    bool supportsGraphics = false;

    for(uint32_t index = 0; index < static_cast<uint32_t >(deviceInfo.m_QueueFamilyProperties.size()); ++index)
    {
        auto& queueFamily = deviceInfo.m_QueueFamilyProperties[index];
        if(queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            supportsGraphics = true;
            break;
        }
    }

    vk_swapchain_details details = fillSwapChainDetails(deviceToCheck, surface);
    if (!checkDeviceExtensions(deviceToCheck, deviceExtentions)) {
        return false;
    } else {
        if (!checkSwapChainDetails(details))
            return false;
    }

    return supportsGraphics && (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU || deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) && deviceFeatures.geometryShader && deviceFeatures.tessellationShader;
}

int32_t VulkanCore::vkInitSuitabilityRating(const VkPhysicalDevice deviceToRate) {

    int32_t rating = 0;

    VkPhysicalDeviceProperties  deviceProperties;
    VkPhysicalDeviceFeatures    deviceFeatures;

    vkGetPhysicalDeviceProperties(deviceToRate, &deviceProperties);
    vkGetPhysicalDeviceFeatures(deviceToRate, &deviceFeatures);

    if(deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        rating += 1000;

    rating += deviceProperties.limits.maxImageDimension2D;

    return rating;
}

vk_physical_device_info VulkanCore::vkInitGetQueueFamilies(const VkPhysicalDevice device) {

    uint32_t queueFamilyCount = 0;
    vector<VkQueueFamilyProperties> queueFamilies;

    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    if(queueFamilyCount == 0)
    {
        throw std::runtime_error("No device queue families found.");
    }

    queueFamilies.resize(queueFamilyCount);

    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    for(const auto& queueFamily : queueFamilies)
    {
        cout << queueFamily << std::endl;
    }

    vk_physical_device_info info;
    info.m_PhysicalDevice = device;
    info.m_QueueFamilyProperties = queueFamilies;
    return info;
}

void VulkanCore::vkInitCreateSwapchain() {

    m_SwapchainImages.clear();

    vk_swapchain_details swapchainDetails = fillSwapChainDetails(m_PhysicalDevice, m_Surface);

    VkSurfaceFormatKHR surfaceFormatKHR = pickSwapChainSurfaceFormat(swapchainDetails);
    VkPresentModeKHR presentModeKHR = pickSwapChainPresentMode(swapchainDetails);
    VkExtent2D extent2D = pickSwapChainExtent2D(swapchainDetails, WIDTH, HEIGHT);

    uint32_t imageCount = swapchainDetails.m_capabilities.minImageCount + 1;
    if(swapchainDetails.m_capabilities.maxImageCount > 0 && imageCount > swapchainDetails.m_capabilities.maxImageCount)
    {
        imageCount = swapchainDetails.m_capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfoKHR          = {};
    createInfoKHR.sType                             = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfoKHR.pNext                             = nullptr;
    createInfoKHR.surface                           = m_Surface;
    createInfoKHR.minImageCount                     = imageCount;
    createInfoKHR.imageFormat                       = surfaceFormatKHR.format;
    createInfoKHR.imageColorSpace                   = surfaceFormatKHR.colorSpace;
    createInfoKHR.imageExtent                       = extent2D;
    //Engine does not support stereoscopic 3d as of yet so 1 image array layer
    createInfoKHR.imageArrayLayers                  = 1;
    createInfoKHR.imageUsage                        = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    //Currently presentation queue and graphics queue are the same queues, so exclusive mode is okay
    createInfoKHR.imageSharingMode                  = VK_SHARING_MODE_EXCLUSIVE;
    createInfoKHR.queueFamilyIndexCount             = 0;
    createInfoKHR.pQueueFamilyIndices               = nullptr;

    //No pretransformation
    createInfoKHR.preTransform                      = swapchainDetails.m_capabilities.currentTransform;
    //Ignore alpha blending with the windowing system, well, for obvious reasons i guess :)
    createInfoKHR.compositeAlpha                    = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfoKHR.presentMode                       = presentModeKHR;
    createInfoKHR.clipped                           = VK_TRUE;

    //TODO: Set up swapchain stuff here in a proper way for later when resizing windows and whatnot
    //if(m_Swapchain != VK_NULL_HANDLE)
       // createInfoKHR.oldSwapchain = m_Swapchain;
    //else
        createInfoKHR.oldSwapchain = VK_NULL_HANDLE;

    VkResult result = vkCreateSwapchainKHR(m_Device, &createInfoKHR, nullptr, &m_Swapchain);
    vk_if_fail_throw_message(result, "Error creating swapchain.");

    uint32_t swapImageCount = 0;
    vkGetSwapchainImagesKHR(m_Device, m_Swapchain, &swapImageCount, nullptr);

    m_SwapchainImages.resize(swapImageCount);
    vkGetSwapchainImagesKHR(m_Device, m_Swapchain, &swapImageCount, m_SwapchainImages.data());

    m_SwapchainExtent = extent2D;
    m_SwapchainFormat = surfaceFormatKHR.format;

}

void VulkanCore::vkInitCreateSwapchainImageViews() {

    VkResult result;

    cleanUpSwapchainImageViews();
    m_SwapchainImageViews.resize(m_SwapchainImages.size());

    //Default rgba color channel set
    VkComponentMapping componentMapping {VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY};

    //Default sub resource mapping, as color buffer
    VkImageSubresourceRange subresourceRange;
    subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.baseArrayLayer = 0;
    subresourceRange.baseMipLevel   = 0;
    subresourceRange.layerCount     = 1;
    subresourceRange.levelCount     = 1;



    for(uint32_t i = 0; i < static_cast<uint32_t >(m_SwapchainImages.size()); ++i)
    {

        VkImageViewCreateInfo createInfo    = {};
        createInfo.sType                    = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.pNext                    = nullptr;
        createInfo.image                    = m_SwapchainImages[i];
        createInfo.components               = componentMapping;
        createInfo.subresourceRange         = subresourceRange;
        createInfo.viewType                 = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format                   = m_SwapchainFormat;

        result = vkCreateImageView(m_Device, &createInfo, nullptr, &m_SwapchainImageViews[i]);
        vk_if_fail_throw_message(result, "Error when creating image view.");
    }

}

void VulkanCore::vkInitCreatePipeline() {

    ShaderModule vertex = ShaderModule("vert.spv", ShaderModuleType::Vertex,m_Device);
    ShaderModule fragment = ShaderModule("frag.spv", ShaderModuleType ::Fragment, m_Device);

    array<VkPipelineShaderStageCreateInfo,2> shaderStages                           = { vertex.info(), fragment.info() };

    m_Scissors = Pipeline::defaultScissorRectangle(m_SwapchainExtent);
    m_Viewport = Pipeline::defaultViewport(m_SwapchainExtent);
    VkResult result = Pipeline::defaultPipelineLayout(&m_PipelineLayout, m_Device);
    vk_if_fail_throw_message(result, "Error creating pipeline layout.");

    VkPipelineVertexInputStateCreateInfo    vertexInputStateCreateInfo              = Pipeline::defaultVertexInputState();
    VkPipelineInputAssemblyStateCreateInfo  inputAssemblyStateCreateInfo            = Pipeline::defaultInputAssemplyState();
    VkPipelineViewportStateCreateInfo       viewportStateCreateInfo                 = Pipeline::defaultViewPortState(m_Viewport, m_Scissors);
    VkPipelineRasterizationStateCreateInfo  rasterizationStateCreateInfo            = Pipeline::defaultRasterizerState();
    VkPipelineMultisampleStateCreateInfo    multisampleStateCreateInfo              = Pipeline::defaultMultisampleState();

    //TODO: Implement depth stencilling.
    //VkPipelineDepthStencilStateCreateInfo   depthStencilStateCreateInfo             = Pipeline::defaultDepthStencilState();

    VkPipelineColorBlendAttachmentState     colorBlendAttachmentState               = Pipeline::defaultColorBlendAttachmentState();
    vector<VkPipelineColorBlendAttachmentState> attachments = {colorBlendAttachmentState};
    VkPipelineColorBlendStateCreateInfo     colorBlendStateCreateInfo               = Pipeline::defaultColorBlendState(attachments);
    VkPipelineDynamicStateCreateInfo        dynamicStateCreateInfo                  = Pipeline::defaultDynamicState();




    VkGraphicsPipelineCreateInfo info           = {};
    info.sType                                  = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    info.stageCount                             = static_cast<uint32_t >(shaderStages.size());
    info.pStages                                = shaderStages.data();

    info.pVertexInputState                      = &vertexInputStateCreateInfo;
    info.pInputAssemblyState                    = &inputAssemblyStateCreateInfo;
    info.pViewportState                         = &viewportStateCreateInfo;
    info.pRasterizationState                    = &rasterizationStateCreateInfo;
    info.pMultisampleState                      = &multisampleStateCreateInfo;
    info.pDepthStencilState                     = nullptr; //inplement later!!
    info.pColorBlendState                       = &colorBlendStateCreateInfo;
    info.pDynamicState                          = &dynamicStateCreateInfo;
    info.layout                                 = m_PipelineLayout;
    info.renderPass                             = m_RenderPass;
    info.subpass                                = 0;
    info.basePipelineHandle                     = VK_NULL_HANDLE;
    info.basePipelineIndex                      = 0;

    result = vkCreateGraphicsPipelines(m_Device, VK_NULL_HANDLE, 1, &info, nullptr, &m_Pipeline);
    vk_if_fail_throw_message(result, "Error creating pipeline.");

    m_ShaderModules.emplace_back(std::move(vertex));
    m_ShaderModules.emplace_back(std::move(fragment));
}

void VulkanCore::vkInitCreateRenderPass() {

    VkAttachmentDescription attachment      = {};
    attachment.format                       = m_SwapchainFormat;
    attachment.samples                      = VK_SAMPLE_COUNT_1_BIT;

    attachment.loadOp                       = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachment.storeOp                      = VK_ATTACHMENT_STORE_OP_STORE;

    attachment.stencilLoadOp                = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachment.stencilStoreOp               = VK_ATTACHMENT_STORE_OP_DONT_CARE;

    attachment.initialLayout                = VK_IMAGE_LAYOUT_UNDEFINED;
    attachment.finalLayout                  = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference reference         = {};
    reference.attachment                    = 0;
    reference.layout                        = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass            = {};
    subpass.pipelineBindPoint               = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount            = 1;
    subpass.pColorAttachments               = &reference;

    VkSubpassDependency dependency          = {};
    dependency.srcSubpass                   = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass                   = 0;

    dependency.srcStageMask                 = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask                = 0;

    dependency.dstStageMask                 = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask                = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassInfo   = {};
    renderPassInfo.sType                    = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount          = 1;
    renderPassInfo.pAttachments             = &attachment;
    renderPassInfo.subpassCount             = 1;
    renderPassInfo.pSubpasses               = &subpass;
    renderPassInfo.dependencyCount          = 1;
    renderPassInfo.pDependencies            = &dependency;

    VkResult result = vkCreateRenderPass(m_Device, &renderPassInfo, nullptr, &m_RenderPass);
    vk_if_fail_throw_message(result, "Failed to create render pass.");

}

void VulkanCore::vkInitCreateFrameBuffers()
{
    m_SwapchainFrameBuffers.clear();
    m_SwapchainFrameBuffers.resize(m_SwapchainImageViews.size());

    for (uint32_t i = 0; i < static_cast<uint32_t >(m_SwapchainImageViews.size()); i++) {
        VkImageView attachments[] = {
                m_SwapchainImageViews[i]
        };

        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType                   = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass              = m_RenderPass;
        framebufferInfo.attachmentCount         = 1;
        framebufferInfo.pAttachments            = attachments;
        framebufferInfo.width                   = m_SwapchainExtent.width;
        framebufferInfo.height                  = m_SwapchainExtent.height;
        framebufferInfo.layers                  = 1;

        VkResult result = vkCreateFramebuffer(m_Device, &framebufferInfo, nullptr, &m_SwapchainFrameBuffers[i]);
        vk_if_fail_throw_message(result, "Error creating frame buffer");
    }
}

void VulkanCore::vkInitCreateCommandPool() {

    VkCommandPoolCreateInfo info    = {};

    info.sType                      = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    info.pNext                      = nullptr;
    info.flags                      = 0;
    info.queueFamilyIndex           = m_GraphicsQueue.m_FamilyIndex;

    VkResult result = vkCreateCommandPool(m_Device, &info, nullptr, &m_CommandPool);
    vk_if_fail_throw_message(result, "Error creating command pool.");
}

void VulkanCore::vkInitCreateCommandBuffers()
{
    m_CommandBuffers.clear();
    m_CommandBuffers.resize(m_SwapchainFrameBuffers.size());

    VkCommandBufferAllocateInfo info    = {};
    info.sType                          = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    info.commandPool                    = m_CommandPool;
    info.level                          = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    info.commandBufferCount             = static_cast<uint32_t >(m_CommandBuffers.size());

    VkResult result = vkAllocateCommandBuffers(m_Device, &info, m_CommandBuffers.data());
    vk_if_fail_throw_message(result, "Error allocating command buffers");

    for (uint32_t i = 0; i < static_cast<uint32_t >(m_CommandBuffers.size()); i++) {

        VkCommandBufferBeginInfo beginInfo      = {};
        beginInfo.sType                         = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags                         = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
        beginInfo.pInheritanceInfo              = nullptr; // Optional

        vkBeginCommandBuffer(m_CommandBuffers[i], &beginInfo);


        VkRenderPassBeginInfo renderPassInfo    = {};
        renderPassInfo.sType                    = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass               = m_RenderPass;
        renderPassInfo.framebuffer              = m_SwapchainFrameBuffers[i];

        renderPassInfo.renderArea.offset        = {0, 0};
        renderPassInfo.renderArea.extent        = m_SwapchainExtent;

        VkClearValue clearColor                 = {0.0f, 0.0f, 0.0f, 1.0f};
        renderPassInfo.clearValueCount          = 1;
        renderPassInfo.pClearValues             = &clearColor;

        vkCmdBeginRenderPass(m_CommandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(m_CommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_Pipeline);

        vkCmdSetViewport(m_CommandBuffers[i], 0, 1, &m_Viewport);
        vkCmdSetScissor(m_CommandBuffers[i], 0, 1, &m_Scissors);

        vkCmdDraw(m_CommandBuffers[i], 3, 1, 0, 0);

        vkCmdEndRenderPass(m_CommandBuffers[i]);

        VkResult result2 = vkEndCommandBuffer(m_CommandBuffers[i]);
        vk_if_fail_throw_message(result2, "Error recording command buffer.");
    }
}

void VulkanCore::vkInitCreateSwapchainSemaphores() {

    VkSemaphoreCreateInfo info  = {};
    info.sType                  = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    info.pNext                  = nullptr;

    VkResult result = vkCreateSemaphore(m_Device, &info, nullptr, &m_ImageAvailableSemaphore);
    vk_if_fail_throw_message(result, "Failed to create image available semaphore.");
    result = vkCreateSemaphore(m_Device, &info, nullptr, &m_RenderingFinsihedSemaphore);
    vk_if_fail_throw_message(result, "Failed to create renderingfinishedsemaphore");


}


























