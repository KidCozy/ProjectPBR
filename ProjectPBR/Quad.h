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
	ID3DX11EffectShaderResourceVariable* Depth = nullptr;
	ID3DX11EffectShaderResourceVariable* Color = nullptr;
	ID3DX11EffectShaderResourceVariable* Albedo = nullptr;
	ID3DX11EffectShaderResourceVariable* Visible = nullptr;


	ID3DX11EffectVectorVariable* PixelOffset = nullptr;
	ID3DX11EffectVectorVariable* ViewPosition = nullptr;
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
	void SetDepth(ID3D11ShaderResourceView* InDepth) { Depth->SetResource(InDepth); }
	void SetColor(ID3D11ShaderResourceView* InColor) { Color->SetResource(InColor); }
	void SetAlbedo(ID3D11ShaderResourceView* InAlbedo) { Albedo->SetResource(InAlbedo); }
	void SetRay(ID3D11ShaderResourceView* InRay) { Visible->SetResource(InRay); }

	void SetPixelOffset(XMVECTOR* Offset) { PixelOffset->SetFloatVector(Offset->m128_f32); }
	Quad();
	virtual ~Quad();


};

