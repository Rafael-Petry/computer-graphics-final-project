#ifndef ASTEROID_H
#define ASTEROID_H

#include "../celestialBody.h"

class Asteroid : public CelestialBody {
public:
	void translate() override;
	void rotate() override;
};

#endif
