#version 330 core

// ─── Attributs de sommet ────────────────────────────────────────────────────
// Correspondent exactement au VBO interleaved construit dans render.cpp :
//   stride = 10 floats  →  pos(4) | normal(4) | uv(2)
layout (location = 0) in vec4 model_coefficients;   // position locale (w=1)
layout (location = 1) in vec4 normal_coefficients;  // normale locale  (w=0)
layout (location = 2) in vec2 texture_coefficients; // coordonnées UV

// ─── Matrices ───────────────────────────────────────────────────────────────
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// ─── Sorties vers le Fragment Shader ────────────────────────────────────────
out vec4 position_world;   // position du sommet en world space
out vec4 normal_world;     // normale du sommet en world space
out vec2 texcoords;        // UV interpolés

void main()
{
    // Position en clip space (obligatoire)
    gl_Position = projection * view * model * model_coefficients;

    // Position en world space (pour le calcul de l'éclairage)
    position_world = model * model_coefficients;

    // Normale en world space
    // On utilise la transposée de l'inverse pour gérer les non-uniformités
    normal_world = inverse(transpose(model)) * normal_coefficients;
    normal_world.w = 0.0;

    // UV passés tels quels, interpolés par le rasteriseur
    texcoords = texture_coefficients;
}
