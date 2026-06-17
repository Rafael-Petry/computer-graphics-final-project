#define TINYOBJLOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include <cmath>
#include <iostream>
#include <unordered_map>

#include <glm/vec3.hpp>

#include "mesh.h"
#include "render.h"
#include "../../vendor/include/stb_image.h"

// ---------------------------------------------------------------------------
// Helpers internes
// ---------------------------------------------------------------------------
namespace {
    constexpr int STRIDE = 10; // floats par vertex : pos(4)+normal(4)+uv(2)
    constexpr int STRIDE_BYTES = STRIDE * static_cast<int>(sizeof(float));

    // Crée un VAO/VBO à partir d'un buffer de floats interleaved
    void buildVAO(GLuint &vao, GLuint &vbo, const std::vector<float> &data)
    {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(data.size() * sizeof(float)), data.data(), GL_STATIC_DRAW);

        // location 0 — position vec4
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, STRIDE_BYTES, reinterpret_cast<void *>(0));
        glEnableVertexAttribArray(0);
        // location 1 — normal vec4
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, STRIDE_BYTES, reinterpret_cast<void *>(4 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // location 2 — texcoord vec2
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, STRIDE_BYTES, reinterpret_cast<void *>(8 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
    }

    // Ajoute un vertex dans le buffer
    void pushVertex(std::vector<float> &data, const tinyobj::attrib_t &attrib, const tinyobj::index_t &index)
    {
        if (index.vertex_index >= 0) {
            const size_t b = static_cast<size_t>(index.vertex_index) * 3;
            data.push_back(attrib.vertices[b]);
            data.push_back(attrib.vertices[b + 1]);
            data.push_back(attrib.vertices[b + 2]);
            data.push_back(1.0f);
        } else {
            data.insert(data.end(), {0.f, 0.f, 0.f, 1.f});
        }

        if (index.normal_index >= 0) {
            const size_t b = static_cast<size_t>(index.normal_index) * 3;
            data.push_back(attrib.normals[b]);
            data.push_back(attrib.normals[b + 1]);
            data.push_back(attrib.normals[b + 2]);
            data.push_back(0.0f);
        } else {
            data.insert(data.end(), {0.f, 1.f, 0.f, 0.f});
        }

        if (index.texcoord_index >= 0) {
            const size_t b = static_cast<size_t>(index.texcoord_index) * 2;
            data.push_back(attrib.texcoords[b]);
            data.push_back(1.0f - attrib.texcoords[b + 1]); // flip Y OpenGL
        } else {
            data.insert(data.end(), {0.f, 0.f});
        }
    }

    // Hash simple
    float hash(int x, int y)
    {
        int n = x + y * 57;
        n = (n << 13) ^ n;
        return 1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f;
    }
    float smoothstep(float t) { return t * t * (3.0f - 2.0f * t); }
    float lerp(float a, float b, float t) { return a + t * (b - a); }
    float valueNoise(float x, float y)
    {
        const int ix = static_cast<int>(std::floor(x));
        const int iy = static_cast<int>(std::floor(y));
        const float fx = x - static_cast<float>(ix);
        const float fy = y - static_cast<float>(iy);
        return lerp(lerp(hash(ix, iy), hash(ix + 1, iy), smoothstep(fx)), lerp(hash(ix, iy + 1), hash(ix + 1, iy + 1), smoothstep(fx)), smoothstep(fy));
    }
    float fbm(float x, float y, int octaves = 6)
    {
        float value = 0.f, amplitude = 0.5f, frequency = 1.f;
        for (int o = 0; o < octaves; ++o) {
            value += amplitude * valueNoise(x * frequency, y * frequency);
            amplitude *= 0.5f;
            frequency *= 2.f;
        }
        return value;
    }
    struct RGB
    {
        unsigned char r, g, b;
    };
    RGB desertColor(float n)
    {
        struct Stop
        {
            float t;
            RGB c;
        };
        const Stop palette[] = {{0.00f, {210, 180, 120}}, {0.30f, {185, 140, 70}}, {0.55f, {150, 100, 50}}, {0.75f, {110, 75, 40}}, {1.00f, {70, 50, 30}}};
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
} // namespace

// ---------------------------------------------------------------------------
// Chargement d'une texture image via stb_image
// ---------------------------------------------------------------------------
GLuint RenderHelper::loadTexture(const std::string &filename, const std::string &baseDir)
{
    const std::string fullPath = baseDir + filename;

    int width = 0, height = 0, channels = 0;
    stbi_set_flip_vertically_on_load(false);
    unsigned char *data = stbi_load(fullPath.c_str(), &width, &height, &channels, 4);

    if (!data) {
        std::cerr << "Failed to load texture: " << fullPath << std::endl;
        return 0;
    }

    GLuint texId = 0;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);
    return texId;
}

