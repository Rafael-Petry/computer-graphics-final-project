#ifndef PLANET_H
#define PLANET_H

#include <string>

#include "../celestialBody.h"
#include "../sun/sun.h"

class Planet : public CelestialBody {
public:
	void translate() override;
	void rotate() override;

	Sun& getSun();
	const Sun& getSun() const;

	static const std::string color;

private:
	Sun sun;
};

#endif
