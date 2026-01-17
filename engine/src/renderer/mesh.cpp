#include "engine/mesh.h"
#include <cstddef>
#include <glad/glad.h>

Mesh mesh_Create(Vertex* vertices, uint32_t vertexCount, uint32_t* indices, uint32_t indexCount)
{
    Mesh mesh = {};
    mesh.indexCount = indexCount;

    glGenVertexArrays(1, &mesh.vertexArray);
    glGenBuffers(1, &mesh.vertexBuffer);
    glGenBuffers(1, &mesh.elementBuffer);

    glBindVertexArray(mesh.vertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(uint32_t), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoords));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    return mesh;
}

void mesh_Destroy(Mesh& mesh)
{
    if (mesh.vertexArray) glDeleteVertexArrays(1, &mesh.vertexArray);
    if (mesh.vertexBuffer) glDeleteBuffers(1, &mesh.vertexBuffer);
    if (mesh.vertexBuffer) glDeleteBuffers(1, &mesh.elementBuffer);

    mesh = {};
}