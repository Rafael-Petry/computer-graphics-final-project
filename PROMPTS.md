# Prompts

Este arquivo agrupa todos prompts executados por agentes de IA durante o desenvolvimento do trabalho. Na prática, cada prompt foi executado separademente pedindo para o agente ler o arquivo isolado do prompt correspondente em **_prompts/_**, com comandos como: `Run the prompt on prompts/*.md`.

## 0-limitations

Run the following prompt knowing that the ambient has GLFW, glm and glad. Any matrices calculations need to be explicit, without the encapsulation of pre-defined functions from glm or other libraries. For example, you can't use pre-defined functions for camera and transform such as: gluLookAt(), gluOrtho2D(), gluPerspective(), gluPickMatrix(), gluProject(), gluUnProject(), glm::lookAt(), glm::ortho(), glm::perspective(), glm::pickMatrix(), glm::rotate(), glm::scale(), glm::translate(). Instead, use the functions on the _src/vendor/include/matrices.h_ file.

## 1-structure

Create a folder _\_old/_. Read the current project and reestructure it in a way so that what is current on _src/_ folder becomes _\_old/src/_. Next, move the _data/_ folder to _\_old/src/_. Move _src/main.cpp_ to _\_old/src/_. Move _CMakeLists.txt_, _CMakePresets.json_, _Makefile_, _Makefile.macOS_ and _.dir-locals.el_ to _\_old/_ so that compilation only happens if it is run with _\_old/_ as the target. Also, move _COMPILACAO.md_ to _\_old/_.

Finally, move _lib-linux_, _lib-mingw-32_, _lib-migw-64_, _lib-ucrt-64_, _lib-vc2022_ and _includes/_ to a new folder _src/vendor_.

Make the necessary changes on files that references those old paths so the project runs correctly.

## 2-spaceship-init

On _src/spaceship/_, implement a Spaceship class editing the _src/spaceship/spaceship.cpp_ and _src/spaceship/spaceship.h_ files. The class needs to have a static variable called _model_ referencing the _src/spaceship/spaceship.obj_ file, that will store the model for this object. It needs to have a function called _translate()_ that will print "Moving the spaceship...", a function called _rotate()_ that will print "Rotating the spaceship", a function called _shoot()_ that will print "The spaceship is shooting..." and a static function _getModel()_ to return the _model_ variable.

Edit _src/spaceship/spaceship.obj_ to store a simple cube model.

Edit _src/main.cpp_ to instantiate an object of the Spaceship class and call all its functions.

Finally, make any changes to paths to compile the project correctly.

## 3-celestial-bodies-init

On _src/celestialBody/_, implement an abstract **CelestialBody** class editing the _src/celestialBody/celestialBody.cpp_ and _src/celestialBody/celestialBody.h_ files. The class needs to have a static variable called _model_ referencing the _src/celestialBody/celestialBody.obj_ file, that will store the model for this object. It needs to have a static _color_ variable that will store the gray color as a HEX string. It needs to have an abstract function called _translate()_, an abstract function called _rotate()_ and a static function _getModel()_ to return the _model_ variable.

Edit _src/celestialBody/asteroid/asteroid.cpp_ and _src/celestialBody/asteroid/asteroid.h_ to implement an **Asteroid** class that extends **CelestialBody**. It needs to override _translate()_ to print "Moving asteroid towards the spaceship..." and _rotate()_ to print "Rotating the asteroid...".

Edit _src/celestialBody/sun/sun.cpp_ and _src/celestialBody/sun/sun.h_ to implement a **Sun** class that extends **CelestialBody**. It needs to override _translate()_ to print "The sun stands still..." and _rotate()_ to print "Rotating the sun...". It also needs to override the color variable to store a yellow color as a HEX string.

Edit _src/celestialBody/planet/planet.cpp_ and _src/celestialBody/planet/planet.h_ to implement a **Planet** class that extends **CelestialBody**. It needs to have a variable _sun_ that will reference an object of the **Sun** class. It needs to override the _color_ variable to store a green color as a HEX string. It needs to override _translate()_ to print "The planet is orbiting the sun..." and _rotate()_ to print "Rotating the planet...". It needs to have a function _getSun()_ that returns the _sun_ variable.

Edit _src/main.cpp_ to instantiate an object of the Asteroid, Sun and Planet classes and call all its functions.

Finally, make any changes to paths to compile the project correctly.

## 4-celestial-bodies-init

On _src/celestialBody/_, add a function _getColor()_ to the **CelestialBody** class that returns the _color_ variable. Change the **Sun** class to be a singleton. Remove the _sun_ variable from **Planet** and its _getSun()_ function.

