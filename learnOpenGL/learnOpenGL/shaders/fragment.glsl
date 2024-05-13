#version 330 core
out vec4 FragColor;

in vec3 ourColour;
in vec4 vertexPosition;

void main()
{
	FragColor = vertexPosition;
}