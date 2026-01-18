#version 430 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec3 vNormal;
out vec2 vTexCoords;
out vec3 vWorldPos;

layout (std140) uniform CameraData
{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    vWorldPos = worldPos.xyz;
    
    vNormal = mat3(transpose(inverse(model))) * aNormal; 
    
    vTexCoords = aTexCoords;
    gl_Position = projection * view * worldPos;
}