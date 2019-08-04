#pragma once
#include "ConstantGeometry.h"
#include "VirtualObject.h"
#include"DXGIPreset.h"
class Quad :
	public ConstantGeometry, public VirtualObject
{
private:

	ID3DX11EffectShaderResourceVariable* Position = nullptr;
	ID3DX11EffectShaderResourceVariable* Normal = nullptr;

protected:

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnRender() override;
	virtual void OnRelease() override;

public:

	void SetPosition(ID3D11ShaderResourceView* InPosition) { Position->SetResource(InPosition); }
	void SetNormal(ID3D11ShaderResourceView* InNormal) { Normal->SetResource(InNormal); }

	Quad();
	virtual ~Quad();


};

