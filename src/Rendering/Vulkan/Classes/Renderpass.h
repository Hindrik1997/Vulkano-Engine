//
// Created by hindrik on 30-3-17.
//

#ifndef VULKANOENGINE_RENDERPASS_H
#define VULKANOENGINE_RENDERPASS_H

#include "../VkUniqueHandle.h"
#include <vector>

using std::vector;

class Renderpass final
{
private:
    VkUniqueHandle<VkRenderPass>    m_Renderpass;
public:
    Renderpass(VkDevice device, const vector<VkAttachmentDescription>& imageAttachments, const vector<VkSubpassDescription>& subpasses);
    Renderpass(VkDevice device, const vector<VkAttachmentDescription>& imageAttachments, const vector<VkSubpassDescription>& subpasses, const vector<VkSubpassDependency>& dependecies);

    Renderpass(Renderpass&&)            = default;
    Renderpass& operator=(Renderpass&&) = default;

    Renderpass(const Renderpass&) = delete;
    Renderpass& operator=(const Renderpass&) = delete;

    ~Renderpass() = default;

public:
    VkRenderPass renderpass()const;
};


#endif //VULKANOENGINE_RENDERPASS_H