## 5-window

Read _prompts/0-limitations_. Change _src/main.cpp_ to create an application window. It needs to be resizable.

## 6-scene

Read _prompts/0-limitations_. On the window initialized on _src/main.cpp_, load scene with a camera that shows a **Spaceship**, a **Sun**, an **Asteroid** and three **Planets**. For that, on _src/spaceship/scapeship.cpp_, implement the render function so that I can just call render on the object from main to instantiate its model. Do the same for the render function on _src/celestialBody/celestialBody.cpp_, taking into account the color property of the object. The scene needs to have a free camera. Implement the camera operations on _src/camera/camera.cpp_ and _src/camera/camera.h_ files to keep _src/main.cpp_ clean.

## 7-organize-shaders-and-render-correct-models

Move the fragment shader code from _src/main.cpp_ into _src/shaders/fragment.glsl_. Also, move the vertex shader code from _src/main.cpp_ into _src/shaders/vertex.glsl_. Change the render logic of **CelestialBody** and **Spaceship** to use the correct model, loaded on the _model_ variable.

## 8-organize-rendering

On _src/helpers/render.cpp_, create a helper class that implements a static rendering function using the tiny*obj_loader library. Change the rendering logic on the **Spaceship** and **CelestialBody** class to just use this centralized rendering function on \_src/helpers/render.cpp*.

## 9-organize-shaders-color-input

On _src/helpers/color.cpp_, create a helper class that implements a static hex to rgb color conversion function. On **CelestialBody**, use this new function instead.

On _src/helpers/input.cpp_, create a helper class that processes all inputs on static functions. Change _src/main.cpp_ and _src/camera/camera.cpp_ to use the newly created function.

Create a shader loader class on _src/shaders/loader/loader.cpp_, Move any shader loading functions from _src/main.cpp_ to this class, create them as static.

## 10-organize-window

Create a instantiable window class on _src/window/window.cpp_. Move all window logic from \*src/main.cpp/ to this newly created class.

## 11-spaceship-update-extension

On the Spaceship class, extend the parent update function so when the user is holding the R key, it prints "They see me rolling, they hating" on the terminal.

## 12-spaceship-roll

Read _prompts/0-limitations_. Change the spaceship's updateView method to, when the isRolling flag is true, instead of controlling pitch and yaw, it controlls the roll.

## 13-spaceship-local-rotation

Read _prompts/0-limitations_. Change the spaceship's rotate method so the rotation is local instead of global.

## 14-spaceship-inertia

Read _prompts/0-limitations_. Change the spaceship's movement to have inertia as it is on outer space. Moving on a particular directions accelerates the spaceship and moving on the opposite decelerates it.

## 15-spaceship-rotation-inertia

Read _prompts/0-limitations_. Change the spaceship's rotation to have inertia as it is on outer space. Rotating on a particular axis accelerates the spaceship's rotationg and moving on the opposite decelerates it.

## 16-spaceship-drag-inertia

Read _prompts/0-limitations_. Change the spaceship's rotation and movement to have drag to the inertia so the velocity decays over time.

## 17-planet-animation

Read _prompts/0-limitations_. Change the planets' translation method to use 2 cubic bezier curves around the sun instead. Change the Sun class to store its position and create a getPosition method that returns it. On planet, use the singleton sun to get the position and generate 4 points around it for the curves. It needs to be controlled by deltaTime.

## 18-asteroid-animation

Read _prompts/0-limitations_. Change the asteroid's translation method to trace a line towards the spaceship. Change the spaceship to be a singleton and then access its position on the asteroid's translation method.

## 19-fix-first-person-rotation

Read _prompts/0-limitations_. Change the spaceship's rotation to be consistant with the first person camera. Looking up or down should always feel like looking up or down and looking left or right should always feel like looking left or right in relation to the screen, even if roll is applied.

## 20-refactor-mesh-generation

Change the mesh generation logic to make it so all objects have a reference to its Mesh instead of saving it on a cache of RenderHelper. This reference to the mesh needs to be static so it only needs to load once per class and should be set by the return value of RenderHelper::loadObjMesh. You can remove the getMesh method of RenderHelper because it won't be used anymore and adjust the renderModel method to have the mesh as its parameter instead of the meshPath.

## 21-collision

