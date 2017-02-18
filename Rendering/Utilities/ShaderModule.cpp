//
// Created by hindrik on 14-2-17.
//

#include <bits/ios_base.h>
#include <ios>
#include <fstream>
#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif
#include "ShaderModule.h"
#include "../../Core/Console.h"
#include "../VulkanCore.h"


std::string GetCurrentWorkingDir( void ) {
    char buff[FILENAME_MAX];
    GetCurrentDir( buff, FILENAME_MAX );
    std::string current_working_dir(buff);
    return current_working_dir;
}


vector<char> ShaderModule::readSpirFile(const std::string &fileName) {

    std::ifstream file("../Shaders/" + fileName, std::ios::ate | std::ios::binary);

    if(!file.is_open())
    {
        throw std::runtime_error("Error when opening file!");
    }

    size_t fileSize = static_cast<size_t >(file.tellg());
    vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    return buffer;
}

ShaderModule::ShaderModule(const string& fileName, ShaderModuleType shaderType, VkDevice device) : m_FileName(fileName), m_Device(device), m_ShaderModuleType(shaderType) {

    vector<char> data = readSpirFile(m_FileName);

    VkShaderModuleCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    info.pNext = nullptr;
    info.codeSize = data.size();
    info.pCode = reinterpret_cast<uint32_t *>(data.data());
    info.flags = {};

    VkResult result = vkCreateShaderModule(m_Device, &info, nullptr, &m_ShaderModule);
    if(result != VK_SUCCESS)
        throw std::runtime_error("Erorr creating shader module.");

    m_ShaderCreateInfo = createInfoStructure(*this);
}

ShaderModule::~ShaderModule() {
    if(m_ShaderModule != VK_NULL_HANDLE)
    {
        vkDestroyShaderModule(m_Device, m_ShaderModule, nullptr);
    }
}

ShaderModule::ShaderModule(ShaderModule &&obj) {
    m_Device = obj.m_Device;
    m_ShaderModule = obj.m_ShaderModule;
    m_FileName = obj.m_FileName;
    m_ShaderModuleType = obj.m_ShaderModuleType;
    obj.m_ShaderModule = VK_NULL_HANDLE;
    obj.m_Device = VK_NULL_HANDLE;
}

VkPipelineShaderStageCreateInfo ShaderModule::createInfoStructure(const ShaderModule& module) {

    VkPipelineShaderStageCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    info.pNext = nullptr;
    info.module = module.m_ShaderModule;
    info.stage = static_cast<VkShaderStageFlagBits>(module.m_ShaderModuleType);
    info.pName = "main";

    //TODO: not use this atm, but is good for later.
    info.pSpecializationInfo = nullptr;

    return info;
}

VkShaderModule ShaderModule::shaderModule() {
    return m_ShaderModule;
}

string ShaderModule::fileName() {
    return m_FileName;
}

VkDevice ShaderModule::device() {
    return m_Device;
}

VkPipelineShaderStageCreateInfo ShaderModule::info() {
    return m_ShaderCreateInfo;
}

ShaderModuleType ShaderModule::type() {
    return m_ShaderModuleType;
}
