#pragma once
#include<d3d11.h>
#include<dxgi.h>
#include<vector>
#include"DXGIPreset.h"
#include"VirtualObject.h"

#include"Sphere.h"

class RenderManager :
	public VirtualObject
{
private:

	UINT Width;
	UINT Height;

	ID3D11RenderTargetView** RenderTargetView = new ID3D11RenderTargetView*[BUFFERCOUNT];
	D3D11_TEXTURE2D_DESC RenderTargetDesc{};
	D3D11_RENDER_TARGET_VIEW_DESC RenderTargetViewDesc{};


	ID3D11DepthStencilView* DepthStencilView;
	D3D11_TEXTURE2D_DESC DepthStencilDesc{};
	D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc{};

	std::vector<D3D11_INPUT_ELEMENT_DESC> DefaultInputLayout;

	D3D11_VIEWPORT MainViewport;

public:
	Sphere StaticSphere;

private:

	void SetInputElements();

public:

	_Out_ ID3D11RenderTargetView** GetRenderTargetView() { return RenderTargetView; }
	D3D11_TEXTURE2D_DESC* GetRenderTargetDesc() { return &RenderTargetDesc; }
	D3D11_RENDER_TARGET_VIEW_DESC* GetRenderTargetViewDesc() { return &RenderTargetViewDesc; }

	_Out_ ID3D11DepthStencilView** GetDepthStencilView() { return &DepthStencilView; }
	D3D11_TEXTURE2D_DESC* GetDepthStencilDesc() { return &DepthStencilDesc; }
	D3D11_DEPTH_STENCIL_VIEW_DESC* GetDepthStencilViewDesc() { return &DepthStencilViewDesc; }

	D3D11_VIEWPORT* GetViewport() { return &MainViewport; }

	RenderManager() {}
	RenderManager(_In_ ID3D11Device* InDevice, _In_ ID3D11DeviceContext* InContext, UINT BufferCount);
	virtual ~RenderManager() {}

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnRender() override;
	virtual void OnRelease() override;
};

