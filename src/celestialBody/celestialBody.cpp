#include "celestialBody.h"

#include <glm/vec3.hpp>

#include "../helpers/color/color.h"
#include "../helpers/render/render.h"

const std::string CelestialBody::model = "../../src/celestialBody/celestialBody.obj";
const std::string CelestialBody::color = "#808080";

const std::string& CelestialBody::getColor() const
{
	return color;
}

void CelestialBody::render(GLint modelUniform, GLint colorUniform, const glm::mat4& modelMatrix) const
{
	const glm::vec3 colorRgb = ColorHelper::HexToRgb(getColor());
	RenderHelper::RenderModel(modelUniform, colorUniform, modelMatrix, getModel(), colorRgb);
}

const std::string& CelestialBody::getModel()
{
	return model;
}

