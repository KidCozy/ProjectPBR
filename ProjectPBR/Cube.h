#pragma once
#include "ConstantGeometry.h"
#include"VirtualObject.h"
class Cube :
	public ConstantGeometry, public VirtualObject
{
private:
	int Width;
	int Height;
	int Length;
public:

private:

public:
	Cube(int Width_, int Height_, int Length_) : Width(Width_), Height(Height_), Length(Length_) {}
	virtual ~Cube();


	// VirtualObject을(를) 통해 상속됨
	virtual void OnInit() override;

	virtual void OnUpdate() override;

	virtual void OnRender() override;

	virtual void OnRelease() override;

};

