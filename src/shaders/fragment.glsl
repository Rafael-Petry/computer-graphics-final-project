#version 330 core

// ─── Entrées depuis le Vertex Shader ────────────────────────────────────────
in vec4 position_world;
in vec4 normal_world;
in vec2 texcoords;

// ─── Matrices ───────────────────────────────────────────────────────────────
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// ─── Couleur de l'objet (fallback si pas de texture) ────────────────────────
uniform vec3 objectColor;

// ─── Texture ────────────────────────────────────────────────────────────────
uniform sampler2D textureColormap;
uniform bool useTexture;

// ─── Lumière : position du soleil en world space ─────────────────────────────
// À passer depuis le C++ à chaque frame :
//   glUniform3f(sunPositionUniform, sun.x, sun.y, sun.z);
uniform vec3 sunPosition;
uniform vec3 sunColor;       // ex. vec3(1.0, 0.95, 0.8) pour un soleil chaud
uniform float sunIntensity;  // ex. 3.0

// ─── Paramètres PBR Disney ──────────────────────────────────────────────────
// Ces valeurs peuvent devenir des uniforms plus tard pour les varier par objet.
uniform float metallic        ; // 0 = diélectrique, 1 = métal
uniform float roughness       ; // 0 = miroir, 1 = mat
uniform float subsurface      ;
uniform float specular        ;
uniform float specularTint    ;
uniform float anisotropic     ;
uniform float sheen           ;
uniform float sheenTint       ;
uniform float clearcoat       ;
uniform float clearcoatGloss  ;

// ─── Émission (ex: soleil) ───────────────────────────────────────────────────
uniform bool isEmissive;  // true = pas de shading, couleur pure émissive
uniform vec3 ambientColor;    // ex. vec3(0.02, 0.02, 0.05) pour l'espace

// ─── Sortie ──────────────────────────────────────────────────────────────────
out vec4 color;

// ════════════════════════════════════════════════════════════════════════════
// <START OF THIRD-PARTY SOURCE CODE>
// Copyright Disney Enterprises, Inc.  All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License
// and the following modification to it: Section 6 Trademarks.
// deleted and replaced with:
//
// 6. Trademarks. This License does not grant permission to use the
// trade names, trademarks, service marks, or product names of the
// Licensor and its affiliates, except as required for reproducing
// the content of the NOTICE file.
//
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0

const float PI = 3.14159265358979323846;

float sqr(float x) { return x * x; }

float SchlickFresnel(float u)
{
    float m = clamp(1.0 - u, 0.0, 1.0);
    float m2 = m * m;
    return m2 * m2 * m;
}

float GTR1(float NdotH, float a)
{
    if (a >= 1.0) return 1.0 / PI;
    float a2 = a * a;
    float t = 1.0 + (a2 - 1.0) * NdotH * NdotH;
    return (a2 - 1.0) / (PI * log(a2) * t);
}

float GTR2(float NdotH, float a)
{
    float a2 = a * a;
    float t = 1.0 + (a2 - 1.0) * NdotH * NdotH;
    return a2 / (PI * t * t);
}

float GTR2_aniso(float NdotH, float HdotX, float HdotY, float ax, float ay)
{
    return 1.0 / (PI * ax * ay * sqr(sqr(HdotX / ax) + sqr(HdotY / ay) + NdotH * NdotH));
}

float smithG_GGX(float NdotV, float alphaG)
{
    float a = alphaG * alphaG;
    float b = NdotV * NdotV;
    return 1.0 / (NdotV + sqrt(a + b - a * b));
}

float smithG_GGX_aniso(float NdotV, float VdotX, float VdotY, float ax, float ay)
{
    return 1.0 / (NdotV + sqrt(sqr(VdotX * ax) + sqr(VdotY * ay) + sqr(NdotV)));
}

vec3 mon2lin(vec3 x)
{
    return vec3(pow(x[0], 2.2), pow(x[1], 2.2), pow(x[2], 2.2));
}

