#pragma once
#include "ConstantGeometry.h"
#include "VirtualObject.h"
#include"DXGIPreset.h"
class Quad :
	public ConstantGeometry, public VirtualObject
{
private:
	ID3DX11EffectShaderResourceVariable* Texture[2];
public:
	Quad();
	virtual ~Quad();

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnRender() override;
	virtual void OnRelease() override;
};

