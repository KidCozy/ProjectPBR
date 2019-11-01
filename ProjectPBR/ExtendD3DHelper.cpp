#include "ExtendD3DHelper.h"

D3D11_TEXTURE2D_DESC ExtendD3DHelper::GenerateTexture2DDescriptor(DXGI_FORMAT Format, UINT BindFlags, UINT CPUAccess)
{
	D3D11_TEXTURE2D_DESC Desc = CD3D11_TEXTURE2D_DESC();

	Desc.Width = WINDOW_WIDTH;
	Desc.Height = WINDOW_HEIGHT;
	
	Desc.Format = Format;
	Desc.BindFlags = BindFlags;
	Desc.CPUAccessFlags = CPUAccess;

	return Desc;
}

ID3D11Texture2D* ExtendD3DHelper::CreateTexture2D(LPDEVICE, D3D11_TEXTURE2D_DESC & Desc)
{
	ID3D11Texture2D* Texture;
	Device->CreateTexture2D(&Desc, nullptr, &Texture);

	return Texture;
}

ID3D11RenderTargetView* ExtendD3DHelper::CreateRenderTargetView(LPDEVICE, ID3D11Texture2D ** Texture, D3D11_RENDER_TARGET_VIEW_DESC* Desc)
{
	ID3D11RenderTargetView* RTV;
	HRESULT hr;
	if (Desc == nullptr)
		hr = Device->CreateRenderTargetView(*Texture, nullptr, &RTV);
	else
		hr = Device->CreateRenderTargetView(*Texture, Desc, &RTV);

	

	return RTV;

}

ID3D11ShaderResourceView* ExtendD3DHelper::CreateShaderResourceView(LPDEVICE, ID3D11Texture2D * Texture, D3D11_SHADER_RESOURCE_VIEW_DESC * Desc)
{
	ID3D11ShaderResourceView* SRV;
	Device->CreateShaderResourceView(Texture, Desc, &SRV);
	
	return SRV;
}

bool ExtendD3DHelper::Resize(LPDEVICE, LPSWAPCHAIN, UINT Width, UINT Height, ID3D11RenderTargetView*& SwapChainBuffer, RTTexture * GBuffer, GBufferDescription * Descriptor)
{
	ID3D11Texture2D* BackBuffer = nullptr;

	SUCCESS(SwapChain->ResizeBuffers(1, Width, Height, DXGI_FORMAT_R8G8B8A8_UNORM, 0));

	SUCCESS(SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer));

	SUCCESS(Device->CreateRenderTargetView(BackBuffer, 0, &SwapChainBuffer));




	return true;
}

ID3D11DepthStencilView * ExtendD3DHelper::GenerateDepthStencilView(LPDEVICE)
{
	ID3D11DepthStencilView* DSV;
	ID3D11Texture2D* Texture;
	D3D11_TEXTURE2D_DESC Desc;

	Desc = GenerateTexture2DDescriptor(DXGI_FORMAT_R24G8_TYPELESS, D3D11_BIND_DEPTH_STENCIL, 0);

	SUCCESS(Device->CreateTexture2D(&Desc, nullptr, &Texture));
	SUCCESS(Device->CreateDepthStencilView(Texture, nullptr, &DSV));

	
	return nullptr;
}
