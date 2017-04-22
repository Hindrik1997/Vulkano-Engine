#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform PerObjectUBO
{
    mat4 world;
    mat4 view;
    mat4 projection;
}   perObject;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

out gl_PerVertex {
    vec4 gl_Position;
};

void main() {
    gl_Position = perObject.projection * perObject.view * perObject.world * vec4(inPosition, 1.0);
    fragColor = inColor;
}