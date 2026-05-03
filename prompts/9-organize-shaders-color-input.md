# 9-organize-shaders-color-input

On *src/helpers/color.cpp*, create a helper class that implements a static hex to rgb color conversion function. On **CelestialBody**, use this new function instead.

On *src/helpers/input.cpp*, create a helper class that processes all inputs on static functions. Change *src/main.cpp* and *src/camera/camera.cpp* to use the newly created function.

Create a shader loader class on *src/shaders/loader/loader.cpp*, Move any shader loading functions from *src/main.cpp* to this class, create them as static.