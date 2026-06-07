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

Create a BoundingBox class for collision detection. Implement a CollisionHelper class with a generateBoundingBox method that takes a Mesh struct and uses its shapes and attrib properties to get the min and max of bounding box. On object, create a new boundingBox property that is initialized with generateBoundingBox after the mesh is loaded. The boundingBox property needs to be static for each class that extends Object so it can be reused for multiple instances. On BoundingBox class, implement a testCollision method that takes the boundingBox and position of a object and tests it against the boundingBox and position of another object, printing "The Objects Collided!" if it is true. Call testCollision on Asteroid class, testing it against the Spaceship instance.

## 22-bounding-sphere

Create a Collider class and make BoundingBox extend it. Create a BoundingSphere class that also extends Collider and define a generateBoundingSphere method to CollisionHelper. Change the Object to instead of having boundingBox, have a collider property for more flexibility. The Spaceship will still have a BoundingBox, while all classes that extend CelestialBody will have a BoundingSphere. Implement in BoundingSphere a test for BoundingSphere agains BoundingSphere and BoundingSphere agains BoundingBox.

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
