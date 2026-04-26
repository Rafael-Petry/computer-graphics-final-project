#include "spaceship.h"

#include <iostream>

#include <glm/gtc/type_ptr.hpp>

namespace
{
GLuint GetCubeVao()
{
	static GLuint vao = 0;
	static GLuint vbo = 0;

	if (vao != 0)
	{
		return vao;
	}

	const float vertices[] = {
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * static_cast<GLsizei>(sizeof(float)), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	return vao;
}
} // namespace

const std::string Spaceship::model = "src/spaceship/spaceship.obj";

void Spaceship::render(GLint modelUniform, GLint colorUniform, const glm::mat4& modelMatrix) const
{
	glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniform3f(colorUniform, 0.73f, 0.79f, 0.88f);

	const GLuint vao = GetCubeVao();
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);
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
