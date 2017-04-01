//
// Created by hindrik on 30-3-17.
//

#ifndef VULKANOENGINE_MANAGEDTARGETOUTPUT_H
#define VULKANOENGINE_MANAGEDTARGETOUTPUT_H

#include "../../Utility Classes/NonCopyable.h"
#include "VulkanPipelineManager.h"
#include "../Vulkan/Classes/PipelineStateObject.h"
#include "../Vulkan/Classes/Renderpass.h"

struct ManagedTargetOutput : NonCopyable
{
    RenderTargetOutput      m_Output;
    VulkanPipelineManager   m_Manager;

    ManagedTargetOutput(RenderTargetOutput&& output);

    ManagedTargetOutput(ManagedTargetOutput&&) = default;
    ManagedTargetOutput& operator=(ManagedTargetOutput&&) = default;
};


#endif //VULKANOENGINE_MANAGEDTARGETOUTPUT_H
