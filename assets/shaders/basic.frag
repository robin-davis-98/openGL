#version 430 core
out vec4 FragColor;

in vec3 vNormal;
in vec3 vWorldPos;

struct PointLight {
    vec4 position;
    vec4 colour;
};

layout(std430, binding = 1) buffer LightBuffer {
    uint count;
    uint _padding[3];
    PointLight lights[];
};

uniform vec3 uBaseColour;

void main() {
    vec3 normal = normalize(vNormal);
    vec3 viewDir = normalize(vec3(0,0,5) - vWorldPos); // Hardcoded camera pos for test
    vec3 lighting = vec3(0.0);

    for(uint i = 0; i < count; i++) {
        vec3 lightPos = lights[i].position.xyz;
        float radius = lights[i].position.w;
        vec3 lightCol = lights[i].colour.rgb;
        float intensity = lights[i].colour.w;

        vec3 lightDir = normalize(lightPos - vWorldPos);
        float dist = length(lightPos - vWorldPos);

        // FALLOFF: Using a smoother falloff to avoid hard black edges
        float attenuation = pow(clamp(1.0 - (dist / radius), 0.0, 1.0), 2.0) * intensity;

        // DIFFUSE: Ensure we aren't getting negative values
        float diff = max(dot(normal, lightDir), 0.0);
        
        lighting += lightCol * diff * attenuation;
    }

    // AMBIENT: Increase this to 0.2 to see if the cube shape is actually there
    vec3 ambient = uBaseColour * 0.2;
    vec3 diffuse = uBaseColour * lighting;

    FragColor = vec4(ambient + diffuse, 1.0);
}