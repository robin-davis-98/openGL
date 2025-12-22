#pragma once
#include "colour.h"
#include <glm/glm.hpp>
#include <string>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <cmath>

constexpr Colour colour_NormalizedRGBA(float r, float g, float b, float a = 1.0f)
{
    return Colour{ glm::vec4(r,g,b,a) };
}

constexpr Colour colour_RGBA(float r, float g, float b, float a = 255)
{
    return Colour{ glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f) };
}

inline Colour ColourFromHex(const std::string& hex)
{
    unsigned int r = 0, g = 0, b = 0;
    if (hex[0] == '#')
    {
        std::stringstream ss;
        ss << std::hex << hex.substr(1,2);
        ss >> r;
        ss.clear();
        ss << std::hex << hex.substr(3, 2);
        ss >> g;
        ss.clear();
        ss << std::hex << hex.substr(5, 2);
        ss >> b;

        return colour_RGBA(r, g, b);
    }

    if (hex.length() == 6) 
    {
        std::stringstream ss;
        ss << std::hex << hex.substr(0,2);
        ss >> r;
        ss.clear();
        ss << std::hex << hex.substr(2,2);
        ss >> g;
        ss.clear();
        ss << std::hex << hex.substr(4,2);
        ss >> b;

        return colour_RGBA(r, g, b);
    }

    return colour_RGBA(255, 71, 234);
}