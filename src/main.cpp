#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shaders/loader/loader.h"
#include "window/window.h"
#include "objects/spaceship/spaceship.h"

int main()
{
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return -1;
    }

    if (!Window::getInstance().initialize(1280, 720, "Asteroids 3D")) {
        return -1;
    }

    const GLuint shaderProgram = ShaderLoader::createShaderProgram("../../src/shaders/vertex.glsl", "../../src/shaders/fragment.glsl");
    glUseProgram(shaderProgram);
    Window::getInstance().setShaderProgram(shaderProgram);

    // ── Soleil : source de lumière ───────────────────────────────────────────
    glUniform3f(glGetUniformLocation(shaderProgram, "sunColor"), 1.0f, 0.95f, 0.8f);
    glUniform1f(glGetUniformLocation(shaderProgram, "sunIntensity"), 3.0f);

    // ── Lumière ambiante (espace) ────────────────────────────────────────────
    glUniform3f(glGetUniformLocation(shaderProgram, "ambientColor"), 0.02f, 0.02f, 0.05f);

    // ── Paramètres PBR Disney (valeurs par défaut pour tous les objets) ──────
    // Ces valeurs peuvent être surchargées par objet si besoin plus tard.
    glUniform1f(glGetUniformLocation(shaderProgram, "metallic"), 0.6f);
    glUniform1f(glGetUniformLocation(shaderProgram, "roughness"), 0.3f);
    glUniform1f(glGetUniformLocation(shaderProgram, "subsurface"), 0.0f);
    glUniform1f(glGetUniformLocation(shaderProgram, "specular"), 0.8f);
    glUniform1f(glGetUniformLocation(shaderProgram, "specularTint"), 0.2f);
    glUniform1f(glGetUniformLocation(shaderProgram, "anisotropic"), 0.0f);
    glUniform1f(glGetUniformLocation(shaderProgram, "sheen"), 0.0f);
    glUniform1f(glGetUniformLocation(shaderProgram, "sheenTint"), 0.5f);
    glUniform1f(glGetUniformLocation(shaderProgram, "clearcoat"), 0.5f);
    glUniform1f(glGetUniformLocation(shaderProgram, "clearcoatGloss"), 0.8f);

    // ── Texture : désactivée par défaut (activée par renderModelTextured) ────
    glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "textureColormap"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "isEmissive"), 0);

    while (!glfwWindowShouldClose(Window::getInstance().getGlfwWindow())) {
        Window::getInstance().update();
    }

    std::cout << "Final Score: " << Spaceship::getInstance().getScore() << std::endl;

    Window::getInstance().close();
    glDeleteProgram(shaderProgram);
    glfwTerminate();

    return 0;
}
