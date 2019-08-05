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
	ID3DX11EffectShaderResourceVariable* Binormal = nullptr;
	ID3DX11EffectShaderResourceVariable* Tangent = nullptr;

	ID3DX11EffectVectorVariable* PixelOffset = nullptr;
protected:

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnRender() override;
	virtual void OnRelease() override;

public:

	void SetPosition(ID3D11ShaderResourceView* InPosition) { Position->SetResource(InPosition); }
	void SetNormal(ID3D11ShaderResourceView* InNormal) { Normal->SetResource(InNormal); }
	void SetBinormal(ID3D11ShaderResourceView* InBinormal) { Binormal->SetResource(InBinormal); }
	void SetTangent(ID3D11ShaderResourceView* InTangent) { Tangent->SetResource(InTangent); }
	void SetPixelOffset(XMVECTOR* Offset) { PixelOffset->SetFloatVector(Offset->m128_f32); }
	Quad();
	virtual ~Quad();


};

