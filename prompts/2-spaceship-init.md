# 2-spaceship-init

On *src/spaceship/*, implement a Spaceship class editing the *src/spaceship/spaceship.cpp* and *src/spaceship/spaceship.h* files. The class needs to have a static variable called *model* referencing the *src/spaceship/spaceship.obj* file, that will store the model for this object. It needs to have a function called *translate()* that will print "Moving the spaceship...", a function called *rotate()* that will print "Rotating the spaceship", a function called *shoot()* that will print "The spaceship is shooting..." and a static function *getModel()* to return the *model* variable. 

Edit *src/spaceship/spaceship.obj* to store a simple cube model.

Edit *src/main.cpp* to instantiate an object of the Spaceship class and call all its functions.

Finally, make any changes to paths to compile the project correctly.