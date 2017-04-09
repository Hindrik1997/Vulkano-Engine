//
// Created by hindrik on 14-2-17.
//

#ifndef VULKANOENGINE_VULKANENUMS_H
#define VULKANOENGINE_VULKANENUMS_H

enum class ShaderModuleType : char
{
    Null,
    Vertex = VK_SHADER_STAGE_VERTEX_BIT,
    Geometry = VK_SHADER_STAGE_GEOMETRY_BIT,
    Fragment = VK_SHADER_STAGE_FRAGMENT_BIT,

};


#endif //VULKANOENGINE_VULKANENUMS_H
