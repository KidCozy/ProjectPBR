#pragma once
#include "ConstantGeometry.h"
#include "VirtualObject.h"

class Sphere :
	public ConstantGeometry, public VirtualObject
{
protected:
	float Radius;
	UINT Slice;
	UINT Stack;



protected:
	_inline void SetRadius(float InRadius) { Radius = InRadius; }
	_inline void SetSlice(float InSlice) { Slice = InSlice; }
	_inline void SetStack(float InStack) { Stack = InStack; }

	virtual void OnInit();
	virtual void OnUpdate();
	virtual void OnRender();
	virtual void OnRelease();
public:

	void SetProperty(float InRadius, UINT Slice, UINT Stack);

	float GetRadius() { return Radius; }
	float GetSlice() { return Slice; }
	float GetStack() { return Stack; }

	Sphere();
	virtual ~Sphere();
};

