#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include <string>

struct Shader
{
    uint32_t programID = 0;
};

Shader shader_Create(const std::string& vertexPath, const std::string& fragmentPath);

void shader_Destroy(Shader& shader);
void shader_Use(const Shader& shader);
uint32_t shader_Compile(uint32_t type, const std::string& source);