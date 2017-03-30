//
// Created by hindrik on 14-2-17.
//

#ifndef VULKANOENGINE_SHADERMODULE_H
#define VULKANOENGINE_SHADERMODULE_H

#include <vector>
#include <string>
#include "../VulkanPlatforms/VulkanPlatform.h"
#include "../../Utilities/VulkanEnums.h"
#include "../VkUniqueHandle.h"

using std::vector;
using std::string;

class ShaderModule : NonCopyable
{
private:
    VkUniqueHandle<VkShaderModule>      m_ShaderModule      = {m_Device, vkDestroyShaderModule};
    VkDevice                            m_Device            = VK_NULL_HANDLE;
    string                              m_FileName          = "";
    ShaderModuleType                    m_ShaderModuleType  = ShaderModuleType::Null;
    VkPipelineShaderStageCreateInfo     m_ShaderCreateInfo  = {};
public:
    ShaderModule(const string& fileName, ShaderModuleType shaderType, VkDevice device);
    ~ShaderModule();

    ShaderModule (ShaderModule&&)           = default;
    ShaderModule& operator=(ShaderModule&&) = default;

    static vector<char> readSpirFile(const string& fileName);
    static VkPipelineShaderStageCreateInfo createInfoStructure(const ShaderModule& module);

    ShaderModuleType type();
    VkPipelineShaderStageCreateInfo info();
    VkDevice device();
    string fileName();
    VkShaderModule shaderModule();
};


#endif //VULKANOENGINE_SHADERMODULE_H
