#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColour;

out vec3 ourColour;
out vec4 vertexPosition;

uniform float vertexOffset;

void main()
{
    gl_Position = vec4(aPos * -1, 1.0);
    ourColour = aColour;
    vertexPosition = vec4(aPos, 1.0);
}