Create a BoundingBox class for collision detection. Implement a ColliderGenerator class with a generateBoundingBox method that takes a Mesh struct and uses its shapes and attrib properties to get the min and max of bounding box. On object, create a new boundingBox property that is initialized with generateBoundingBox after the mesh is loaded. The boundingBox property needs to be static for each class that extends Object so it can be reused for multiple instances. On BoundingBox class, implement a testCollision method that takes the boundingBox and position of a object and tests it against the boundingBox and position of another object, printing "The Objects Collided!" if it is true. Call testCollision on Asteroid class, testing it against the Spaceship instance.

## 22-bounding-sphere

Create a Collider class and make BoundingBox extend it. Create a BoundingSphere class that also extends Collider and define a generateBoundingSphere method to ColliderGenerator. Change the Object to instead of having boundingBox, have a collider property for more flexibility. The Spaceship will still have a BoundingBox, while all classes that extend CelestialBody will have a BoundingSphere. Implement in BoundingSphere a test for BoundingSphere agains BoundingSphere and BoundingSphere agains BoundingBox.

## 23-refactor-collisions

Change the Object class to have a property scale and a getScale method that returns it. Use the scale property on the scale() method of transform. Then, change the testCollision methods on the colliders classes to instead only take two objects as parameters and use their getters to implement the test. Undo the testCollision overloading where it exists, by specifying different tests, such as testCollisionBoundingBox and testCollisionSpheres. Create a separate collide() method for the Object class and call it after all the transform operations, so you can also isolate the testCollision call on Asteroid's translate into this method.

Add the position property to the Object class so all objects just extend it. Change the Object class getPosition to always return that and remove getPosition from child classes.

## 24-spaceship-shooting

Add a functionality for the spaceship be able to shoot an instant speed ray that collides with any asteroid that it finds in its way. Add a crosshair to the camera to indicate where the ray will be shot. The ray needs to have a max range, so anything further from this range doesn't get hit. There needs to be a 1 second cooldown between each shot. Make the ray visible as a red cilindrical line that extends from the camera.

## 25-multiple-asteroids

Change the scene to instead have multiple asteroids (add a property that controls the amount, by default 10). Each asteroid has to spawn on a random position. Shooting has to test collision with each asteroid.

Make it so the shot can do collateral damage to asteroids behind the one being hit.

When hit, the asteroid need to be respawned on a different random position too.

## 26-health-and-score

On the Spaceship class, create two integer properties called score (starting as 0) and health (starting as 3). Whenever the spaceship shoots an asteroid, you need to increase the score by 100. When an asteroid hits the Spaceship, the asteroid needs to be destroyed and it should decrease the health by 1.

Import the imgui library to this project as a vendor. On the Scene class, create a text on the upper left corner displaying the spaceship's health. On the upper right corner, create a text that displays the spaceship's score.

## 27-asteroid-size

Make it so the asteroids are able to have different sizes (small, medium and large) and multiply its scaleValue by a factor of this size.

Smaller asteroids need to be faster than larger ones. Add a property enableRespawn to the asteroid. All initial asteroids need to have this property set to true.

Asteroids that have enableRespawn = false are still destructible.

Destroying an asteroid needs to also spawn 3 asteroids of a smaller size if it is not a small asteroid. The new asteroids have enableRespawn set to false and are generated around the destroyed asteroid.

## 28-asteroid-spawner

Create an AsteroidSpawnerHelper class that is responsible for spawning asteroids at "waves", in increments of 5 per round. Each round needs to have 30 seconds of downtime. The initial amount of asteroids should be 10. The asteroids have to be spawned in a large radius around the spaceship to avoid instant collisions. The AsteroidSpawnerHelper should have static functions to be called on the Scene class.

## 29-multiple-planets

Change the Scene to have 5 planets in orbit of the sun. Each orbit needs to be progressively further from the sun. You can change the Planet class to have the distance from the sun as a property for the bézier curve. Also, make the speed of the orbit slower. Start each planet on a random orbit phase.

## 30-spaceship-landing-and-takeoff

Make it so the spaceship is able to land on a planet using collisions. The landing needs to stop all momentum for the spaceship and fix it to the planet. It also needs to lock the spaceship rotation while landed so it looks straight.

The spaceship needs to be able to takeoff from the planet when holding shift. This unlocks the position and rotation of the spaceship.

## 31-conditional-landing

Make it so the spaceship can only land on the planet if it is approaching with its base facing the planet's surface. If it is not, then the spaceship will be bumped away and suffer 1 damage to its health.

## 32-look-at-camera

Replace the scene camera by a look-at camera that appears behind of the spaceship. Whenever the player changes cameras using the C key, the look-at camera teleports right behind of the ship.

## 33-radar-interface

