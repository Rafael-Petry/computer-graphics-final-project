#ifndef PLANET_H
#define PLANET_H

#include <string>

#include "../celestialBody.h"

class Planet : public CelestialBody {
public:
	void translate() override;
	void rotate() override;
	const std::string& getColor() const override;

	static const std::string color;
};

#endif
