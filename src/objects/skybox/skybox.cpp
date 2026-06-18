#include <cmath>
#include <random>
#include <vector>

#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "skybox.h"
#include "../spaceship/spaceship.h"
#include "../../helpers/render/render.h"
#include "../../window/window.h"
#include "../../vendor/include/matrices.h"

Mesh Skybox::mesh;
bool Skybox::textureGenerated = false;

Skybox::Skybox()
{
    if (mesh.vao == 0) {
        // Crée une sphère procédurale inversée (faces vers l'intérieur)
        const int stacks = 32;
        const int slices = 64;
        const float radius = 900.0f; // grande sphère autour de la scène

        std::vector<float> data;
        data.reserve(stacks * slices * 6 * 10);

        auto pushVertex = [&](float theta, float phi) {
            // Position sur la sphère
            float x = radius * sinf(phi) * cosf(theta);
            float y = radius * cosf(phi);
            float z = radius * sinf(phi) * sinf(theta);

            // Normale vers l'intérieur
            float nx = -sinf(phi) * cosf(theta);
            float ny = -cosf(phi);
            float nz = -sinf(phi) * sinf(theta);

            // UV sphérique
            float u = theta / (2.0f * static_cast<float>(M_PI));
            float v = phi / static_cast<float>(M_PI);

            data.insert(data.end(), {x, y, z, 1.f, nx, ny, nz, 0.f, u, v});
        };

        for (int i = 0; i < stacks; ++i) {
            for (int j = 0; j < slices; ++j) {
                const float phi0 = static_cast<float>(M_PI) * static_cast<float>(i) / static_cast<float>(stacks);
                const float phi1 = static_cast<float>(M_PI) * static_cast<float>(i + 1) / static_cast<float>(stacks);
                const float theta0 = 2.0f * static_cast<float>(M_PI) * static_cast<float>(j) / static_cast<float>(slices);
                const float theta1 = 2.0f * static_cast<float>(M_PI) * static_cast<float>(j + 1) / static_cast<float>(slices);

                // Triangle 1
                pushVertex(theta0, phi0);
                pushVertex(theta1, phi0);
                pushVertex(theta0, phi1);

                // Triangle 2
                pushVertex(theta1, phi0);
                pushVertex(theta1, phi1);
                pushVertex(theta0, phi1);
            }
        }

        constexpr int STRIDE = 10;
        const GLsizei strideBytes = STRIDE * static_cast<GLsizei>(sizeof(float));

        glGenVertexArrays(1, &mesh.vao);
        glGenBuffers(1, &mesh.vbo);
        glBindVertexArray(mesh.vao);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(data.size() * sizeof(float)), data.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, strideBytes, reinterpret_cast<void *>(0));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, strideBytes, reinterpret_cast<void *>(4 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, strideBytes, reinterpret_cast<void *>(8 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
        mesh.vertexCount = static_cast<GLsizei>(data.size() / STRIDE);
    }

    if (!textureGenerated) {
        generateStarTexture(2048, 1024, 42);
        textureGenerated = true;
    }
}

Skybox &Skybox::getInstance()
{
    static Skybox instance;
    return instance;
}

void Skybox::generateStarTexture(int width, int height, unsigned int seed)
{
    std::vector<unsigned char> pixels(static_cast<size_t>(width * height * 3), 0);

    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> xDist(0, width - 1);
    std::uniform_int_distribution<int> yDist(0, height - 1);
    std::uniform_real_distribution<float> brightnessDist(0.5f, 1.0f);
    std::uniform_real_distribution<float> sizeDist(0.0f, 1.0f);

    // Nombre d'étoiles
    const int starCount = 2000;

    for (int s = 0; s < starCount; ++s) {
        const int cx = xDist(rng);
        const int cy = yDist(rng);
        const float brightness = brightnessDist(rng);
        const bool isBig = sizeDist(rng) < 0.1f; // 10% d'étoiles plus grandes

        // Couleur légèrement teintée (blanc chaud, blanc froid, blanc pur)
        std::uniform_int_distribution<int> tintDist(0, 2);
        const int tint = tintDist(rng);
        unsigned char r, g, b;
        const unsigned char val = static_cast<unsigned char>(brightness * 255.0f);
        if (tint == 0) {
            r = val;
            g = val;
            b = val;
        } // blanc pur
        else if (tint == 1) {
            r = val;
            g = static_cast<unsigned char>(val * 0.9f);
            b = static_cast<unsigned char>(val * 0.7f);
        } // chaud
        else {
            r = static_cast<unsigned char>(val * 0.8f);
            g = static_cast<unsigned char>(val * 0.9f);
            b = val;
        } // froid

        // Pixel central
        auto setPixel = [&](int px, int py, float factor) {
            if (px < 0 || px >= width || py < 0 || py >= height)
                return;
            const size_t idx = static_cast<size_t>((py * width + px) * 3);
            pixels[idx] = static_cast<unsigned char>(std::min(255.0f, r * factor));
            pixels[idx + 1] = static_cast<unsigned char>(std::min(255.0f, g * factor));
            pixels[idx + 2] = static_cast<unsigned char>(std::min(255.0f, b * factor));
        };

        setPixel(cx, cy, 1.0f);

        if (isBig) {
            // Halo doux autour des étoiles brillantes
            setPixel(cx - 1, cy, 0.4f);
            setPixel(cx + 1, cy, 0.4f);
            setPixel(cx, cy - 1, 0.4f);
            setPixel(cx, cy + 1, 0.4f);
            setPixel(cx - 1, cy - 1, 0.15f);
            setPixel(cx + 1, cy - 1, 0.15f);
            setPixel(cx - 1, cy + 1, 0.15f);
            setPixel(cx + 1, cy + 1, 0.15f);
        }
    }

    GLuint texId = 0;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    mesh.textureId = texId;
}

void Skybox::update(GLint modelUniform, GLint colorUniform, GLint useTextureUniform, GLint texSamplerUniform, GLint isEmissiveUniform, Window *window)
{
    if (mesh.vao == 0 || mesh.vertexCount <= 0)
        return;

    // La skybox est centrée sur la caméra (suit le joueur)
    const glm::vec3 camPos = Spaceship::getInstance().getCameraPosition();
    const glm::mat4 model = Matrix_Translate(camPos.x, camPos.y, camPos.z);

    glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));
    glUniform3f(colorUniform, 1.0f, 1.0f, 1.0f);

    // Émissif : pas de shading, texture pure
    if (isEmissiveUniform >= 0)
        glUniform1i(isEmissiveUniform, 1);

    // Texture d'étoiles
    if (useTextureUniform >= 0)
        glUniform1i(useTextureUniform, 1);
    if (texSamplerUniform >= 0)
        glUniform1i(texSamplerUniform, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mesh.textureId);

    // Désactive le depth write pour que la skybox soit toujours derrière
    glDepthMask(GL_FALSE);

    glBindVertexArray(mesh.vao);
    glDrawArrays(GL_TRIANGLES, 0, mesh.vertexCount);
    glBindVertexArray(0);

    glDepthMask(GL_TRUE);

    glBindTexture(GL_TEXTURE_2D, 0);
    if (useTextureUniform >= 0)
        glUniform1i(useTextureUniform, 0);
    if (isEmissiveUniform >= 0)
        glUniform1i(isEmissiveUniform, 0);
}