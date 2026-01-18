#include "engine/shader.h"
#include "engine/utils/file.h"
#include <iostream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

Shader shader_Create(const std::string& vertexPath, const std::string& fragmentPath)
{
    Shader shader{};

    std::string vertexSource = read_file(vertexPath);
    std::string fragmentSource = read_file(fragmentPath);

    uint32_t vertexShader = shader_Compile(GL_VERTEX_SHADER, vertexSource);
    uint32_t fragmentShader = shader_Compile(GL_FRAGMENT_SHADER, fragmentSource);

    shader.programID = glCreateProgram();
    glAttachShader(shader.programID, vertexShader);
    glAttachShader(shader.programID, fragmentShader);
    glLinkProgram(shader.programID);

    int success;
    glGetProgramiv(shader.programID, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(shader.programID, 512, nullptr, infoLog);
        std::cerr << "Shader link error:\n" << infoLog << std::endl;

        glDeleteProgram(shader.programID);
        shader.programID = 0;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shader;
}

Shader shader_CreateCompute(const std::string& computePath)
{
    Shader shader{};

    std::string computeSource = read_file(computePath);
    uint32_t computeShader = shader_Compile(GL_COMPUTE_SHADER, computeSource);

    shader.programID = glCreateProgram();
    glAttachShader(shader.programID, computeShader);
    glLinkProgram(shader.programID);

    int success;
    glGetProgramiv(shader.programID, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(shader.programID, 512, nullptr, infoLog);
        std::cerr << "Compute Shader link error:\n" << infoLog << std::endl;
        glDeleteProgram(shader.programID);
        shader.programID = 0;
    }

    glDeleteShader(computeShader);

    return shader;
}

void shader_Dispatch(uint32_t x, uint32_t y, uint32_t z)
{
    glDispatchCompute(x, y, z);
    // Ensure all memory writes to SSBOs are finished before the next shader uses them
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void shader_Destroy(Shader& shader)
{
    if (shader.programID)
    {
        glDeleteProgram(shader.programID);
        shader.programID = 0;
    }
}

void shader_Use(const Shader& shader)
{
    glUseProgram(shader.programID);
}

uint32_t shader_Compile(uint32_t type, const std::string& source)
{
    uint32_t shader = glCreateShader(type);
    const char* src = source.c_str();

    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return shader;
}

void shader_SetInt(const Shader& shader, const std::string& name, int value)
{
    shader_Use(shader);

    glUniform1i(glGetUniformLocation(shader.programID, name.c_str()), value);
}

void shader_SetBool(const Shader& shader, const std::string& name, bool value)
{
    shader_Use(shader);

    glUniform1i(glGetUniformLocation(shader.programID, name.c_str()), (int) value);
}

void shader_SetFloat(const Shader& shader, const std::string& name, float value)
{
    shader_Use(shader);
    
    glUniform1f(glGetUniformLocation(shader.programID, name.c_str()), value);
}

void shader_SetVec2(const Shader& shader, const std::string& name, const glm::vec2& value)
{
    shader_Use(shader);
    
    glUniform2fv(glGetUniformLocation(shader.programID, name.c_str()), 1, glm::value_ptr(value));
}

void shader_SetVec3(const Shader& shader, const std::string& name, const glm::vec3& value)
{
    shader_Use(shader);
    
    glUniform3fv(glGetUniformLocation(shader.programID, name.c_str()), 1, glm::value_ptr(value));
}

void shader_SetVec4(const Shader& shader, const std::string& name, const glm::vec4& value)
{
    shader_Use(shader);
    
    glUniform4fv(glGetUniformLocation(shader.programID, name.c_str()), 1, glm::value_ptr(value));
}

void shader_SetMat4(const Shader& shader, const std::string& name, const glm::mat4& value)
{
    shader_Use(shader);
    
    glUniformMatrix4fv(glGetUniformLocation(shader.programID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}