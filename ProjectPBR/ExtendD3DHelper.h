#pragma once
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
#include<d3dcompiler.h>

#include<Windows.h>
#include<sal.h>
#include<vector>

#include"Utility.h"
#include"DXGIPreset.h"
#include"Geometrics.h"
#include"Geometry.h"
#include"Material.h"


#define LPDEVICE ID3D11Device* Device
#define LPCONTEXT ID3D11DeviceContext* Context
#define LPSWAPCHAIN IDXGISwapChain* SwapChain

class ExtendD3DHelper
{
public:

	static D3D11_TEXTURE2D_DESC GenerateTexture2DDescriptor(DXGI_FORMAT Format, UINT BindFlags, UINT CPUAccess);

	static ID3D11Texture2D* CreateTexture2D(LPDEVICE, D3D11_TEXTURE2D_DESC& Desc);
	static ID3D11RenderTargetView* CreateRenderTargetView(LPDEVICE, ID3D11Texture2D** Texture, D3D11_RENDER_TARGET_VIEW_DESC* Desc);
	static ID3D11ShaderResourceView* CreateShaderResourceView(LPDEVICE, ID3D11Texture2D* Texture, D3D11_SHADER_RESOURCE_VIEW_DESC* Desc);

	static bool Resize(LPDEVICE, LPSWAPCHAIN, UINT Width, UINT Height, ID3D11RenderTargetView*& SwapChainBuffer, RTTexture* GBuffer, GBufferDescription* Descriptor);
	static ID3D11DepthStencilView* GenerateDepthStencilView(LPDEVICE);

};

