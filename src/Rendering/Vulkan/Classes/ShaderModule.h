//
// Created by hindrik on 14-2-17.
//

#ifndef VULKANOENGINE_SHADERMODULE_H
#define VULKANOENGINE_SHADERMODULE_H

#include <vector>
#include <string>
#include "../VulkanPlatforms/VulkanPlatform.h"
#include "../../Utilities/VulkanEnums.h"

using std::vector;
using std::string;

class ShaderModule {
private:
    VkShaderModule                      m_ShaderModule      = VK_NULL_HANDLE;
    VkDevice                            m_Device            = VK_NULL_HANDLE;
    string                              m_FileName          = "";
    ShaderModuleType                    m_ShaderModuleType  = ShaderModuleType::Null;
    VkPipelineShaderStageCreateInfo     m_ShaderCreateInfo  = {};
public:
    ShaderModule(const string& fileName, ShaderModuleType shaderType, VkDevice device);
    ~ShaderModule();
    ShaderModule (const ShaderModule& obj) = delete;
    ShaderModule (ShaderModule&& obj);

    static vector<char> readSpirFile(const string& fileName);
    static VkPipelineShaderStageCreateInfo createInfoStructure(const ShaderModule& module);

    ShaderModuleType type();
    VkPipelineShaderStageCreateInfo info();
    VkDevice device();
    string fileName();
    VkShaderModule shaderModule();
};


#endif //VULKANOENGINE_SHADERMODULE_H
