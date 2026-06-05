# 27-asteroid-size

Make it so the asteroids are able to have different sizes (small, medium and large) and multiply its scaleValue by a factor of this size.

Smaller asteroids need to be faster than larger ones. Add a property enableRespawn to the asteroid. All initial asteroids need to have this property set to true.

Asteroids that have enableRespawn = false are still destructible.

Destroying an asteroid needs to also spawn 3 asteroids of a smaller size if it is not a small asteroid. The new asteroids have enableRespawn set to false and are generated around the destroyed asteroid.
