#include "engine/model.h"
#include "engine/shader.h"
#include <glad/glad.h>

Model model_Load(const std::string& filePath)
{
    Model model = {};
    model.partCount = 0;

    return model;
}

Model model_Create(Vertex* vertices, uint32_t vertexCount, uint32_t* indices, uint32_t indexCount, Material material)
{
    Model model = {};

    Mesh mesh = mesh_Create(vertices, vertexCount, indices, indexCount);
    
    model.parts[0].mesh = mesh;
    model.parts[0].material = material;
    model.parts[0].position = glm::vec3(0.0f);
    model.parts[0].active = true;
    model.partCount = 1;

    return model;
}

void model_Draw(const Model& model)
{
    for (uint32_t i = 0; i < model.partCount; ++i)
    {
        const ModelPart& part = model.parts[i];
        if (!part.active) continue;

        if (part.material.shader)
        {
            shader_Use(*part.material.shader);

            glm::mat4 modelMatrix = glm::mat4(1.0f);

            modelMatrix = glm::translate(modelMatrix, model.position);

            modelMatrix = glm::translate(modelMatrix, part.position);

            shader_SetMat4(*part.material.shader, "model", modelMatrix);
        }

        glBindVertexArray(part.mesh.vertexArray);
        glDrawElements(GL_TRIANGLES, part.mesh.indexCount, GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);
}

void model_Destroy(Model& model)
{
    for (uint32_t i = 0; i < model.partCount; ++i)
    {
        mesh_Destroy(model.parts[i].mesh);
        material_Destroy(model.parts[i].material);
        model.parts[i].active = false;
    }
    
    model.partCount = 0;
}