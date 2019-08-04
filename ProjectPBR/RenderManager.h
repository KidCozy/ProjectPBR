#pragma once
#include<d3d11.h>
#include<dxgi.h>
#include<vector>
#include<DirectXColors.h>
#include"DXGIPreset.h"
#include"VirtualObject.h"
#include"D3DHelper.h"
#include"Sphere.h"
#include"Camera.h"
#include"Quad.h"

#define DEFAULT_MATERIALSIZE 2

enum RENDERBUFFER
{
	RENDERBUFFER_GEOMETRY,
	RENDERBUFFER_MERGE,
	RENDERBUFFER_DEBUG,
};

class RenderManager :
	public VirtualObject
{
private:

	UINT Width;
	UINT Height;

	ID3D11RenderTargetView* MergeBuffer = nullptr;

	RTTexture GBuffer[BUFFERCOUNT] = { nullptr };
	GBufferDescription GBufferDesc;
	D3D11_TEXTURE2D_DESC RenderTargetDesc;

	ID3D11DepthStencilView* DepthStencilView = nullptr;
	D3D11_TEXTURE2D_DESC DepthStencilDesc{};

	std::vector<Material*> MaterialContainer;

	D3D11_VIEWPORT MainViewport;

	ID3DX11EffectTechnique* SelectTech;

public:
	Sphere StaticSphere;
	Quad ScreenQuad;
	Camera StaticCamera = Camera(ASPECT_RATIO, XM_PIDIV2, 0.01f, 100.0f);

	XMMATRIX World;

private:
	void SetInputElements();
	void SetMaterialPath();

	void SetPass(Geometry& Object, UINT Index);
	void SetTechnique(ID3DX11EffectTechnique* TargetTech) { if (TargetTech != nullptr) SelectTech = TargetTech; }

protected:

	virtual void PostInitialize() override;

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnRender() override;
	virtual void OnRelease() override;

public:

	void SetWidth(UINT NewWidth) { Width = NewWidth; }
	void SetHeight(UINT NewHeight) { Height = NewHeight; }

	void ClearScreen(XMVECTORF32 ClearColor);

	void BindBuffer(Geometry* Geometry);

	void DrawObject(Geometry* Object, RENDERBUFFER RenderBuffer);


	ID3D11RenderTargetView* GetMergeBuffer() { return MergeBuffer; }
	ID3D11RenderTargetView** GetMergeBufferPointer() { return &MergeBuffer; }
	RTTexture* GetGBufferPointer() { return GBuffer; }
	GBufferDescription* GetGBufferDescriptor() { return &GBufferDesc; }

	_Out_ ID3D11RenderTargetView** GetRenderTargetViewPointer() { return &GBuffer[0].RTV; }
	D3D11_TEXTURE2D_DESC* GetRenderTargetDesc() { return &GBufferDesc.RenderTargetDesc; }
	D3D11_RENDER_TARGET_VIEW_DESC* GetRenderTargetViewDesc() { return &GBufferDesc.RTVDesc; }

	_Out_ ID3D11DepthStencilView** GetDepthStencilViewPointer() { return &DepthStencilView; }
	_Out_ ID3D11DepthStencilView* GetDepthStencilView() { return DepthStencilView; }
	D3D11_TEXTURE2D_DESC* GetDepthStencilDesc() { return &GBufferDesc.DepthStencilDesc; }
	D3D11_DEPTH_STENCIL_VIEW_DESC* GetDepthStencilViewDesc() { return &GBufferDesc.DSVDesc; }

	D3D11_DEPTH_STENCIL_DESC* GetDepthStencilInfoDesc() { return &GBufferDesc.DepthStencilInfoDesc; }
		
	D3D11_VIEWPORT* GetViewport() { return &MainViewport; }



	RenderManager() {}
	RenderManager(UINT InWidth, UINT InHeight, _In_ ID3D11Device* InDevice, _In_ ID3D11DeviceContext* InContext, UINT BufferCount);
	virtual ~RenderManager() {}

};

