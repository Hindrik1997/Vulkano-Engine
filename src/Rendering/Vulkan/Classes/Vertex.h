//
// Created by hindrik on 15-4-17.
//

#ifndef VULKANOENGINE_VERTEX_H
#define VULKANOENGINE_VERTEX_H

#include "../../../Core/glm/glm.hpp"
#include "../VulkanPlatforms/VulkanPlatform.h"

struct Vertex
{
    glm::vec2 pos;
    glm::vec3 color;

    static VkVertexInputBindingDescription bindingDescription()
    {
        VkVertexInputBindingDescription description = {};

        description.binding = 0;
        description.stride = sizeof(Vertex);
        description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return description;
    }

    static array<VkVertexInputAttributeDescription, 2> attributeDescriptions()
    {
        array<VkVertexInputAttributeDescription, 2> descriptions = {};

        descriptions[0].binding = 0;
        descriptions[0].location = 0;
        descriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        descriptions[0].offset = offsetof(Vertex, pos);

        descriptions[1].binding = 0;
        descriptions[1].location = 1;
        descriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        descriptions[1].offset = offsetof(Vertex, color);



        return descriptions;
    };



};


#endif //VULKANOENGINE_VERTEX_H
