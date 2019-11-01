#pragma once
#include<DirectXMath.h>

#define BUFFERCOUNT 8
#define ASPECT_RATIO 1.777f
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define SWAPCHAIN_FORMAT DXGI_FORMAT_R8G8B8A8_UNORM

struct D3D11_RENDER_TARGET_VIEW_DESC;
struct D3D11_DEPTH_STENCIL_VIEW_DESC;

using namespace DirectX;

struct RTTexture
{
	ID3D11Texture2D* Texture = NULL;
	ID3D11RenderTargetView* RTV = NULL;
	ID3D11ShaderResourceView* SRV = NULL;

};

struct GBufferVariables
{
	XMVECTOR ViewportDimensions;
};

struct GBufferDescription
{
	D3D11_RENDER_TARGET_VIEW_DESC RTVDesc{};
	D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc{};
	D3D11_DEPTH_STENCIL_DESC DepthStencilInfoDesc{};
	D3D11_TEXTURE2D_DESC RenderTargetDesc{};
	D3D11_TEXTURE2D_DESC DepthStencilDesc{};
	D3D11_TEXTURE2D_DESC RayBufferDesc{};
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc{};
	D3D11_SHADER_RESOURCE_VIEW_DESC DepthSRVDesc{};
	DXGI_SAMPLE_DESC SamplerDesc{};

};