# Prompts

Este arquivo agrupa todos prompts executados por agentes de IA durante o desenvolvimento do trabalho. Na prática, cada prompt foi executado separademente pedindo para o agente ler o arquivo isolado do prompt correspondente em ***prompts/***, com comandos como: `Run the prompt on prompts/*.md`.

## 1-structure

Create a folder *_old/*. Read the current project and reestructure it in a way so that what is current on *src/* folder becomes *_old/src/*. Next, move the *data/* folder to *_old/src/*. Move *src/main.cpp* to *_old/src/*. Move *CMakeLists.txt*, *CMakePresets.json*, *Makefile*, *Makefile.macOS* and *.dir-locals.el* to *_old/* so that compilation only happens if it is run with *_old/* as the target. Also, move *COMPILACAO.md* to *_old/*.

Finally, move *lib-linux*, *lib-mingw-32*, *lib-migw-64*, *lib-ucrt-64*, *lib-vc2022* and *includes/* to a new folder *src/vendor*.

Make the necessary changes on files that references those old paths so the project runs correctly.

## 2-spaceship-init

On *src/spaceship/*, implement a Spaceship class editing the *src/spaceship/spaceship.cpp* and *src/spaceship/spaceship.h* files. The class needs to have a static variable called *model* referencing the *src/spaceship/spaceship.obj* file, that will store the model for this object. It needs to have a function called *translate()* that will print "Moving the spaceship...", a function called *rotate()* that will print "Rotating the spaceship", a function called *shoot()* that will print "The spaceship is shooting..." and a static function *getModel()* to return the *model* variable. 

Edit *src/spaceship/spaceship.obj* to store a simple cube model.

Edit *src/main.cpp* to instantiate an object of the Spaceship class and call all its functions.

Finally, make any changes to paths to compile the project correctly.