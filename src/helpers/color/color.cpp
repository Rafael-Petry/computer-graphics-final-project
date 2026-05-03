#include <cctype>
#include "color.h"

float hexDigitToFloat(char c)
{
    if (c >= '0' && c <= '9') {
        return static_cast<float>(c - '0');
    }

    const char lower = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    if (lower >= 'a' && lower <= 'f') {
        return static_cast<float>(10 + (lower - 'a'));
    }

    return 0.0f;
}

glm::vec3 ColorHelper::hexToRgb(const std::string &hex)
{
    if (hex.size() != 7 || hex[0] != '#') {
        return glm::vec3(0.5f, 0.5f, 0.5f);
    }

    const float r = (hexDigitToFloat(hex[1]) * 16.0f + hexDigitToFloat(hex[2])) / 255.0f;
    const float g = (hexDigitToFloat(hex[3]) * 16.0f + hexDigitToFloat(hex[4])) / 255.0f;
    const float b = (hexDigitToFloat(hex[5]) * 16.0f + hexDigitToFloat(hex[6])) / 255.0f;

    return glm::vec3(r, g, b);
}
