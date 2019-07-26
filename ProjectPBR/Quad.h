#pragma once
#include "ConstantGeometry.h"
#include "VirtualObject.h"
class Quad :
	public ConstantGeometry, public VirtualObject
{
private:
	ID3DX11EffectVariable* Texture;

public:
	Quad();
	virtual ~Quad();

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnRender() override;
	virtual void OnRelease() override;
};

