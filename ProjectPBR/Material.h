#pragma once
#pragma comment(lib,"Effects11d.lib")

#include<d3d11.h>
#include<D3DX11.h>
#include<D3Dcompiler.h>
#include<vector>
#include<d3dx11effect.h>

#include"Geometrics.h"

enum TECHNIQUES
{
	TECHNIQUE_GEOMETRY,
	TECHNIQUE_BUFFER,
};


class Material
{
protected:

	ID3DX11EffectMatrixVariable* WorldMatrix;
	ID3DX11EffectMatrixVariable* ViewMatrix;
	ID3DX11EffectMatrixVariable* ProjectionMatrix;

	ID3DX11Effect* Shader;

	ConstantBuffer ConstBuffer;

	D3DX11_PASS_DESC Pass;
	ID3D11InputLayout* InputLayout;
	ID3DX11EffectTechnique* Technique;
	ID3DX11EffectPass* CurrentPass;

	std::vector<ID3D11InputLayout*> InputLayouts;
	std::vector<ID3D11ShaderResourceView*> ShaderResources;
	std::vector<ID3DX11EffectVectorVariable*> VectorVariables;
	std::vector<ID3DX11EffectVariable*> Variables;
	std::vector<ID3DX11EffectPass*> Passes;
	std::vector<D3D11_INPUT_ELEMENT_DESC>* InputLayoutDesc;

	std::vector<ID3DX11EffectShaderResourceVariable*> MaterialShaderResources;

	LPCWSTR Path = L"DefaultShader.fx";

public:
	bool CompileShader(ID3D11DeviceContext* Context, ID3D11Device* Device, std::vector<D3D11_INPUT_ELEMENT_DESC> InputLayoutDesc, LPCWSTR Path);
	bool GenerateEffect(ID3D11Device* Device, LPCWSTR Path);

	void SetFile(LPCWSTR NewPath) { Path = NewPath; }
	void SetInputLayout(std::vector<D3D11_INPUT_ELEMENT_DESC>& InputLayout) { InputLayoutDesc = &InputLayout; }
	
	void SetWorldMatrix(const XMMATRIX& Mat) { WorldMatrix->SetMatrix((float*)&Mat); }
	void SetViewMatrix(const XMMATRIX& Mat) { ViewMatrix->SetMatrix((float*)&Mat); }
	void SetProjectionMatrix(const XMMATRIX& Mat) { ProjectionMatrix->SetMatrix((float*)&Mat); }

	void SetWorldMatrixPointer(ID3DX11EffectMatrixVariable* Matrix) { WorldMatrix = Matrix; }
	void SetViewMatrixPointer(ID3DX11EffectMatrixVariable* Matrix) { ViewMatrix = Matrix; }
	void SetProjectionMatrixPointer(ID3DX11EffectMatrixVariable* Matrix) { ProjectionMatrix = Matrix; }

	void SetTechnique(TECHNIQUES InTechnique) { Technique = Shader->GetTechniqueByIndex(InTechnique); }

	HRESULT SetPass(UINT Index) {
		if (Technique->GetPassByIndex(Index) == nullptr) return E_FAIL; CurrentPass = Shader->GetTechniqueByName("GeometryTech")->GetPassByIndex(Index); return S_OK; }

	ConstantBuffer* GetConstBuffer() { return &ConstBuffer; }
	ID3DX11Effect** GetEffectPointer() { return &Shader; }
	ID3DX11Effect* GetEffect() { return Shader; }

	ID3D11InputLayout*& GetInputLayout(UINT Index) { return InputLayouts[Index]; }


	std::vector<ID3D11InputLayout*>* GetInputLayouts() { return &InputLayouts; }
	std::vector<D3D11_INPUT_ELEMENT_DESC>* GetInputLayoutVector() { return InputLayoutDesc; }
	ID3DX11EffectTechnique* GetTechnique() { return Technique; }
	ID3DX11EffectTechnique* GetTechniqueByFromShader(TECHNIQUES Index) { if (Shader->GetTechniqueByIndex(Index)->IsValid()) return Shader->GetTechniqueByIndex(Index); }
	ID3DX11EffectPass** GetPassPointer() { return &CurrentPass; }
	ID3DX11EffectPass* GetPassFromTechnique(UINT Index) { if (Technique->GetPassByIndex(Index) != nullptr) return Technique->GetPassByIndex(Index); return nullptr; }

	
	std::vector<ID3D11ShaderResourceView*>* GetShaderResourceContainer() { return &ShaderResources; }

	void SetShaderResourceView(ID3DX11EffectShaderResourceVariable* Object, ID3D11ShaderResourceView* InSRV) { if(InSRV!=nullptr) Object->SetResource(InSRV); }

	void AddTextureResource(ID3D11ShaderResourceView* Texture);


	LPCWSTR GetPath() { return Path; }

	Material() {}
	Material(Transform* TransformPtr) { ConstBuffer.World = XMLoadFloat4x4(&TransformPtr->GetTransform()); }
	virtual ~Material();
};

