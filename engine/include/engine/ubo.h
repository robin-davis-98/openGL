#pragma once
#include <cstdint>
#include <string>
#include "engine/shader.h"

struct UniformBuffer
{
    uint32_t ID;
    uint32_t bindingPoint;
};

ENGINE_API UniformBuffer ubo_Create(uint32_t size, uint32_t bindingPoint);
ENGINE_API void ubo_BindToShader(const UniformBuffer& ubo, const Shader& shader, const std::string& blockName);
ENGINE_API void ubo_Update(const UniformBuffer& ubo, uint32_t offset, uint32_t size, const void* data);
ENGINE_API void ubo_Destroy(UniformBuffer& ubo);