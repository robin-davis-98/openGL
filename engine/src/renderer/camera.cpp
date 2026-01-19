#include "engine/app.h"
#include "engine/camera.h"

void camera_ProcessInput(Camera& cam, App& app, float deltaTime)
{
    float speed = 5.0f * deltaTime;

    glm::vec3 right = glm::normalize(glm::cross(cam.front, cam.up));

    if (glfwGetKey(app.window.nativeHandle, GLFW_KEY_W) == GLFW_PRESS)
        cam.position += speed * cam.front;
    if (glfwGetKey(app.window.nativeHandle, GLFW_KEY_S) == GLFW_PRESS)
        cam.position -= speed * cam.front;

    if (glfwGetKey(app.window.nativeHandle, GLFW_KEY_A) == GLFW_PRESS)
        cam.position -= speed * right;
    if (glfwGetKey(app.window.nativeHandle, GLFW_KEY_D) == GLFW_PRESS)
        cam.position += speed * right;

    if (glfwGetKey(app.window.nativeHandle, GLFW_KEY_SPACE) == GLFW_PRESS)
        cam.position += speed * cam.up;
    if (glfwGetKey(app.window.nativeHandle, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        cam.position -= speed * cam.up;


    float changeSpeed = 0.5f; // Adjust how fast health moves (50% per second)

    if (glfwGetKey(app.window.nativeHandle, GLFW_KEY_EQUAL) == GLFW_PRESS) // The + key
    {
        playerHealth = glm::clamp(playerHealth + (changeSpeed * deltaTime), 0.0f, 1.0f);
    }

    if (glfwGetKey(app.window.nativeHandle, GLFW_KEY_MINUS) == GLFW_PRESS) // The - key
    {
        playerHealth = glm::clamp(playerHealth - (changeSpeed * deltaTime), 0.0f, 1.0f);
    }
}

void camera_ProcessMouse(Camera& cam, float xpos, float ypos)
{
    if (cam.firstMouse)
    {
        cam.lastX = xpos;
        cam.lastY = ypos;
        cam.firstMouse = false;
    }

    float xoffset = xpos - cam.lastX;
    float yoffset = cam.lastY - ypos;
    cam.lastX = xpos;
    cam.lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    cam.yaw += xoffset;
    cam.pitch += yoffset;

    if (cam.pitch > 89.0f) cam.pitch = 89.0f;
    if (cam.pitch < -89.0f) cam.pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(cam.yaw)) * cos(glm::radians(cam.pitch));
    direction.y = sin(glm::radians(cam.pitch));
    direction.z = sin(glm::radians(cam.yaw)) * cos(glm::radians(cam.pitch));
    cam.front = glm::normalize(direction);
}