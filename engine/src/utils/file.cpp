#include "engine/utils/file.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

std::string read_file(const std::string& path)
{
    std::filesystem::path exeDir = std::filesystem::current_path();
    std::filesystem::path absolutePath = exeDir / path;

    std::ifstream file(absolutePath);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << absolutePath << std::endl;
        return {};
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}