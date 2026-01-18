#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include "engine/engine.h"

struct ENGINE_API Shader
{
    uint32_t programID = 0;
};

ENGINE_API Shader shader_Create(const std::string& vertexPath, const std::string& fragmentPath);
ENGINE_API void shader_SetMat4(const Shader& shader, const std::string& name, const glm::mat4& value);
ENGINE_API void shader_SetVec3(const Shader& shader, const std::string& name, const glm::vec3& value);

ENGINE_API Shader shader_CreateCompute(const std::string& computePath);
ENGINE_API void shader_Dispatch(uint32_t x, uint32_t y, uint32_t z);

void shader_Destroy(Shader& shader);
void shader_Use(const Shader& shader);
uint32_t shader_Compile(uint32_t type, const std::string& source);
