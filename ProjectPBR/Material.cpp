#include "stdafx.h"
#include "Material.h"


ID3D11ShaderResourceView* Material::LoadTexture(ID3D11Device * Device, LPCWSTR Path)
{
	ID3D11ShaderResourceView* SRV;
	HRESULT hr;
	D3DX11CreateShaderResourceViewFromFile(Device, Path, NULL, NULL, &SRV, &hr);

	if (FAILED(hr))
	{
		MessageBox(NULL, L"Failed to load texture.", 0, 0);
	}

	return SRV;
}

ID3D11ShaderResourceView * Material::LoadEnvironmentTexture(ID3D11Device * Device, LPCWSTR Path)
{
	ID3D11ShaderResourceView* SRV;
	HRESULT hr;

	ID3D11Texture2D* Tex = nullptr;

	D3DX11_IMAGE_LOAD_INFO Info{};

	Info.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	D3DX11CreateTextureFromFile(Device, Path, &Info, NULL, (ID3D11Resource**)&Tex,&hr);

	if (FAILED(hr))
	{
		MessageBox(NULL, L"Failed to load texture (file).", 0, 0);
	}

	D3D11_TEXTURE2D_DESC TexDesc{};
	Tex->GetDesc(&TexDesc);

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc{};
	SRVDesc.Format = TexDesc.Format;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	SRVDesc.TextureCube.MipLevels = TexDesc.MipLevels;
	SRVDesc.TextureCube.MostDetailedMip = 0;


	hr = Device->CreateShaderResourceView(Tex, &SRVDesc, &SRV);

	if (FAILED(hr))
	{
		MessageBox(NULL, L"Failed to load texture (srv).", 0, 0);
	}

	//D3DX11CreateShaderResourceViewFromFile(Device, Path, NULL, NULL, &SRV, &hr);

	//if (FAILED(hr))
	//{
	//	MessageBox(NULL, L"Failed to load texture.", 0, 0);
	//}

	return SRV;
}


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

	if(FAILED(GenerateEffect(Device, Path)))
	{
		MessageBox(NULL, L"Failed to generate effect", 0, 0);
		return false;
	}

	WorldMatrix = Shader->GetVariableByName("World")->AsMatrix();
	ViewMatrix = Shader->GetVariableByName("View")->AsMatrix();
	ProjectionMatrix = Shader->GetVariableByName("Projection")->AsMatrix();


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

	WorldMatrix = Shader->GetVariableByName("World")->AsMatrix();
	ViewMatrix = Shader->GetVariableByName("View")->AsMatrix();
	ProjectionMatrix = Shader->GetVariableByName("Projection")->AsMatrix();
	
	return true;
}

void Material::AddTextureResource(TextureResource * Texture)
{
	std::wstring str = Texture->GetPath();
	std::string path;
	std::wstring FileName;

	int t;

	for (UINT i = 0; i < str.size(); i++)
	{
		if (str[i] == L'\\' || str[i] == L'/')
		{
			t = i;
		}
	}

	for (UINT i = t + 1; i < str.size(); i++)
	{
		FileName.push_back(str[i]);
	}

	path.assign(FileName.begin(), FileName.end());
//	Textures->SetFileName(FileName.c_str());

	Textures.insert_or_assign(path, Texture);
}

Material::~Material()
{
}
