#version 430 core
out vec4 FragColor;

in vec2 vUv;

uniform sampler2D u_Texture;
uniform vec4 u_Colour;
uniform bool u_HasTexture;

void main()
{
    vec4 texColour = vec4(1.0);
    if (u_HasTexture)
    {
        texColour = texture(u_Texture, vUv);
    }

    FragColor = texColour * u_Colour;

    if (FragColor.a < 0.05) discard; 
}