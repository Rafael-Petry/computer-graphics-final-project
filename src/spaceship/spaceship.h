#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <string>

class Spaceship
{
public:
	void render() const;
	void translate() const;
	void rotate() const;
	void shoot() const;

	static const std::string& getModel();

private:
	static const std::string model;
};

#endif
