// Helper for color conversions.
#include "color.h"

#include <cctype>

namespace
{
float HexDigitToFloat(char c)
{
	if (c >= '0' && c <= '9')
	{
		return static_cast<float>(c - '0');
	}

	const char lower = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
	if (lower >= 'a' && lower <= 'f')
	{
		return static_cast<float>(10 + (lower - 'a'));
	}

	return 0.0f;
}
} // namespace

glm::vec3 ColorHelper::HexToRgb(const std::string& hex)
{
	if (hex.size() != 7 || hex[0] != '#')
	{
		return glm::vec3(0.5f, 0.5f, 0.5f);
	}

	const float r = (HexDigitToFloat(hex[1]) * 16.0f + HexDigitToFloat(hex[2])) / 255.0f;
	const float g = (HexDigitToFloat(hex[3]) * 16.0f + HexDigitToFloat(hex[4])) / 255.0f;
	const float b = (HexDigitToFloat(hex[5]) * 16.0f + HexDigitToFloat(hex[6])) / 255.0f;

	return glm::vec3(r, g, b);
}
