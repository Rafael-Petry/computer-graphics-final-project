#include "celestialBody.h"

#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

#include <glm/gtc/type_ptr.hpp>

#include <glm/vec3.hpp>

namespace
{
struct MeshData
{
	GLuint vao = 0;
	GLuint vbo = 0;
	GLsizei vertexCount = 0;
};

unsigned ParseObjIndexToken(const std::string& token, size_t vertexCount)


{
	const size_t slashPos = token.find('/');
	const std::string indexToken = slashPos == std::string::npos ? token : token.substr(0, slashPos);
	int index = std::stoi(indexToken);
	if (index < 0)
	{
		index = static_cast<int>(vertexCount) + index + 1;
	}

	return static_cast<unsigned>(index - 1);
}

MeshData LoadObjMesh(const std::string& path)


{
	MeshData mesh;

	std::ifstream file(path);
	if (!file.is_open())
	{
		std::cerr << "Could not open OBJ file: " << path << std::endl;
		return mesh;
	}

	std::vector<glm::vec3> vertices;
	std::vector<float> trianglePositions;

	std::string line;
	while (std::getline(file, line))
	{
		if (line.size() < 2)
		{
			continue;
		}

		std::stringstream lineStream(line);
		std::string prefix;
		lineStream >> prefix;

		if (prefix == "v")
		{
			glm::vec3 position(0.0f, 0.0f, 0.0f);
			lineStream >> position.x >> position.y >> position.z;
			vertices.push_back(position);
		}
		else if (prefix == "f")
		{
			std::vector<unsigned> faceIndices;
			std::string token;
			while (lineStream >> token)
			{
				faceIndices.push_back(ParseObjIndexToken(token, vertices.size()));
			}

			for (size_t i = 1; i + 1 < faceIndices.size(); ++i)
			{
				const glm::vec3& a = vertices[faceIndices[0]];
				const glm::vec3& b = vertices[faceIndices[i]];
				const glm::vec3& c = vertices[faceIndices[i + 1]];

				trianglePositions.push_back(a.x);
				trianglePositions.push_back(a.y);
				trianglePositions.push_back(a.z);
				trianglePositions.push_back(b.x);
				trianglePositions.push_back(b.y);
				trianglePositions.push_back(b.z);
				trianglePositions.push_back(c.x);
				trianglePositions.push_back(c.y);
				trianglePositions.push_back(c.z);
			}
		}
	}

	if (trianglePositions.empty())
	{
		std::cerr << "No triangles parsed from OBJ file: " << path << std::endl;
		return mesh;
	}

	glGenVertexArrays(1, &mesh.vao);
	glGenBuffers(1, &mesh.vbo);

	glBindVertexArray(mesh.vao);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
	glBufferData(GL_ARRAY_BUFFER, trianglePositions.size() * sizeof(float), trianglePositions.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * static_cast<GLsizei>(sizeof(float)), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	mesh.vertexCount = static_cast<GLsizei>(trianglePositions.size() / 3);
	return mesh;
}

const MeshData& GetMesh(const std::string& path)


{
	static std::unordered_map<std::string, MeshData> cache;
	const auto it = cache.find(path);
	if (it != cache.end())
	{
		return it->second;
	}

	const MeshData mesh = LoadObjMesh(path);
	const auto inserted = cache.emplace(path, mesh);
	return inserted.first->second;

}

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

	glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniform3f(colorUniform, r, g, b);

	const MeshData& mesh = GetMesh(getModel());
	if (mesh.vao == 0 || mesh.vertexCount <= 0)
	{
		return;
	}

	glBindVertexArray(mesh.vao);
	glDrawArrays(GL_TRIANGLES, 0, mesh.vertexCount);
	glBindVertexArray(0);
}

const std::string& CelestialBody::getModel()
{
	return model;
}

