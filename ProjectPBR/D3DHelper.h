#pragma once
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include<d3d11.h>
#include<dxgi.h>
#include<Windows.h>
#include<sal.h>

class ConstantGeometry;

class D3DHelper
{
private:
	UINT Width;
	UINT Height;
	UINT SampleCount = 1;

	DXGI_FORMAT TextureFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
	

	ID3D11Device* Device;

private:

	bool CreateViewport(UINT Width, UINT Height, D3D11_VIEWPORT* OutViewport);

	bool GetMSAAFeature();

	void SetSampleCount(UINT Count) { SampleCount = Count; }
public:
	
	bool CreateDXGI(_In_ HWND hWnd, _Out_ IDXGIFactory* Factory,
		UINT Width, UINT Height, _Out_ IDXGISwapChain* OutSwapChain);

	bool CreateDevice(_Out_ ID3D11Device* OutDevice, _Out_ ID3D11DeviceContext* OutContext);

	bool CreateRenderTarget(_Out_ ID3D11Texture2D** RenderTarget, _Out_ ID3D11RenderTargetView** RenderTargetView);

	bool AllocConstantBuffer(ID3D11Device& InDevice, ConstantGeometry& Object);

	D3DHelper() {}
	D3DHelper(ID3D11Device* NewDevice, UINT InWidth, UINT InHeight) : Device(NewDevice), Width(InWidth), Height(InHeight){}
	~D3DHelper(){}
};