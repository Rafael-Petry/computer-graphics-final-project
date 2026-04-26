# 7-organize-shaders-and-render-correct-models

Move the fragment shader code from *src/main.cpp* into *src/shaders/fragment.glsl*. Also, move the vertex shader code from *src/main.cpp* into *src/shaders/vertex.glsl*. Change the render logic of **CelestialBody** and **Spaceship** to use the correct model, loaded on the *model* variable.