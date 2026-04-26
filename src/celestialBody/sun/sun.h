#ifndef SUN_H
#define SUN_H

#include <string>

#include "../celestialBody.h"

class Sun : public CelestialBody {
public:
	void translate() override;
	void rotate() override;

	static const std::string color;
};

#endif
