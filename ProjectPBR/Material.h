#pragma once
#pragma comment(lib,"Effects11d.lib")

#include<d3d11.h>
#include<D3DX11.h>
#include<D3Dcompiler.h>
#include<vector>
#include<d3dx11effect.h>


#include"Geometrics.h"


class Material
{
protected:

	ID3DX11Effect* Shader;

	ID3D11VertexShader* VS;
	ID3D11PixelShader* PS;

	ConstantBuffer ConstBuffer;

	D3DX11_PASS_DESC Pass;
	ID3D11InputLayout* InputLayout;

	std::vector<ID3DX11EffectTechnique*> TechVector;
	std::vector<ID3D11ShaderResourceView*> ShaderResources;
	std::vector<D3D11_INPUT_ELEMENT_DESC>* InputLayoutDesc;

	LPCWSTR Path;

public:
	bool CompileShader(ID3D11DeviceContext* Context, ID3D11Device* Device, std::vector<D3D11_INPUT_ELEMENT_DESC> InputLayoutDesc, LPCWSTR Path);
	bool GenerateEffect(ID3D11Device* Device, LPCWSTR Path);
	bool AddTexture2DResource(ID3D11Device* Device, ID3D11Texture2D* Resource, D3D11_SHADER_RESOURCE_VIEW_DESC& SRVDesc);

	void SetInputLayout(std::vector<D3D11_INPUT_ELEMENT_DESC>& InputLayout) { InputLayoutDesc = &InputLayout; }
	void SetFile(LPCWSTR NewPath) { Path = NewPath; }

	ConstantBuffer* GetConstBuffer() { return &ConstBuffer; }
	ID3DX11Effect** GetEffectPointer() { return &Shader; }
	ID3DX11Effect* GetEffect() { return Shader; }

	ID3D11VertexShader** GetVertexShader() { return &VS; }
	ID3D11PixelShader** GetPixelShader() { return &PS; }
	ID3D11InputLayout*& GetInputLayout() { return InputLayout; }

	std::vector<D3D11_INPUT_ELEMENT_DESC>* GetInputLayoutVector() { return InputLayoutDesc; }
	std::vector<ID3DX11EffectTechnique*>* GetTechniqueVector() { return &TechVector; }

	LPCWSTR GetPath() { return Path; }

	Material() {}
	Material(Transform* TransformPtr) { ConstBuffer.World = XMLoadFloat4x4(&TransformPtr->GetTransform()); }
	virtual ~Material();
};

