#define TINYOBJLOADER_IMPLEMENTATION

#include <cmath>
#include <iostream>

#include <glm/vec3.hpp>

#include "mesh.h"
#include "render.h"

// ---------------------------------------------------------------------------
// Rendu sans texture (comportement existant inchangé)
// ---------------------------------------------------------------------------
void RenderHelper::renderModel(GLint modelUniform, GLint colorUniform, const glm::mat4 &modelMatrix, const Mesh &mesh, const glm::vec3 &color)
{
    glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniform3f(colorUniform, color.x, color.y, color.z);
    if (mesh.vao == 0 || mesh.vertexCount <= 0) {
        return;
    }

    glBindVertexArray(mesh.vao);
    glDrawArrays(GL_TRIANGLES, 0, mesh.vertexCount);
    glBindVertexArray(0);
}

// ---------------------------------------------------------------------------
// Rendu avec texture
// ---------------------------------------------------------------------------
void RenderHelper::renderModelTextured(
    GLint modelUniform, GLint colorUniform, GLint useTextureUniform, GLint textureSamplerUniform, const glm::mat4 &modelMatrix, const Mesh &mesh, const glm::vec3 &color)
{
    glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniform3f(colorUniform, color.x, color.y, color.z);

    if (mesh.vao == 0 || mesh.vertexCount <= 0) {
        return;
    }

    const bool hasTexture = (mesh.textureId != 0);
    glUniform1i(useTextureUniform, hasTexture ? 1 : 0);

    if (hasTexture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mesh.textureId);
        glUniform1i(textureSamplerUniform, 0);
    }

    glBindVertexArray(mesh.vao);
    glDrawArrays(GL_TRIANGLES, 0, mesh.vertexCount);
    glBindVertexArray(0);

    if (hasTexture) {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

// ---------------------------------------------------------------------------
// Chargement OBJ — VBO interleaved : position(4) + normal(4) + uv(2)
// Layout :  location 0 = position (vec4)
//           location 1 = normal   (vec4)
//           location 2 = texcoord (vec2)
// stride = 10 floats = 40 bytes
// ---------------------------------------------------------------------------
Mesh RenderHelper::loadObjMesh(const std::string &path)
{
    Mesh mesh;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    const bool loaded = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str(), nullptr, true);

    if (!warn.empty())
        std::cerr << "OBJ load warning: " << warn << std::endl;
    if (!err.empty())
        std::cerr << "OBJ load error: " << err << std::endl;
    if (!loaded) {
        std::cerr << "Failed to load OBJ file: " << path << std::endl;
        return mesh;
    }

    // Chaque float correspond à : px py pz pw  nx ny nz nw  u v
    std::vector<float> data;
    data.reserve(shapes.size() * 30);

    for (const auto &shape : shapes) {
        for (const auto &index : shape.mesh.indices) {

            // --- position (vec4, w=1) ---
            if (index.vertex_index >= 0) {
                const size_t b = static_cast<size_t>(index.vertex_index) * 3;
                data.push_back(attrib.vertices[b]);
                data.push_back(attrib.vertices[b + 1]);
                data.push_back(attrib.vertices[b + 2]);
                data.push_back(1.0f);
            } else {
                data.insert(data.end(), {0.f, 0.f, 0.f, 1.f});
            }

            // --- normale (vec4, w=0) ---
            if (index.normal_index >= 0) {
                const size_t b = static_cast<size_t>(index.normal_index) * 3;
                data.push_back(attrib.normals[b]);
                data.push_back(attrib.normals[b + 1]);
                data.push_back(attrib.normals[b + 2]);
                data.push_back(0.0f);
            } else {
                data.insert(data.end(), {0.f, 1.f, 0.f, 0.f});
            }

            // --- UV (vec2) ---
            if (index.texcoord_index >= 0) {
                const size_t b = static_cast<size_t>(index.texcoord_index) * 2;
                data.push_back(attrib.texcoords[b]);
                data.push_back(attrib.texcoords[b + 1]);
            } else {
                data.insert(data.end(), {0.f, 0.f});
            }
        }
    }

    if (data.empty()) {
        std::cerr << "No triangles parsed from OBJ file: " << path << std::endl;
        return mesh;
    }

    constexpr int STRIDE = 10; // floats per vertex
    const GLsizei strideBytes = STRIDE * static_cast<GLsizei>(sizeof(float));

    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vbo);

    glBindVertexArray(mesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);

    // location 0 — position vec4
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, strideBytes, reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(0);

    // location 1 — normal vec4
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, strideBytes, reinterpret_cast<void *>(4 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // location 2 — texcoord vec2
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, strideBytes, reinterpret_cast<void *>(8 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    mesh.vertexCount = static_cast<GLsizei>(data.size() / STRIDE);
    mesh.attrib = attrib;
    mesh.shapes = shapes;
    return mesh;
}

// ---------------------------------------------------------------------------
// Cylindre — on ajoute normales et UV pour rester cohérent
// ---------------------------------------------------------------------------
Mesh RenderHelper::createCylinderMesh(int segments)
{
    Mesh mesh;
    if (segments < 3)
        return mesh;

    const float radius = 0.5f;
    const float halfHeight = 0.5f;
    const float step = 2.0f * static_cast<float>(M_PI) / static_cast<float>(segments);

    // stride : px py pz pw  nx ny nz nw  u v  (10 floats)
    std::vector<float> data;
    data.reserve(static_cast<size_t>(segments) * 10 * 12);

    auto push = [&](float px, float py, float pz, float nx, float ny, float nz, float u, float v) { data.insert(data.end(), {px, py, pz, 1.f, nx, ny, nz, 0.f, u, v}); };

    for (int i = 0; i < segments; ++i) {
        const float a0 = step * static_cast<float>(i);
        const float a1 = step * static_cast<float>(i + 1);
        const float c0 = cosf(a0), s0 = sinf(a0);
        const float c1 = cosf(a1), s1 = sinf(a1);
        const float u0 = static_cast<float>(i) / static_cast<float>(segments);
        const float u1 = static_cast<float>(i + 1) / static_cast<float>(segments);

        // Côté
        push(radius * c0, radius * s0, -halfHeight, c0, s0, 0, u0, 0);
        push(radius * c0, radius * s0, halfHeight, c0, s0, 0, u0, 1);
        push(radius * c1, radius * s1, halfHeight, c1, s1, 0, u1, 1);

        push(radius * c0, radius * s0, -halfHeight, c0, s0, 0, u0, 0);
        push(radius * c1, radius * s1, halfHeight, c1, s1, 0, u1, 1);
        push(radius * c1, radius * s1, -halfHeight, c1, s1, 0, u1, 0);

        // Dessus
        push(0, 0, halfHeight, 0, 0, 1, 0.5f, 0.5f);
        push(radius * c0, radius * s0, halfHeight, 0, 0, 1, (c0 + 1) * .5f, (s0 + 1) * .5f);
        push(radius * c1, radius * s1, halfHeight, 0, 0, 1, (c1 + 1) * .5f, (s1 + 1) * .5f);

        // Dessous
        push(0, 0, -halfHeight, 0, 0, -1, 0.5f, 0.5f);
        push(radius * c1, radius * s1, -halfHeight, 0, 0, -1, (c1 + 1) * .5f, (s1 + 1) * .5f);
        push(radius * c0, radius * s0, -halfHeight, 0, 0, -1, (c0 + 1) * .5f, (s0 + 1) * .5f);
    }

    constexpr int STRIDE = 10;
    const GLsizei strideBytes = STRIDE * static_cast<GLsizei>(sizeof(float));

    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vbo);
    glBindVertexArray(mesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, strideBytes, reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, strideBytes, reinterpret_cast<void *>(4 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, strideBytes, reinterpret_cast<void *>(8 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    mesh.vertexCount = static_cast<GLsizei>(data.size() / STRIDE);
    return mesh;
}

// ---------------------------------------------------------------------------
// Texture procédurale désertique/rocheuse (Perlin-like via value noise)
// Génère une texture GPU et la stocke dans mesh.textureId
// ---------------------------------------------------------------------------
namespace {
    // Hash simple déterministe
    float hash(int x, int y)
    {
        int n = x + y * 57;
        n = (n << 13) ^ n;
        return 1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f;
    }

    // Interpolation cubique
    float smoothstep(float t) { return t * t * (3.0f - 2.0f * t); }

    float lerp(float a, float b, float t) { return a + t * (b - a); }

    // Value noise 2D
    float valueNoise(float x, float y)
    {
        const int ix = static_cast<int>(std::floor(x));
        const int iy = static_cast<int>(std::floor(y));
        const float fx = x - static_cast<float>(ix);
        const float fy = y - static_cast<float>(iy);
        const float ux = smoothstep(fx);
        const float uy = smoothstep(fy);

        return lerp(lerp(hash(ix, iy), hash(ix + 1, iy), ux), lerp(hash(ix, iy + 1), hash(ix + 1, iy + 1), ux), uy);
    }

    // Bruit fractal (fbm)
    float fbm(float x, float y, int octaves = 6)
    {
        float value = 0.0f;
        float amplitude = 0.5f;
        float frequency = 1.0f;
        for (int o = 0; o < octaves; ++o) {
            value += amplitude * valueNoise(x * frequency, y * frequency);
            amplitude *= 0.5f;
            frequency *= 2.0f;
        }
        return value;
    }

    struct RGB
    {
        unsigned char r, g, b;
    };

    // Palette désertique/rocheuse
    RGB desertColor(float n)
    {
        // n en [0,1]
        // Zones : sable clair → ocre → roche brune → roche sombre
        struct Stop
        {
            float t;
            RGB c;
        };
        const Stop palette[] = {
            {0.00f, {210, 180, 120}}, // sable clair
            {0.30f, {185, 140, 70}},  // ocre
            {0.55f, {150, 100, 50}},  // roche orange
            {0.75f, {110, 75, 40}},   // roche brune
            {1.00f, {70, 50, 30}},    // roche sombre
        };
        const int N = 5;
        if (n <= palette[0].t)
            return palette[0].c;
        if (n >= palette[N - 1].t)
            return palette[N - 1].c;
        for (int i = 0; i < N - 1; ++i) {
            if (n >= palette[i].t && n <= palette[i + 1].t) {
                const float t = (n - palette[i].t) / (palette[i + 1].t - palette[i].t);
                return {static_cast<unsigned char>(palette[i].c.r + t * (palette[i + 1].c.r - palette[i].c.r)),
                        static_cast<unsigned char>(palette[i].c.g + t * (palette[i + 1].c.g - palette[i].c.g)),
                        static_cast<unsigned char>(palette[i].c.b + t * (palette[i + 1].c.b - palette[i].c.b))};
            }
        }
        return palette[N - 1].c;
    }
}

void RenderHelper::generateDesertPlanetTexture(Mesh &mesh, int width, int height, unsigned int seed)
{
    std::vector<unsigned char> pixels(static_cast<size_t>(width * height * 3));

    const float offsetX = static_cast<float>(seed) * 3.7f;
    const float offsetY = static_cast<float>(seed) * 2.3f;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const float fx = (static_cast<float>(x) / static_cast<float>(width)) * 4.0f + offsetX;
            const float fy = (static_cast<float>(y) / static_cast<float>(height)) * 4.0f + offsetY;

            // Bruit principal
            float n = fbm(fx, fy, 6);

            // Crêtes rocheuses : on déforme avec un second bruit
            const float warpX = fbm(fx + 1.7f, fy + 9.2f, 3) * 0.4f;
            const float warpY = fbm(fx + 8.3f, fy + 2.8f, 3) * 0.4f;
            n = fbm(fx + warpX, fy + warpY, 5);

            // Normalisation approximative [0,1]
            n = (n + 1.0f) * 0.5f;
            n = std::fmax(0.0f, std::fmin(1.0f, n));

            const RGB c = desertColor(n);
            const size_t idx = static_cast<size_t>((y * width + x) * 3);
            pixels[idx] = c.r;
            pixels[idx + 1] = c.g;
            pixels[idx + 2] = c.b;
        }
    }

    GLuint texId = 0;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    mesh.textureId = texId;
}