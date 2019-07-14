#pragma once
#include "Geometry.h"
class ConstantGeometry :
	public Geometry
{
protected:
	BaseBuffer* Buffer;
	
public:
	ConstantGeometry();
	virtual ~ConstantGeometry();
};

