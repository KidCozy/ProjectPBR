#pragma once
#include"Ray.h"
class Collider
{
protected:

public:

	virtual bool Hit(const Ray& ray, float Min, float Max) const = 0;

	Collider();
	~Collider();
};

