//
// Created by hindrik on 16-4-17.
//

#ifndef VULKANOENGINE_WVP_H
#define VULKANOENGINE_WVP_H



using namespace glm;

struct WorldViewProjectionUBO
{
    mat4 world;
    mat4 view;
    mat4 projection;
};




#endif //VULKANOENGINE_WVP_H
