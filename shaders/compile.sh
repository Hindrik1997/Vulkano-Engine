#!/usr/bin/env bash

#pwd
#ls

#$VULKAN_SDK/bin/glslangValidator -V shader.vert

echo "Vertex shaders:"
for file in *.vert
do
    ${VULKAN_SDK}/bin/glslangValidator -V "$file"
done

echo "Fragment shaders:"
for file in *.frag
do
    ${VULKAN_SDK}/bin/glslangValidator -V "$file"
done