#include "engine/shader.h"
#include "engine/utils/file.h"
#include <iostream>
#include <glad/glad.h>

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

static uint32_t shader_Compile(uint32_t type, const std::string& source)
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