On the Scene class, create a new user interface that works as a radar/map. It shows the spaceship's location at the very center as a white dot, the asteroids' position as a red dot, the planets as a green dot and the sun as a yellow dot. If any of the objects are below the spaceship, instead it shows the position as an arrow pointing down. Similarly, if it is on top of the ship, it shows as an arrow pointing up. The radar should show as a square on the bottom right corner. If any object is out of the square's bounds, than it shows on top of the edge.

Change the spaceship's representation from being a dot to instead being an arrow that points towards where it is looking.

## 34-final-tweaks

Make it so the asteroids can collide with planets and the sun. When colliding with these objects, it should destroy the asteroid and not grant points or spawn smaller asteroids.

When the spaceship lands on a planet, it should restore its health up to 5. Also when health reaches 0, the game should close.

Create an interface that is enabled when a wave of asteroids has been cleared. This interface is a text that counts down the time for the next wave to be spawned. Once the wave has been spawned, disable this interface again.

## 35-trees-and-bushes

Edit src/objects/bush/ and src/objects/tree/ to create a Bush class and a Tree class. Both these classes need to extend the Object class. For trees, create a BoundingBox collider that will bump the spaceship, similarly to how the Planet bumps it. Bushes wont have any collisions. Both Trees and Bushes need to be spawned on a Planet's surface.

## 36-transparency-tree-leaves

Change my fragment shader to use the alpha channel correctly to render the tree leaves texture. Right now, it incorrectly has some white background around it.

<pre>
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
// glUniform3f(sunPositionUniform, sun.x, sun.y, sun.z);
uniform vec3 sunPosition;
uniform vec3 sunColor; // ex. vec3(1.0, 0.95, 0.8) pour un soleil chaud
uniform float sunIntensity; // ex. 3.0

// ─── Paramètres PBR Disney ──────────────────────────────────────────────────
// Ces valeurs peuvent devenir des uniforms plus tard pour les varier par objet.
uniform float metallic ; // 0 = diélectrique, 1 = métal
uniform float roughness ; // 0 = miroir, 1 = mat
uniform float subsurface ;
uniform float specular ;
uniform float specularTint ;
uniform float anisotropic ;
uniform float sheen ;
uniform float sheenTint ;
uniform float clearcoat ;
uniform float clearcoatGloss ;

// ─── Émission (ex: soleil) ───────────────────────────────────────────────────
uniform bool isEmissive; // true = pas de shading, couleur pure émissive
uniform vec3 ambientColor; // ex. vec3(0.02, 0.02, 0.05) pour l'espace

// ─── Sortie ──────────────────────────────────────────────────────────────────
out vec4 color;

// ════════════════════════════════════════════════════════════════════════════
// <START OF THIRD-PARTY SOURCE CODE>
// Copyright Disney Enterprises, Inc. All rights reserved.
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

float sqr(float x) { return x \* x; }

float SchlickFresnel(float u)
{
float m = clamp(1.0 - u, 0.0, 1.0);
float m2 = m _ m;
return m2 _ m2 \* m;
}

float GTR1(float NdotH, float a)
{
if (a >= 1.0) return 1.0 / PI;
float a2 = a _ a;
float t = 1.0 + (a2 - 1.0) _ NdotH _ NdotH;
return (a2 - 1.0) / (PI _ log(a2) \* t);
}

float GTR2(float NdotH, float a)
{
float a2 = a _ a;
float t = 1.0 + (a2 - 1.0) _ NdotH _ NdotH;
return a2 / (PI _ t \* t);
}

float GTR2_aniso(float NdotH, float HdotX, float HdotY, float ax, float ay)
{
return 1.0 / (PI _ ax _ ay _ sqr(sqr(HdotX / ax) + sqr(HdotY / ay) + NdotH _ NdotH));
}

float smithG_GGX(float NdotV, float alphaG)
{
float a = alphaG _ alphaG;
float b = NdotV _ NdotV;
return 1.0 / (NdotV + sqrt(a + b - a \* b));
}

float smithG_GGX_aniso(float NdotV, float VdotX, float VdotY, float ax, float ay)
{
return 1.0 / (NdotV + sqrt(sqr(VdotX _ ax) + sqr(VdotY _ ay) + sqr(NdotV)));
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
vec4 origin = vec4(0.0, 0.0, 0.0, 1.0);
vec4 camera_position = inverse(view) \* origin;

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
</pre>

## 37-procedural-generation-discontinuation

Procedural generation of grass and sun textures look not continuous where both ends meet.

<pre>
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
</pre>
