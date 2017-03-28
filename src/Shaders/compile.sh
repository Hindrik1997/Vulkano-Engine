#!/usr/bin/env bash

#pwd
#ls

#$VULKAN_SDK/bin/glslangValidator -V shader.vert

echo "Vertex Shaders:"
for file in *.vert
do
    ${VULKAN_SDK}/bin/glslangValidator -V "$file"
done

echo "Fragment Shaders:"
for file in *.frag
do
    ${VULKAN_SDK}/bin/glslangValidator -V "$file"
done