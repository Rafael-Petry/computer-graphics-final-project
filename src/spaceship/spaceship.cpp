#include "spaceship.h"

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
} // namespace

const std::string Spaceship::model = "src/spaceship/spaceship.obj";

void Spaceship::render(GLint modelUniform, GLint colorUniform, const glm::mat4& modelMatrix) const
{
	glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniform3f(colorUniform, 0.73f, 0.79f, 0.88f);

	const MeshData& mesh = GetMesh(getModel());
	if (mesh.vao == 0 || mesh.vertexCount <= 0)
	{
		return;
	}

	glBindVertexArray(mesh.vao);
	glDrawArrays(GL_TRIANGLES, 0, mesh.vertexCount);
	glBindVertexArray(0);
}

void Spaceship::translate() const
{
	std::cout << "Moving the spaceship..." << std::endl;
}

void Spaceship::rotate() const
{
	std::cout << "Rotating the spaceship" << std::endl;
}

void Spaceship::shoot() const
{
	std::cout << "The spaceship is shooting..." << std::endl;
}

const std::string& Spaceship::getModel()
{
	return model;
}
