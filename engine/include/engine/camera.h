#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "engine/engine.h"

struct App;

struct CameraMatrices
{
    glm::mat4 projection;
    glm::mat4 view;
    float nearPlane;
    float farPlane;
    float _padding[2];
};

struct Camera
{
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;

    float yaw;
    float pitch;

    float fov;
    float aspectRatio;
    float nearPlane;
    float farPlane;

    float lastX, lastY;
    bool firstMouse;
};

inline glm::mat4 camera_GetViewMatrix(const Camera& cam)
{
    return glm::lookAt(cam.position, cam.position + cam.front, cam.up);
}

inline glm::mat4 camera_GetProjectionMatrix(const Camera& cam)
{
    return glm::perspective(glm::radians(cam.fov), cam.aspectRatio, cam.nearPlane, cam.farPlane);
}

ENGINE_API void camera_ProcessInput(Camera& cam, App& app, float deltaTime);
ENGINE_API void camera_ProcessMouse(Camera& cam, float xpos, float ypos);