// ---------------------------------------------------------------------------
// Rendu sans texture (fallback)
// ---------------------------------------------------------------------------
void RenderHelper::renderModel(GLint modelUniform, GLint colorUniform, const glm::mat4 &modelMatrix, const Mesh &mesh, const glm::vec3 &color)
{
    glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniform3f(colorUniform, color.x, color.y, color.z);
    if (mesh.vao == 0 || mesh.vertexCount <= 0)
        return;
    glBindVertexArray(mesh.vao);
    glDrawArrays(GL_TRIANGLES, 0, mesh.vertexCount);
    glBindVertexArray(0);
}

// ---------------------------------------------------------------------------
// Rendu avec texture unique (planètes procédurales)
// ---------------------------------------------------------------------------
void RenderHelper::renderModelTextured(
    GLint modelUniform, GLint colorUniform, GLint useTextureUniform, GLint textureSamplerUniform, const glm::mat4 &modelMatrix, const Mesh &mesh, const glm::vec3 &color)
{
    glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniform3f(colorUniform, color.x, color.y, color.z);

    if (mesh.vao == 0 || mesh.vertexCount <= 0)
        return;

    const bool hasTexture = (mesh.textureId != 0);
    if (useTextureUniform >= 0)
        glUniform1i(useTextureUniform, hasTexture ? 1 : 0);

    if (hasTexture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mesh.textureId);
        if (textureSamplerUniform >= 0)
            glUniform1i(textureSamplerUniform, 0);
    }

    glBindVertexArray(mesh.vao);
    glDrawArrays(GL_TRIANGLES, 0, mesh.vertexCount);
    glBindVertexArray(0);

    if (hasTexture)
        glBindTexture(GL_TEXTURE_2D, 0);

    // Reset useTexture pour ne pas contaminer les objets suivants
    if (useTextureUniform >= 0)
        glUniform1i(useTextureUniform, 0);
}

// ---------------------------------------------------------------------------
// Rendu multi-matériaux (.mtl)
// ---------------------------------------------------------------------------
void RenderHelper::renderModelMTL(GLint modelUniform,
                                  GLint colorUniform,
                                  GLint useTextureUniform,
                                  GLint textureSamplerUniform,
                                  GLint metallicUniform,
                                  GLint roughnessUniform,
                                  GLint specularUniform,
                                  GLint isEmissiveUniform,
                                  const glm::mat4 &modelMatrix,
                                  const Mesh &mesh,
                                  const glm::vec3 &color)
{
    glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Si pas de sous-meshes : fallback vers renderModelTextured
    if (!mesh.hasSubMeshes()) {
        renderModelTextured(modelUniform, colorUniform, useTextureUniform, textureSamplerUniform, modelMatrix, mesh, color);
        return;
    }

    for (const SubMesh &sub : mesh.subMeshes) {
        if (sub.vao == 0 || sub.vertexCount <= 0)
            continue;

        // Couleur de l'objet (tintée par la couleur C++ si pas de texture)
        glUniform3f(colorUniform, color.x, color.y, color.z);

        // Propriétés PBR du matériau
        if (metallicUniform >= 0)
            glUniform1f(metallicUniform, sub.metallic);
        if (roughnessUniform >= 0)
            glUniform1f(roughnessUniform, sub.roughness);
        if (specularUniform >= 0)
            glUniform1f(specularUniform, sub.specular);
        if (isEmissiveUniform >= 0)
            glUniform1i(isEmissiveUniform, sub.isEmissive ? 1 : 0);

        // Texture diffuse
        const bool hasTexture = (sub.textureId != 0);
        if (useTextureUniform >= 0)
            glUniform1i(useTextureUniform, hasTexture ? 1 : 0);

        if (hasTexture) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, sub.textureId);
            if (textureSamplerUniform >= 0)
                glUniform1i(textureSamplerUniform, 0);
        }

        glBindVertexArray(sub.vao);
        glDrawArrays(GL_TRIANGLES, 0, sub.vertexCount);
        glBindVertexArray(0);

        if (hasTexture)
            glBindTexture(GL_TEXTURE_2D, 0);
    }

    // Reset tous les uniforms après le rendu pour ne pas contaminer les objets suivants
    if (useTextureUniform >= 0)
        glUniform1i(useTextureUniform, 0);
    if (isEmissiveUniform >= 0)
        glUniform1i(isEmissiveUniform, 0);
}

