#ifndef COLLIDER_H
#define COLLIDER_H

class Collider
{
public:
    virtual ~Collider() = default;
    virtual bool isInitialized() const = 0;
};

#endif
