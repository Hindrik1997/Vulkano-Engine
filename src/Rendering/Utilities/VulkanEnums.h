//
// Created by hindrik on 14-2-17.
//

#ifndef VULKANOENGINE_VULKANENUMS_H
#define VULKANOENGINE_VULKANENUMS_H

enum class ShaderModuleType : uint8_t
{
    Null,
    Vertex = VK_SHADER_STAGE_VERTEX_BIT,
    Geometry = VK_SHADER_STAGE_GEOMETRY_BIT,
    Fragment = VK_SHADER_STAGE_FRAGMENT_BIT,

};

enum class SampleCount : uint8_t
{
    None        = VK_SAMPLE_COUNT_1_BIT,
    Two         = VK_SAMPLE_COUNT_2_BIT,
    Four        = VK_SAMPLE_COUNT_4_BIT,
    Eight       = VK_SAMPLE_COUNT_8_BIT,
    Sixteen     = VK_SAMPLE_COUNT_16_BIT,
    ThirtyTwo   = VK_SAMPLE_COUNT_32_BIT,
    SixtyFour   = VK_SAMPLE_COUNT_64_BIT,
};

enum class CommandBufferLevel : uint8_t
{
    Primary = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
    Secondary = VK_COMMAND_BUFFER_LEVEL_SECONDARY,
};

#endif //VULKANOENGINE_VULKANENUMS_H
