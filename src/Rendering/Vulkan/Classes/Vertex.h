//
// Created by hindrik on 15-4-17.
//

#ifndef VULKANOENGINE_VERTEX_H
#define VULKANOENGINE_VERTEX_H

#include "../VulkanPlatforms/VulkanPlatform.h"
#include <glm/glm.hpp>

struct Vertex
{
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;

    static VkVertexInputBindingDescription bindingDescription()
    {
        VkVertexInputBindingDescription description = {};

        description.binding = 0;
        description.stride = sizeof(Vertex);
        description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return description;
    }

    static array<VkVertexInputAttributeDescription, 3> attributeDescriptions()
    {
        array<VkVertexInputAttributeDescription, 3> descriptions = {};

        descriptions[0].binding = 0;
        descriptions[0].location = 0;
        descriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        descriptions[0].offset = offsetof(Vertex, pos);

        descriptions[1].binding = 0;
        descriptions[1].location = 1;
        descriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        descriptions[1].offset = offsetof(Vertex, color);

        descriptions[2].binding = 0;
        descriptions[2].location = 2;
        descriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        descriptions[2].offset = offsetof(Vertex, texCoord);


        return descriptions;
    };



};


#endif //VULKANOENGINE_VERTEX_H
