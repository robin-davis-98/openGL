#pragma once
#include <cstdint>
#include "engine/shader.h"
#include "engine/camera.h"
#include "engine/engine.h"
#include "engine/model.h"
#include "engine/renderTarget.h"
#include "engine/light.h"
#include "engine/utils/colour.h"

#define MAX_SCENE_MODELS 32

enum class NodeType
{
    Empty,
    Model,
    Camera,
    Light
};

struct Node
{
    std::string name;
    NodeType type;

    Model* model;
    LightComponent light;

    Node* parent = nullptr;
    std::vector<Node*> children;

    glm::vec3 position = {0, 0, 0};
    glm::vec3 rotation = {0, 0, 0};
    glm::vec3 scale = {1, 1, 1};

    glm::mat4 worldMatrix = glm::mat4(1.0f);
};

struct ENGINE_API Scene
{
    Node* root;

    std::vector<Model> models;
    std::vector<Camera> cameras;

    Camera* currentCamera;
};

extern struct Node* selectedNode;
extern struct Shader outlineShader;

ENGINE_API Scene scene_Create();

ENGINE_API Node* scene_AddModel(Scene& scene, Node* parent, Model* model, const std::string& name);
ENGINE_API Node* scene_AddPointLight(Scene& scene, Node* parent, glm::vec3 colour, float intensity, float radius, const std::string& name = "Point Light");
ENGINE_API void scene_Render(Scene& scene, RenderTarget& target, Colour& clearColour);
ENGINE_API void scene_Destroy(Scene& scene);
void scene_RenderNode(Node* node);
void scene_RenderSelection(Node* node);
void node_UpdateWorldMatrix(Node* node, glm::mat4 parentTransform);