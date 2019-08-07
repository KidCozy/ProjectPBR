#pragma once
#include<D3D11.h>
class TextureResource
{
protected:
	LPCWSTR Path = L"";
	LPCWSTR FileName = L"";
	
	ID3D11Texture2D* Texture = nullptr;
	ID3D11ShaderResourceView* SRV = nullptr;

public:

	void SetPath(LPCWSTR NewPath) { Path = NewPath; }
	void SetFileName(LPCWSTR NewFile) { FileName = NewFile; }

	LPCWSTR GetPath() const { return Path; }
	ID3D11ShaderResourceView* GetShaderResourceView() { return SRV; }

	void SetShaderResourceView(ID3D11ShaderResourceView* Object) { SRV = Object; }

	TextureResource();
	virtual ~TextureResource();
};

