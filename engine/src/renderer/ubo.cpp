#include "engine/ubo.h"
#include <glad/glad.h>

UniformBuffer ubo_Create(uint32_t size, uint32_t bindingPoint)
{
    UniformBuffer ubo;
    ubo.bindingPoint = bindingPoint;

    glGenBuffers(1, &ubo.ID);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo.ID);
    glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, ubo.ID);

    return ubo;
}

void ubo_BindToShader(const UniformBuffer& ubo, const Shader& shader, const std::string& blockName)
{
    uint32_t blockIndex = glGetUniformBlockIndex(shader.programID, blockName.c_str());
    if (blockIndex != GL_INVALID_INDEX)
    {
        glUniformBlockBinding(shader.programID, blockIndex, ubo.bindingPoint);
    }
}

void ubo_Update(const UniformBuffer& ubo, uint32_t offset, uint32_t size, const void* data)
{
    glBindBuffer(GL_UNIFORM_BUFFER, ubo.ID);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void ubo_Destroy(UniformBuffer& ubo)
{
    glDeleteBuffers(1, &ubo.ID);
    ubo.ID = 0;
}