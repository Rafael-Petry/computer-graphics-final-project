# To-Do

This file defines a bunch of to do lists for different tasks and functionalities that needs to be done to finish the project.

## 1. Specification

- [x] Fill the form with the group info
- [x] Record a demo video of Outer Wilds
- [x] Take 3 screenshots from the for the spec
- [x] Write SPEC.md

## 2. Restructuring The Base Project

- [x] Isolate the old application and external libraries into separate folders
- [x] Create this TODO.md
- [x] Create a simple application that compiles correctly

## 3. First Classes and Initial Assets

- [x] Define a "Spaceship" class with a cube model
- [x] Define an abstract "CelestialBody" class with a sphere model
- [x] Define a "Sun" class that extends "CelestialBody" with a yellow color
- [x] Define a "Planet" class that extends "CelestialBody" with a green color
- [x] Define a "Asteroid" class that extends "CelestialBody" with a gray color
- [x] Edit application to have a "scene camera" that shows a spaceship, a sun, three planets and an asteroid
- [x] Fix architectural problems on the project

## 4. Animation

- [] Anchor first person camera to the spaceship
- [] Implement C key to transition between first person camera and the "scene camera"
- [] Implement spaceship movement (with inertia)
    - [] W needs to go forward
    - [] S needs to go backwards
    - [] A needs to go to the left
    - [] D needs to go to the right
    - [] Shift needs to go up
    - [] Ctrl needs to go down
- [] Implement spaceship rotation
    - [] Mouse controls pitch and yaw
    - [] If player holds R, than mouse controlls roll
- [] Implement planets' "orbit movement" with cubic Bézier curves
- [] Implement asteroids' "liniar movement" towards the spaceship

## 5. Visuals

- [-] Import better meshes for the objects
- [] Import Disney's PBR shader code from other labs
- [] Make textures work
    - [] For planets, make an algorithm to generate the texture procedurally 
- [] Make lighting work
- [] ...

## 6. idk, probably collisions

- ...
