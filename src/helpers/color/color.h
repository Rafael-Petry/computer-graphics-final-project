#ifndef COLOR_H
#define COLOR_H

#include <string>

#include <glm/vec3.hpp>

class ColorHelper
{
public:
    static glm::vec3 hexToRgb(const std::string &hex);
};

#endif
