#ifndef SUN_H
#define SUN_H

#include <string>

#include "../celestialBody.h"

class Sun : public CelestialBody {
public:
	Sun(const Sun&) = delete;
	Sun& operator=(const Sun&) = delete;

	static Sun& getInstance();

	void translate() override;
	void rotate() override;

	static const std::string color;

private:
	Sun() = default;
};

#endif
