#include "engine/scene.h"
#include "engine/shader.h"
#include "engine/renderer.h"
#include <iostream>
#include <glad/glad.h>

Node* selectedNode = nullptr;

Scene scene_Create()
{
    Scene scene = {};

    scene.root = new Node();
    scene.root->name = "Root";
    scene.root->type = NodeType::Empty;
    scene.root->parent = nullptr;
    scene.root->position = {0, 0, 0};
    scene.root->rotation = {0, 0, 0};
    scene.root->scale = {1, 1, 1};
    scene.root->worldMatrix = glm::mat4(1.0f);

    return scene;
}

Node* scene_AddModel(Scene& scene, Node* parent, Model* model, const std::string& name)
{
    Node* newNode = new Node();
    newNode->name = name;
    newNode->type = NodeType::Model;
    newNode->model = model;
    newNode->parent = parent ? parent : scene.root;
    
    // Link to hierarchy
    newNode->parent->children.push_back(newNode);

    return newNode;
}
Node* scene_AddPointLight(Scene& scene, Node* parent, glm::vec3 colour, float intensity, float radius, const std::string& name)
{
    Node* newNode = new Node();
    newNode->name = name;
    newNode->type = NodeType::Light;
    
    newNode->position = {0, 0, 0};
    newNode->rotation = {0, 0, 0};
    newNode->scale = {1, 1, 1};
    newNode->worldMatrix = glm::mat4(1.0f);

    newNode->light.colour = colour;
    newNode->light.intensity = intensity;
    newNode->light.radius = radius;
    newNode->model = nullptr;

    newNode->parent = parent ? parent : scene.root;
    if (newNode->parent)
    {
        newNode->parent->children.push_back(newNode);
    }

    return newNode;
}

void scene_RenderNode(Node* node)
{
    if (node->type == NodeType::Model && node->model)
    {
        model_Draw(node->model, node->worldMatrix);
    }

    for(Node* child : node->children)
    {
        scene_RenderNode(child);
    }
}

void scene_RenderSelection(Node* node)
{
    if (node == selectedNode && node->type == NodeType::Model && node->model)
    {
        // 1. Setup Outline State
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT); // FIX: Cull the front, show only the back
        glFrontFace(GL_CCW);  // Standard winding
        
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        // 2. Setup Shader
        shader_Use(outlineShader);
        shader_SetVec3(outlineShader, "uColour", glm::vec3(1.0f, 0.8f, 0.0f));
        
        // 3. Draw Scaled Mesh
        // A scale of 1.03f usually looks cleaner for an inverted hull
        glm::mat4 outlineScale = glm::scale(node->worldMatrix, glm::vec3(1.01f));
        model_DrawWithShader(node->model, outlineScale, outlineShader);

        // 4. RESET STATE IMMEDIATELY
        glCullFace(GL_BACK); // Back to normal
    }

    // 2. Recursive call (now safe because state is restored above)
    for(Node* child : node->children)
    {
        scene_RenderSelection(child);
    }
}

void scene_Render(Scene& scene, RenderTarget& target, Colour& clearColour)
{
    glBindFramebuffer(GL_FRAMEBUFFER, target.fbo);
    glViewport(0, 0, target.width, target.height);

    glClearColor(
        clearColour.rgba.r,
        clearColour.rgba.g,
        clearColour.rgba.b,
        clearColour.rgba.a
    );
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    if (scene.root)
    {
        node_UpdateWorldMatrix(scene.root, glm::mat4(1.0f));

        renderer_UpdateLights(scene);

        scene_RenderNode(scene.root);
        scene_RenderSelection(scene.root);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void node_DestroyRecursive(Node* node)
{
    if (!node) return;

    for (Node* child : node->children)
    {
        node_DestroyRecursive(child);
    }

    if (node->type == NodeType::Model && node->model)
    {
        model_Destroy(node->model);
    }

    delete node;
}

void scene_Destroy(Scene& scene)
{
    node_DestroyRecursive(scene.root);
    scene.root = nullptr;
}

void node_UpdateWorldMatrix(Node* node, glm::mat4 parentTransform)
{
    glm::mat4 local = glm::translate(glm::mat4(1.0f), node->position);

    local = glm::rotate(local, glm::radians(node->rotation.x), {1, 0, 0});
    local = glm::rotate(local, glm::radians(node->rotation.y), {0, 1, 0});
    local = glm::rotate(local, glm::radians(node->rotation.z), {0, 0, 1});

    local = glm::scale(local, node->scale);

    node->worldMatrix = parentTransform * local;

    for (Node* child : node->children)
    {
        node_UpdateWorldMatrix(child, node->worldMatrix);
    }
}