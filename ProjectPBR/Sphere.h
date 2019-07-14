#pragma once
#include "ConstantGeometry.h"
#include "VirtualObject.h"
class Sphere :
	public ConstantGeometry, public VirtualObject
{
private:
	float Radius;
	UINT Slice;
	UINT Stack;

private:
	_inline void SetRadius(float InRadius) { Radius = InRadius; }
	_inline void SetSlice(float InSlice) { Slice = InSlice; }
	_inline void SetStack(float InStack) { Stack = InStack; }

protected:
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

