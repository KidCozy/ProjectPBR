#include "stdafx.h"
#include "Geometry.h"


void Geometry::AddTexture(ID3D11Device* Device, LPCWSTR Path)
{
	std::wstring P = std::wstring(RELATIVE_ROOT) + Path;

	TextureResource* Tex = new TextureResource();

	Tex->SetPath(Path);

	Tex->SetShaderResourceView(Material::LoadTexture(Device, P.c_str()));
	

	GetMaterial()->AddTextureResource(Tex);

}

void Geometry::AddEnvironmentTexture(ID3D11Device* Device, LPCWSTR Path)
{
	std::wstring P = std::wstring(RELATIVE_ROOT) + Path;

	TextureResource* Tex = new TextureResource();

	Tex->SetPath(Path);

	Tex->SetShaderResourceView(Material::LoadEnvironmentTexture(Device, P.c_str()));


	GetMaterial()->AddTextureResource(Tex);

}

Geometry::Geometry()
{
}


Geometry::~Geometry()
{
}
