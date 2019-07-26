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

	if(FAILED(GenerateEffect(Device, Path)))
	{
		MessageBox(NULL, L"Failed to generate effect", 0, 0);
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

bool Material::GenerateEffect(ID3D11Device* Device, LPCWSTR Path)
{
	ID3DBlob* ErrBlob;

	if (FAILED(D3DX11CompileEffectFromFile(Path, nullptr, nullptr,
		D3DCOMPILE_ENABLE_STRICTNESS, 0, Device, &Shader, &ErrBlob)))
	{
		return false;
	}


	//if(FAILED(D3DX11Create(Path, D3DCOMPILE_ENABLE_STRICTNESS, Device, &Shader)))
	//	return false;
	
	
	return true;
}

bool Material::AddTexture2DResource(ID3D11Device* Device, ID3D11Texture2D* Resource, D3D11_SHADER_RESOURCE_VIEW_DESC& SRVDesc)
{

	if (FAILED(Device->CreateShaderResourceView(Resource, &SRVDesc, &ShaderResources[ShaderResources.size()]))) {
		MessageBox(NULL, L"Failed to create srv", 0, 0);
		return false;
	}

	return true;
}

Material::~Material()
{
}
