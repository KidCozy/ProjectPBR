#include "stdafx.h"
#include "Material.h"


bool Material::CompileShader(ID3D11DeviceContext* Context, ID3D11Device* Device,std::vector<D3D11_INPUT_ELEMENT_DESC> InputLayoutDesc, LPCWSTR Path)
{
	ID3DBlob* VertexBlob,* PixelBlob, * ErrorBlob;

	HRESULT hr;

	D3DX11CompileFromFile(Path, NULL, NULL, "VS", "vs_4_0",
		D3DCOMPILE_ENABLE_STRICTNESS, 0, NULL, &VertexBlob, &ErrorBlob, &hr);

	if (FAILED(hr))
		MessageBox(NULL, L"Failed to compile vertex shader", 0, 0);

	D3DX11CompileFromFile(Path, NULL, NULL, "PS", "ps_4_0",
		D3DCOMPILE_ENABLE_STRICTNESS, 0, NULL, &PixelBlob, &ErrorBlob, &hr);
	
	if (FAILED(hr))
		MessageBox(NULL, L"Failed to compile pixel shader", 0, 0);

	if (FAILED(Device->CreateVertexShader(VertexBlob->GetBufferPointer(), VertexBlob->GetBufferSize(), NULL, &VS))) 
	{
		MessageBox(NULL, L"Failed to create vertex shader", 0, 0);
	}
	if(FAILED(Device->CreatePixelShader(PixelBlob->GetBufferPointer(), PixelBlob->GetBufferSize(), NULL, &PS)))
	{
		MessageBox(NULL, L"Failed to create pixel shader", 0, 0);
	}

	if (FAILED(Device->CreateInputLayout(InputLayoutDesc.data(), InputLayoutDesc.size(), 
		VertexBlob->GetBufferPointer(), VertexBlob->GetBufferSize(), &InputLayout)))
	{
		MessageBox(NULL, L"Failed to create input layout", 0, 0);
	}

	Context->IASetInputLayout(InputLayout);

	VertexBlob->Release();
	PixelBlob->Release();
	return true;
}

Material::Material()
{
}


Material::~Material()
{
}
