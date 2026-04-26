# 3-celestial-bodies-init

On *src/celestialBody/*, implement an abstract **CelestialBody** class editing the *src/celestialBody/celestialBody.cpp* and *src/celestialBody/celestialBody.h* files. The class needs to have a static variable called *model* referencing the *src/celestialBody/celestialBody.obj* file, that will store the model for this object. It needs to have a static *color* variable that will store the gray color as a HEX string. It needs to have an abstract function called *translate()*, an abstract function called *rotate()* and a static function *getModel()* to return the *model* variable.

Edit *src/celestialBody/asteroid/asteroid.cpp* and *src/celestialBody/asteroid/asteroid.h* to implement an **Asteroid** class that extends **CelestialBody**. It needs to override *translate()* to print "Moving asteroid towards the spaceship..." and *rotate()* to print "Rotating the asteroid...".

Edit *src/celestialBody/sun/sun.cpp* and *src/celestialBody/sun/sun.h* to implement a **Sun** class that extends **CelestialBody**. It needs to override *translate()* to print "The sun stands still..." and *rotate()* to print "Rotating the sun...". It also needs to override the color variable to store a yellow color as a HEX string.

Edit *src/celestialBody/planet/planet.cpp* and *src/celestialBody/planet/planet.h* to implement a **Planet** class that extends **CelestialBody**. It needs to have a variable *sun* that will reference an object of the **Sun** class. It needs to override the *color* variable to store a green color as a HEX string. It needs to override *translate()* to print "The planet is orbiting the sun..." and *rotate()* to print "Rotating the planet...". It needs to have a function *getSun()* that returns the *sun* variable.

Edit *src/main.cpp* to instantiate an object of the Asteroid, Sun and Planet classes and call all its functions.

Finally, make any changes to paths to compile the project correctly.