//
// Created by hindrik on 14-2-17.
//


#include <ios>
#include <fstream>

#include "ShaderModule.h"
#include "../../../Core/Logger.h"
#include "../../Utilities/VulkanUtilityFunctions.h"



auto ShaderModule::readSpirFile(const std::string &fileName) -> vector<char>
{

    std::ifstream file("shaders/" + fileName, std::ios::ate | std::ios::binary);

    if(!file.is_open())
    {
        Logger::failure("Error when opening file!");
    }

    size_t fileSize = static_cast<size_t >(file.tellg());
    vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    return buffer;
}

ShaderModule::ShaderModule(const string& fileName, ShaderModuleType shaderType, VkDevice device) : m_FileName(fileName), m_Device(device), m_ShaderModuleType(shaderType) {

    vector<char> data = readSpirFile(m_FileName);

    //alignment
    vector<uint32_t> alignedData(data.size() / sizeof(uint32_t) + 1);
    memcpy(alignedData.data(), data.data(), data.size());

    VkShaderModuleCreateInfo info   = {};
    info.sType                      = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    info.pNext                      = nullptr;
    info.codeSize                   = static_cast<uint32_t >(data.size());
    info.pCode                      = alignedData.data();
    info.flags                      = {};

    VkResult result = vkCreateShaderModule(m_Device, &info, nullptr, m_ShaderModule.reset());
    vkIfFailThrowMessage(result,"Error creating shader module!");

    m_ShaderCreateInfo = createInfoStructure(*this);
}

ShaderModule::~ShaderModule()
{
}

auto ShaderModule::createInfoStructure(const ShaderModule& module) -> VkPipelineShaderStageCreateInfo
{

    VkPipelineShaderStageCreateInfo info    = {};
    info.sType                              = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    info.pNext                              = nullptr;
    info.module                             = module.m_ShaderModule;
    info.stage                              = static_cast<VkShaderStageFlagBits>(module.m_ShaderModuleType);
    info.pName                              = "main";

    //TODO: not use this atm, but is good for later.
    info.pSpecializationInfo = nullptr;

    return info;
}

auto ShaderModule::shaderModule() -> VkShaderModule
{
    return m_ShaderModule;
}

auto ShaderModule::fileName() -> string
{
    return m_FileName;
}

auto ShaderModule::device() -> VkDevice
{
    return m_Device;
}

auto ShaderModule::info() -> VkPipelineShaderStageCreateInfo
{
    return m_ShaderCreateInfo;
}

auto ShaderModule::type() -> ShaderModuleType
{
    return m_ShaderModuleType;
}
