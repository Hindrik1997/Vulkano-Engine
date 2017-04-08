//
// Created by hindrik on 8-4-17.
//

#ifndef VULKANOENGINE_RENDERPASSRESOURCEMANAGER_H
#define VULKANOENGINE_RENDERPASSRESOURCEMANAGER_H


#include "../../Data Storage/CacheOptimizedStorage.h"
#include "../Vulkan/Classes/PipelineStateObject.h"
#include "../Vulkan/Classes/Renderpass.h"

class RenderpassResourceManager final
{
private:
    Renderpass m_Renderpass;
    CacheOptimizedStorage<PipelineStateObject> m_PSOs;
public:
    RenderpassResourceManager(Renderpass&& renderpass);
    ~RenderpassResourceManager() = default;
    RenderpassResourceManager(RenderpassResourceManager&&) = default;
    RenderpassResourceManager(const RenderpassResourceManager&) = delete;

    RenderpassResourceManager& operator=(RenderpassResourceManager&&) = default;
    RenderpassResourceManager& operator=(const RenderpassResourceManager&) = delete;

public:
};


#endif //VULKANOENGINE_RENDERPASSRESOURCEMANAGER_H
