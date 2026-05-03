#include <string>

#include <glad/glad.h>
#include <glm/vec3.hpp>

#include "celestialBody.h"
#include "../../vendor/include/matrices.h"

CelestialBody::CelestialBody(const std::string &meshPath, const glm::vec3 &color) : Object(meshPath, color) {}
