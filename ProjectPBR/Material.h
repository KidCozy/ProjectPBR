#pragma once

#include<d3d11.h>
#include<D3DX11.h>
#include<D3Dcompiler.h>
#include<vector>
#include"Geometrics.h"

class Material
{
protected:
	ID3D11VertexShader* VS;
	ID3D11PixelShader* PS;

	ConstantBuffer ConstBuffer;

	ID3D11InputLayout* InputLayout;

public:

	bool CompileShader(ID3D11DeviceContext* Context, ID3D11Device* Device, std::vector<D3D11_INPUT_ELEMENT_DESC> InputLayoutDesc, LPCWSTR Path);
	ConstantBuffer* GetConstBuffer() { return &ConstBuffer; }
	ID3D11VertexShader** GetVertexShader() { return &VS; }
	ID3D11PixelShader** GetPixelShader() { return &PS; }

	
	Material(Transform* TransformPtr) { ConstBuffer.World = XMLoadFloat4x4(&TransformPtr->GetTransform()); }
	virtual ~Material();
};

