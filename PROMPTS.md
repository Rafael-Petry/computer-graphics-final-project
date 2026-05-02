# Prompts

Este arquivo agrupa todos prompts executados por agentes de IA durante o desenvolvimento do trabalho. Na prática, cada prompt foi executado separademente pedindo para o agente ler o arquivo isolado do prompt correspondente em ***prompts/***, com comandos como: `Run the prompt on prompts/*.md`.

## 0-limitations

Run the following prompt knowing that the ambient has GLFW, glm and glad. Any matrices calculations need to be explicit, without the encapsulation of pre-defined functions from glm or other libraries. For example, you can't use pre-defined functions for camera and transform such as: gluLookAt(), gluOrtho2D(), gluPerspective(), gluPickMatrix(), gluProject(), gluUnProject(), glm::lookAt(), glm::ortho(), glm::perspective(), glm::pickMatrix(), glm::rotate(), glm::scale(), glm::translate(). Instead, use the functions on the *src/vendor/include/matrices.h* file.

## 1-structure

Create a folder *_old/*. Read the current project and reestructure it in a way so that what is current on *src/* folder becomes *_old/src/*. Next, move the *data/* folder to *_old/src/*. Move *src/main.cpp* to *_old/src/*. Move *CMakeLists.txt*, *CMakePresets.json*, *Makefile*, *Makefile.macOS* and *.dir-locals.el* to *_old/* so that compilation only happens if it is run with *_old/* as the target. Also, move *COMPILACAO.md* to *_old/*.

Finally, move *lib-linux*, *lib-mingw-32*, *lib-migw-64*, *lib-ucrt-64*, *lib-vc2022* and *includes/* to a new folder *src/vendor*.

Make the necessary changes on files that references those old paths so the project runs correctly.

## 2-spaceship-init

On *src/spaceship/*, implement a Spaceship class editing the *src/spaceship/spaceship.cpp* and *src/spaceship/spaceship.h* files. The class needs to have a static variable called *model* referencing the *src/spaceship/spaceship.obj* file, that will store the model for this object. It needs to have a function called *translate()* that will print "Moving the spaceship...", a function called *rotate()* that will print "Rotating the spaceship", a function called *shoot()* that will print "The spaceship is shooting..." and a static function *getModel()* to return the *model* variable. 

Edit *src/spaceship/spaceship.obj* to store a simple cube model.

Edit *src/main.cpp* to instantiate an object of the Spaceship class and call all its functions.

Finally, make any changes to paths to compile the project correctly.

## 3-celestial-bodies-init

On *src/celestialBody/*, implement an abstract **CelestialBody** class editing the *src/celestialBody/celestialBody.cpp* and *src/celestialBody/celestialBody.h* files. The class needs to have a static variable called *model* referencing the *src/celestialBody/celestialBody.obj* file, that will store the model for this object. It needs to have a static *color* variable that will store the gray color as a HEX string. It needs to have an abstract function called *translate()*, an abstract function called *rotate()* and a static function *getModel()* to return the *model* variable.

Edit *src/celestialBody/asteroid/asteroid.cpp* and *src/celestialBody/asteroid/asteroid.h* to implement an **Asteroid** class that extends **CelestialBody**. It needs to override *translate()* to print "Moving asteroid towards the spaceship..." and *rotate()* to print "Rotating the asteroid...".

Edit *src/celestialBody/sun/sun.cpp* and *src/celestialBody/sun/sun.h* to implement a **Sun** class that extends **CelestialBody**. It needs to override *translate()* to print "The sun stands still..." and *rotate()* to print "Rotating the sun...". It also needs to override the color variable to store a yellow color as a HEX string.

Edit *src/celestialBody/planet/planet.cpp* and *src/celestialBody/planet/planet.h* to implement a **Planet** class that extends **CelestialBody**. It needs to have a variable *sun* that will reference an object of the **Sun** class. It needs to override the *color* variable to store a green color as a HEX string. It needs to override *translate()* to print "The planet is orbiting the sun..." and *rotate()* to print "Rotating the planet...". It needs to have a function *getSun()* that returns the *sun* variable.

Edit *src/main.cpp* to instantiate an object of the Asteroid, Sun and Planet classes and call all its functions.

Finally, make any changes to paths to compile the project correctly.

## 4-celestial-bodies-init

On *src/celestialBody/*, add a function *getColor()* to the **CelestialBody** class that returns the *color* variable. Change the **Sun** class to be a singleton. Remove the *sun* variable from **Planet** and its *getSun()* function.

## 5-window

Read *prompts/0-limitations*. Change *src/main.cpp* to create an application window. It needs to be resizable.

## 6-scene

Read *prompts/0-limitations*. On the window initialized on *src/main.cpp*, load scene with a camera that shows a **Spaceship**, a **Sun**, an **Asteroid** and three **Planets**. For that, on *src/spaceship/scapeship.cpp*, implement the render function so that I can just call render on the object from main to instantiate its model. Do the same for the render function on *src/celestialBody/celestialBody.cpp*, taking into account the color property of the object. The scene needs to have a free camera. Implement the camera operations on *src/camera/camera.cpp* and *src/camera/camera.h* files to keep *src/main.cpp* clean.

## 7-organize-shaders-and-render-correct-models

Move the fragment shader code from *src/main.cpp* into *src/shaders/fragment.glsl*. Also, move the vertex shader code from *src/main.cpp* into *src/shaders/vertex.glsl*. Change the render logic of **CelestialBody** and **Spaceship** to use the correct model, loaded on the *model* variable.

## 8-organize-rendering

On *src/helpers/render.cpp*, create a helper class that implements a static rendering function using the tiny_obj_loader library. Change the rendering logic on the **Spaceship** and **CelestialBody** class to just use this centralized rendering function on *src/helpers/render.cpp*.