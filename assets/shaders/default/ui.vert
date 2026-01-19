#version 430 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aUv;

uniform mat4 u_OrthoProjection;

out vec2 vUv;

void main()
{
    vUv = aUv;
    gl_Position = u_OrthoProjection * vec4(aPos, 0.0, 1.0);
}