// ---------------------------------------------------------------------------
// Chargement OBJ avec support MTL complet
// ---------------------------------------------------------------------------
Mesh RenderHelper::loadObjMesh(const std::string &path)
{
    Mesh mesh;

    // Détermine le dossier de base pour les textures
    std::string baseDir;
    const size_t lastSlash = path.find_last_of("/\\");
    if (lastSlash != std::string::npos)
        baseDir = path.substr(0, lastSlash + 1);

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    const bool loaded = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str(), baseDir.c_str(), true);

    if (!loaded) {
        std::cerr << "Failed to load OBJ file: " << path << std::endl;
        return mesh;
    }

    mesh.attrib = attrib;
    mesh.shapes = shapes;

    const bool hasMaterials = !materials.empty();

    // Cache textures déjà chargées (évite de charger le même jpg plusieurs fois)
    std::unordered_map<std::string, GLuint> textureCache;
    auto getOrLoadTexture = [&](const std::string &texName) -> GLuint {
        if (texName.empty())
            return 0;
        auto it = textureCache.find(texName);
        if (it != textureCache.end())
            return it->second;
        GLuint id = loadTexture(texName, baseDir);
        textureCache[texName] = id;
        return id;
    };

    if (hasMaterials) {
        // ── Mode multi-matériaux : un SubMesh par material_id ────────────────
        // Regroupe les faces par matériau
        // matId → buffer de floats
        std::unordered_map<int, std::vector<float>> matBuffers;

        for (const auto &shape : shapes) {
            const size_t faceCount = shape.mesh.num_face_vertices.size();
            size_t indexOffset = 0;

            for (size_t f = 0; f < faceCount; ++f) {
                const int matId = shape.mesh.material_ids[f];
                const int faceVerts = shape.mesh.num_face_vertices[f];

                std::vector<float> &buf = matBuffers[matId];

                // Triangulation simple (fan depuis v0)
                for (int v = 1; v < faceVerts - 1; ++v) {
                    pushVertex(buf, attrib, shape.mesh.indices[indexOffset]);
                    pushVertex(buf, attrib, shape.mesh.indices[indexOffset + v]);
                    pushVertex(buf, attrib, shape.mesh.indices[indexOffset + v + 1]);
                }
                indexOffset += static_cast<size_t>(faceVerts);
            }
        }

        // Crée un SubMesh par matériau
        for (auto it = matBuffers.begin(); it != matBuffers.end(); ++it) {
            const int matId = it->first;
            std::vector<float> &buf = it->second;
            if (buf.empty())
                continue;

            SubMesh sub;
            buildVAO(sub.vao, sub.vbo, buf);
            sub.vertexCount = static_cast<GLsizei>(buf.size() / STRIDE);

            if (matId >= 0 && matId < static_cast<int>(materials.size())) {
                const tinyobj::material_t &mat = materials[static_cast<size_t>(matId)];

                // Charge la texture diffuse (map_Kd)
                sub.textureId = getOrLoadTexture(mat.diffuse_texname);

                // Conversion Phong → PBR Disney approximative
                // Ns (shininess) 0..1000 → roughness 1..0
                const float ns = mat.shininess;
                sub.roughness = std::max(0.05f, 1.0f - std::sqrt(ns / 1000.0f));

                // Ks moyen → specular
                const float ksAvg = (mat.specular[0] + mat.specular[1] + mat.specular[2]) / 3.0f;
                sub.specular = ksAvg;

                // Métal : si Ks > Kd et pas de texture → probablement métal
                const float kdAvg = (mat.diffuse[0] + mat.diffuse[1] + mat.diffuse[2]) / 3.0f;
                sub.metallic = (ksAvg > kdAvg && sub.textureId == 0) ? 0.7f : 0.0f;

                // Émissif : Ke > 0
                const float keAvg = (mat.emission[0] + mat.emission[1] + mat.emission[2]) / 3.0f;
                sub.isEmissive = (keAvg > 0.1f);
            }

            mesh.subMeshes.push_back(std::move(sub));
        }

    } else {
        // ── Mode fallback : VAO unique (pas de .mtl) ─────────────────────────
        std::vector<float> data;
        data.reserve(shapes.size() * 30);

        for (const auto &shape : shapes)
            for (const auto &index : shape.mesh.indices)
                pushVertex(data, attrib, index);

        if (data.empty()) {
            std::cerr << "No triangles parsed from OBJ file: " << path << std::endl;
            return mesh;
        }

        buildVAO(mesh.vao, mesh.vbo, data);
        mesh.vertexCount = static_cast<GLsizei>(data.size() / STRIDE);
    }

    return mesh;
}

