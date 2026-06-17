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

- [x] Anchor first person camera to the spaceship
- [x] Implement C key to transition between first person camera and the "scene camera"
- [x] Fix spaceship rotate/translate to be consistant
- [x] Implement spaceship movement (with inertia)
    - [x] W needs to go forward
    - [x] S needs to go backwards
    - [x] A needs to go to the left
    - [x] D needs to go to the right
    - [x] Shift needs to go up
    - [x] Ctrl needs to go down
- [x] Implement spaceship rotation
    - [x] Mouse controls pitch and yaw
    - [x] If player holds R, than mouse controlls roll
- [x] Implement planets' "orbit movement" with cubic Bézier curves
- [x] Implement asteroids' "liniar movement" towards the spaceship

## 5. Visuals

- [x] Import better meshes for the objects
- [x] Import Disney's PBR shader code from other labs (https://github.com/cgvis-inf-ufrgs/cgvis-2026-1-lab-2-coelinhos-da-pascoa-cgvis-lab2-coelinhos-da-pascoa/blob/main/src/shader_fragment.glsl)
- [x] Make textures work
    - [x] For planets, make an algorithm to generate a grass texture procedurally
    - [x] For the sun you can probably generate the texture procedurally too
- [x] Make lighting work
    - [x] The sun needs to be a light source

## 6. Collisions

- [x] Refactor mesh generation to facilitate bounding box generation
- [x] Create BoundingBox class
- [x] Create Helper for bounding box generation
- [x] Create a simple collision check of asteroid with spaceship
- [x] Create BoundingSphere class
- [x] Refactor if necessary
- [x] Implement the spaceship shoot mechanic
- [x] Define all necessary collision checks without functionality for now

## 7. Actually Make It a Game

- [x] Create a health UI
- [x] Create a score UI
- [x] There should be a spawner for asteroids (needs refactoring)
- [x] Shoot collision needs to destroy asteroids and update score
- [x] Asteroids should have different sizes (small, medium and large)
- [x] Asteroid destruction needs to spawn smaller asteroids if possible
- [x] Asteroid hit on spaceship needs to damage it and despawn the asteroid
- [x] Sun hit on spaceship needs to finish the game
- [x] Make it so the scene has five planets
- [x] Planet hit on spaceship needs to bump it and damage if it is not landing
- [x] Create a look-at camera
- [x] Create a radar UI that shows the spaceship position/orientation and celestial bodies position
- [x] Add trees and bushes to planets
- [x] Code refactor and physics bug fixes
