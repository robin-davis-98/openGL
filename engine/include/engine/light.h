#pragma once
#include <glm/glm.hpp>
#include "engine/engine.h"

struct PointLight
{
    glm::vec4 position;
    glm::vec4 colour;
};

struct LightBuffer
{
    uint32_t count;
    uint32_t _padding[3];
    PointLight lights[1024];
};

struct LightComponent
{
    glm::vec3 colour = {1.0f, 1.0f, 1.0f};
    float intensity = 1.0f;
    float radius = 10.f;
};