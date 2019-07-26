#pragma once
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "Effects11d.lib")

#include<d3d11.h>
#include<D3DX11.h>
#include<dxgi.h>
#include<d3dx11effect.h>
#include<D3Dcompiler.h>

#include<Windows.h>
#include<sal.h>
#include<vector>

#include"DXGIPreset.h"
#include"Geometrics.h"
#include"Material.h"

class ConstantGeometry;
class D3DHelper
{
private:
	UINT Width;
	UINT Height;
	UINT SampleCount = 1;
	UINT BufferCount = BUFFERCOUNT;

	bool SampleQuality = 0;

	DXGI_FORMAT RenderTargetFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
	

	ID3D11Device* Device;
	ID3D11DeviceContext* Context;
	ID3D11RasterizerState* RasterizerState;
	IDXGISwapChain* SwapChain;

private:
	bool CreateViewport(UINT Width, UINT Height, D3D11_VIEWPORT* OutViewport);
	bool GetMSAAFeature();
	void SetSampleCount(UINT Count) { SampleCount = Count; }

public:
	
	UINT GetSampleCount() const { return SampleCount; }
	UINT GetBufferCount() const { return BufferCount; }

	bool GetSampleQuality() const { return SampleQuality; }

	bool CreateDXGI(_In_ HWND* hWnd, _Out_ IDXGIFactory* Factory,
		UINT Width, UINT Height, _Out_ IDXGISwapChain** OutSwapChain, _Out_ D3D11_VIEWPORT* Viewport);

	bool CreateDevice(_In_ ID3D11Device** OutDevice, _In_ ID3D11DeviceContext** OutContext);

	bool CreateRenderTarget(_In_ IDXGISwapChain* SwapChain, _Out_ ID3D11Texture2D** RenderTarget, _In_ D3D11_TEXTURE2D_DESC* RenderTargetDesc);
	bool CreateDepthStencil(_Out_ ID3D11Texture2D** DepthStencil, _In_ D3D11_TEXTURE2D_DESC* DepthStencilDesc);

	bool CreateRenderTargetView(_Out_ ID3D11RenderTargetView** RenderTargetView, _In_ D3D11_RENDER_TARGET_VIEW_DESC* RenderTargetViewDesc);
	bool CreateDepthStencilView(_Out_ ID3D11DepthStencilView** DepthStencilView, _In_ D3D11_DEPTH_STENCIL_VIEW_DESC* DepthStencilViewDesc);

	static bool AllocConstantBuffer(ID3D11Device* Device, BaseBuffer* BaseBuffer, std::vector<Vertex>* VContainer, std::vector<WORD>* IContainer);
	static bool GenerateEffect(ID3D11Device* Device, Material* Resource);
	static bool CompileShader(ID3D11Device* Device, Material* Resource, ID3DX11EffectTechnique* Technique);


	ID3D11Device* GetDevice() { return Device; }
	ID3D11DeviceContext* GetContext() { return Context; }

	D3DHelper() {}
	D3DHelper(ID3D11Device* NewDevice, ID3D11DeviceContext* NewContext,IDXGISwapChain* NewSwapChain, UINT InWidth, UINT InHeight) : 
		Device(NewDevice), Context(NewContext), SwapChain(NewSwapChain), Width(InWidth), Height(InHeight){}
	~D3DHelper(){}
};