// ---------------------------------------------------------------------------
// Cylindre procédural
// ---------------------------------------------------------------------------
Mesh RenderHelper::createCylinderMesh(int segments)
{
    Mesh mesh;
    if (segments < 3)
        return mesh;

    const float radius = 0.5f;
    const float halfHeight = 0.5f;
    const float step = 2.0f * static_cast<float>(M_PI) / static_cast<float>(segments);

    std::vector<float> data;
    data.reserve(static_cast<size_t>(segments) * STRIDE * 12);

    auto push = [&](float px, float py, float pz, float nx, float ny, float nz, float u, float v) { data.insert(data.end(), {px, py, pz, 1.f, nx, ny, nz, 0.f, u, v}); };

    for (int i = 0; i < segments; ++i) {
        const float a0 = step * static_cast<float>(i);
        const float a1 = step * static_cast<float>(i + 1);
        const float c0 = cosf(a0), s0 = sinf(a0);
        const float c1 = cosf(a1), s1 = sinf(a1);
        const float u0 = static_cast<float>(i) / static_cast<float>(segments);
        const float u1 = static_cast<float>(i + 1) / static_cast<float>(segments);

        push(radius * c0, radius * s0, -halfHeight, c0, s0, 0, u0, 0);
        push(radius * c0, radius * s0, halfHeight, c0, s0, 0, u0, 1);
        push(radius * c1, radius * s1, halfHeight, c1, s1, 0, u1, 1);
        push(radius * c0, radius * s0, -halfHeight, c0, s0, 0, u0, 0);
        push(radius * c1, radius * s1, halfHeight, c1, s1, 0, u1, 1);
        push(radius * c1, radius * s1, -halfHeight, c1, s1, 0, u1, 0);

        push(0, 0, halfHeight, 0, 0, 1, 0.5f, 0.5f);
        push(radius * c0, radius * s0, halfHeight, 0, 0, 1, (c0 + 1) * .5f, (s0 + 1) * .5f);
        push(radius * c1, radius * s1, halfHeight, 0, 0, 1, (c1 + 1) * .5f, (s1 + 1) * .5f);

        push(0, 0, -halfHeight, 0, 0, -1, 0.5f, 0.5f);
        push(radius * c1, radius * s1, -halfHeight, 0, 0, -1, (c1 + 1) * .5f, (s1 + 1) * .5f);
        push(radius * c0, radius * s0, -halfHeight, 0, 0, -1, (c0 + 1) * .5f, (s0 + 1) * .5f);
    }

    buildVAO(mesh.vao, mesh.vbo, data);
    mesh.vertexCount = static_cast<GLsizei>(data.size() / STRIDE);
    return mesh;
}

