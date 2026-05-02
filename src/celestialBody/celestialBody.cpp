#include "celestialBody.h"

#include <cctype>

#include <glm/vec3.hpp>

#include "../helpers/render.h"

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

void HexToRgb(const std::string& hex, float& r, float& g, float& b)
{
	if (hex.size() != 7 || hex[0] != '#')
	{
		r = 0.5f;
		g = 0.5f;
		b = 0.5f;
		return;
	}

	r = (HexDigitToFloat(hex[1]) * 16.0f + HexDigitToFloat(hex[2])) / 255.0f;
	g = (HexDigitToFloat(hex[3]) * 16.0f + HexDigitToFloat(hex[4])) / 255.0f;
	b = (HexDigitToFloat(hex[5]) * 16.0f + HexDigitToFloat(hex[6])) / 255.0f;
}
} // namespace

const std::string CelestialBody::model = "../../src/celestialBody/celestialBody.obj";
const std::string CelestialBody::color = "#808080";

const std::string& CelestialBody::getColor() const
{
	return color;
}

void CelestialBody::render(GLint modelUniform, GLint colorUniform, const glm::mat4& modelMatrix) const
{
	float r = 0.5f;
	float g = 0.5f;
	float b = 0.5f;
	HexToRgb(getColor(), r, g, b);

	RenderHelper::RenderModel(
		modelUniform,
		colorUniform,
		modelMatrix,
		getModel(),
		glm::vec3(r, g, b));
}

const std::string& CelestialBody::getModel()
{
	return model;
}

