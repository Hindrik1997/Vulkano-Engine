//
// Created by hindrik on 16-4-17.
//

#ifndef VULKANOENGINE_TRANSFORM_H
#define VULKANOENGINE_TRANSFORM_H

#include "../Core/glm/glm.hpp"
#include "../Core/glm/gtc/quaternion.hpp"
#include "../Core/glm/gtx/quaternion.hpp"

struct Transform
{
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;

};


#endif //VULKANOENGINE_TRANSFORM_H