// ---------------------------------------------------------------------------
// Texture procédurale désertique (planètes)
// ---------------------------------------------------------------------------
void RenderHelper::generateDesertPlanetTexture(Mesh &mesh, int width, int height, unsigned int seed)
{
    std::vector<unsigned char> pixels(static_cast<size_t>(width * height * 3));

    const float offsetX = static_cast<float>(seed) * 3.7f;
    const float offsetY = static_cast<float>(seed) * 2.3f;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const float fx = (static_cast<float>(x) / static_cast<float>(width)) * 4.0f + offsetX;
            const float fy = (static_cast<float>(y) / static_cast<float>(height)) * 4.0f + offsetY;

            const float warpX = fbm(fx + 1.7f, fy + 9.2f, 3) * 0.4f;
            const float warpY = fbm(fx + 8.3f, fy + 2.8f, 3) * 0.4f;
            float n = fbm(fx + warpX, fy + warpY, 5);
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
// ---------------------------------------------------------------------------
// Helpers internes pour les nouvelles textures procédurales
// ---------------------------------------------------------------------------
namespace {
    struct RGB2
    {
        unsigned char r, g, b;
    };

    float hash2(int x, int y)
    {
        int n = x + y * 57;
        n = (n << 13) ^ n;
        return 1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f;
    }
    float smoothstep2(float t) { return t * t * (3.0f - 2.0f * t); }
    float lerp2(float a, float b, float t) { return a + t * (b - a); }
    float valueNoise2(float x, float y)
    {
        const int ix = static_cast<int>(std::floor(x));
        const int iy = static_cast<int>(std::floor(y));
        const float fx = x - static_cast<float>(ix);
        const float fy = y - static_cast<float>(iy);
        return lerp2(lerp2(hash2(ix, iy), hash2(ix + 1, iy), smoothstep2(fx)), lerp2(hash2(ix, iy + 1), hash2(ix + 1, iy + 1), smoothstep2(fx)), smoothstep2(fy));
    }
    float fbm2(float x, float y, int octaves = 5)
    {
        float v = 0.f, a = 0.5f, f = 1.f;
        for (int o = 0; o < octaves; ++o) {
            v += a * valueNoise2(x * f, y * f);
            a *= 0.5f;
            f *= 2.f;
        }
        return v;
    }

    RGB2 grassColor2(float n, float detail)
    {
        struct Stop
        {
            float t;
            RGB2 c;
        };
        const Stop p[] = {
            {0.00f, {34, 89, 28}},
            {0.25f, {52, 120, 40}},
            {0.50f, {72, 148, 52}},
            {0.70f, {88, 160, 60}},
            {0.85f, {110, 140, 45}},
            {1.00f, {140, 130, 55}},
        };
        float t = std::fmax(0.f, std::fmin(1.f, n * 0.7f + detail * 0.3f));
        for (int i = 0; i < 5; ++i) {
            if (t >= p[i].t && t <= p[i + 1].t) {
                float f = (t - p[i].t) / (p[i + 1].t - p[i].t);
                return {(unsigned char)(p[i].c.r + f * (p[i + 1].c.r - p[i].c.r)),
                        (unsigned char)(p[i].c.g + f * (p[i + 1].c.g - p[i].c.g)),
                        (unsigned char)(p[i].c.b + f * (p[i + 1].c.b - p[i].c.b))};
            }
        }
        return p[5].c;
    }

    RGB2 sunColor2(float n, float turb)
    {
        struct Stop
        {
            float t;
            RGB2 c;
        };
        const Stop p[] = {
            {0.00f, {180, 40, 0}},
            {0.20f, {220, 90, 0}},
            {0.45f, {255, 160, 0}},
            {0.70f, {255, 210, 30}},
            {1.00f, {255, 245, 180}},
        };
        float t = std::fmax(0.f, std::fmin(1.f, n * 0.6f + turb * 0.4f));
        for (int i = 0; i < 4; ++i) {
            if (t >= p[i].t && t <= p[i + 1].t) {
                float f = (t - p[i].t) / (p[i + 1].t - p[i].t);
                return {(unsigned char)(p[i].c.r + f * (p[i + 1].c.r - p[i].c.r)),
                        (unsigned char)(p[i].c.g + f * (p[i + 1].c.g - p[i].c.g)),
                        (unsigned char)(p[i].c.b + f * (p[i + 1].c.b - p[i].c.b))};
            }
        }
        return p[4].c;
    }
}

static GLuint uploadTexture(const std::vector<unsigned char> &pixels, int w, int h)
{
    GLuint texId = 0;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    return texId;
}

// ---------------------------------------------------------------------------
// Texture procédurale herbe (planètes)
// ---------------------------------------------------------------------------
void RenderHelper::generateGrassPlanetTexture(Mesh &mesh, int width, int height, unsigned int seed)
{
    std::vector<unsigned char> pixels(static_cast<size_t>(width * height * 3));
    const float ox = static_cast<float>(seed) * 3.7f;
    const float oy = static_cast<float>(seed) * 2.3f;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const float fx = (static_cast<float>(x) / static_cast<float>(width)) * 6.0f + ox;
            const float fy = (static_cast<float>(y) / static_cast<float>(height)) * 6.0f + oy;

            const float wx = fbm2(fx + 1.7f, fy + 9.2f, 2) * 0.3f;
            const float wy = fbm2(fx + 8.3f, fy + 2.8f, 2) * 0.3f;
            float n = fbm2(fx + wx, fy + wy, 4);
            n = std::fmax(0.f, std::fmin(1.f, (n + 1.0f) * 0.5f));

            float detail = valueNoise2(fx * 8.0f, fy * 8.0f);
            detail = (detail + 1.0f) * 0.5f;

            const RGB2 c = grassColor2(n, detail);
            const size_t idx = static_cast<size_t>((y * width + x) * 3);
            pixels[idx] = c.r;
            pixels[idx + 1] = c.g;
            pixels[idx + 2] = c.b;
        }
    }
    mesh.textureId = uploadTexture(pixels, width, height);
}