vec3 BRDF(vec3 L, vec3 V, vec3 N, vec3 X, vec3 Y, vec3 baseColor)
{
    float NdotL = dot(N, L);
    float NdotV = dot(N, V);
    if (NdotL < 0.0 || NdotV < 0.0) return vec3(0.0);

    vec3 H = normalize(L + V);
    float NdotH = dot(N, H);
    float LdotH = dot(L, H);

    vec3 Cdlin = mon2lin(baseColor);
    float Cdlum = 0.3 * Cdlin[0] + 0.6 * Cdlin[1] + 0.1 * Cdlin[2];

    vec3 Ctint  = Cdlum > 0.0 ? Cdlin / Cdlum : vec3(1.0);
    vec3 Cspec0 = mix(specular * 0.08 * mix(vec3(1.0), Ctint, specularTint), Cdlin, metallic);
    vec3 Csheen = mix(vec3(1.0), Ctint, sheenTint);

    float FL  = SchlickFresnel(NdotL);
    float FV  = SchlickFresnel(NdotV);
    float Fd90 = 0.5 + 2.0 * LdotH * LdotH * roughness;
    float Fd  = mix(1.0, Fd90, FL) * mix(1.0, Fd90, FV);

    float Fss90 = LdotH * LdotH * roughness;
    float Fss = mix(1.0, Fss90, FL) * mix(1.0, Fss90, FV);
    float ss  = 1.25 * (Fss * (1.0 / (NdotL + NdotV) - 0.5) + 0.5);

    float aspect = sqrt(1.0 - anisotropic * 0.9);
    float ax = max(0.001, sqr(roughness) / aspect);
    float ay = max(0.001, sqr(roughness) * aspect);
    float Ds  = GTR2_aniso(NdotH, dot(H, X), dot(H, Y), ax, ay);
    float FH  = SchlickFresnel(LdotH);
    vec3  Fs  = mix(Cspec0, vec3(1.0), FH);
    float Gs;
    Gs  = smithG_GGX_aniso(NdotL, dot(L, X), dot(L, Y), ax, ay);
    Gs *= smithG_GGX_aniso(NdotV, dot(V, X), dot(V, Y), ax, ay);

    vec3 Fsheen = FH * sheen * Csheen;

    float Dr = GTR1(NdotH, mix(0.1, 0.001, clearcoatGloss));
    float Fr = mix(0.04, 1.0, FH);
    float Gr = smithG_GGX(NdotL, 0.25) * smithG_GGX(NdotV, 0.25);

    return ((1.0 / PI) * mix(Fd, ss, subsurface) * Cdlin + Fsheen) * (1.0 - metallic)
         + Gs * Fs * Ds
         + 0.25 * clearcoat * Gr * Fr * Dr;
}
// <END OF THIRD-PARTY SOURCE CODE>
// ════════════════════════════════════════════════════════════════════════════

void main()
{
    // ── Position caméra ──────────────────────────────────────────────────────
    vec4 origin          = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 camera_position = inverse(view) * origin;

    vec4 p = position_world;

    // ── Normale (normalisée) ──────────────────────────────────────────────────
    vec3 N = normalize(normal_world.xyz);

    // ── Direction vers la caméra ──────────────────────────────────────────────
    vec3 V = normalize((camera_position - p).xyz);

    // ── Couleur de base : texture ou objectColor ──────────────────────────────
    vec3 baseColor;
    if (useTexture)
        baseColor = texture(textureColormap, texcoords).rgb;
    else
        baseColor = objectColor;

    // ── Émission pure (soleil, objets lumineux) ───────────────────────────────
    if (isEmissive)
    {
        color = vec4(baseColor, 1.0);
        return;
    }

    // ── Tangente / bitangente pour le BRDF anisotropique ─────────────────────
    // On construit un repère orthonormé autour de N
    vec3 T = cross(vec3(0.0, 1.0, 0.0), N);
    if (length(T) < 0.001)
        T = cross(vec3(1.0, 0.0, 0.0), N);
    T = normalize(T);
    vec3 B = normalize(cross(N, T));

    // ── Éclairage : soleil comme source ponctuelle ────────────────────────────
    vec3 L = normalize(sunPosition - p.xyz);   // direction vers le soleil
    float NdotL = max(0.0, dot(N, L));

    vec3 direct_lighting = vec3(0.0);
    if (NdotL > 0.0)
    {
        vec3 brdf = max(vec3(0.0), BRDF(L, V, N, T, B, baseColor));
        direct_lighting = sunColor * sunIntensity * brdf * NdotL;
    }

    // ── Fill light : lumière de remplissage opposée au soleil ─────────────────
    // Douce, bleutée, pour révéler le relief dans les zones d'ombre
    vec3 fillDir = normalize(-sunPosition - p.xyz + vec3(0.0, 10.0, 0.0));
    float NdotFill = max(0.0, dot(N, fillDir));
    vec3 fillColor = vec3(0.15, 0.2, 0.35);
    vec3 fill_lighting = vec3(0.0);
    if (NdotFill > 0.0)
    {
        vec3 brdfFill = max(vec3(0.0), BRDF(fillDir, V, N, T, B, baseColor));
        fill_lighting = fillColor * brdfFill * NdotFill;
    }

    // ── Rim light : contour lumineux depuis la caméra ─────────────────────────
    // Fait ressortir les silhouettes
    float rim = pow(1.0 - max(0.0, dot(N, V)), 3.0);
    vec3 rim_lighting = rim * vec3(0.08, 0.08, 0.12) * baseColor;

    // ── Terme ambiant (lumière d'arrière-plan de l'espace) ────────────────────
    vec3 ambient_term = ambientColor * baseColor;

    // ── Couleur finale ────────────────────────────────────────────────────────
    color.rgb = direct_lighting + fill_lighting + rim_lighting + ambient_term;
    color.a   = 1.0;

    // ── Correction gamma (sRGB) ───────────────────────────────────────────────
    color.rgb = pow(color.rgb, vec3(1.0 / 2.2));
}
