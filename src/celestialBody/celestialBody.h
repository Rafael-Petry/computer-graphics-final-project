#ifndef CELESTIAL_BODY_H
#define CELESTIAL_BODY_H

#include <string>

class CelestialBody {
public:
	virtual ~CelestialBody() {}

	virtual void translate() = 0;
	virtual void rotate() = 0;

	static const std::string& getModel();
	static const std::string& getColor();

protected:
	static const std::string model;
	static const std::string color;
};

#endif