// ---------------------------------------------------------------------------
// Texture procédurale surface solaire (soleil)
// ---------------------------------------------------------------------------
void RenderHelper::generateSunTexture(Mesh &mesh, int width, int height)
{
    std::vector<unsigned char> pixels(static_cast<size_t>(width * height * 3));

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const float fx = (static_cast<float>(x) / static_cast<float>(width)) * 5.0f;
            const float fy = (static_cast<float>(y) / static_cast<float>(height)) * 5.0f;

            const float wx = fbm2(fx + 2.3f, fy + 7.1f, 3) * 0.5f;
            const float wy = fbm2(fx + 9.4f, fy + 1.6f, 3) * 0.5f;
            float n = fbm2(fx + wx, fy + wy, 5);
            n = std::fmax(0.f, std::fmin(1.f, (n + 1.0f) * 0.5f));

            float turb = fbm2(fx * 3.0f + 4.4f, fy * 3.0f + 2.2f, 3);
            turb = (turb + 1.0f) * 0.5f;

            // Taches solaires
            const float spot = (fbm2(fx * 0.8f + 1.1f, fy * 0.8f + 3.3f, 2) + 1.0f) * 0.5f;
            if (spot < 0.2f)
                n *= spot * 2.5f;

            const RGB2 c = sunColor2(n, turb);
            const size_t idx = static_cast<size_t>((y * width + x) * 3);
            pixels[idx] = c.r;
            pixels[idx + 1] = c.g;
            pixels[idx + 2] = c.b;
        }
    }
    mesh.textureId = uploadTexture(pixels, width, height);
}