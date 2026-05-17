# 17-planet-animation

Read _prompts/0-limitations_. Change the planets' translation method to use 2 cubic bezier curves around the sun instead. Change the Sun class to store its position and create a getPosition method that returns it. On planet, use the singleton sun to get the position and generate 4 points around it for the curves. It needs to be controlled by deltaTime.
