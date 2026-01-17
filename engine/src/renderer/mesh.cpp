#include "engine/mesh.h"
#include <glad/glad.h>

Mesh mesh_Create(Vertex* vertices, uint32_t vertexCount)
{
    Mesh mesh = {};

    glGenVertexArrays(1, &mesh.vertexArray);
    glGenBuffers(1, &mesh.vertexBuffer);

    glBindVertexArray(mesh.vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vertexBuffer);

    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    mesh.vertexCount = vertexCount;

    glBindVertexArray(0);
    return mesh;
}

void mesh_Destroy(Mesh& mesh)
{
    if (mesh.vertexArray) glDeleteVertexArrays(1, &mesh.vertexArray);
    if (mesh.vertexBuffer) glDeleteBuffers(1, &mesh.vertexBuffer);

    mesh = {};
}