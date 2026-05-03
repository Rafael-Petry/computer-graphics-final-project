#include <string>

#include <glad/glad.h>
#include <glm/vec3.hpp>

#include "celestialBody.h"

const std::string CelestialBody::meshPath = "../../src/objects/celestialBody/celestialBody.obj";
const std::string CelestialBody::color = "#808080";

const std::string &CelestialBody::getColor() const { return color; }

void CelestialBody::update(GLint modelUniform, GLint colorUniform, const glm::mat4 &modelMatrix) const
{
    const glm::vec3 colorRgb = ColorHelper::hexToRgb(getColor());
    RenderHelper::renderModel(modelUniform, colorUniform, modelMatrix, getMeshPath(), colorRgb);
}

const std::string &CelestialBody::getMeshPath() { return meshPath; }
