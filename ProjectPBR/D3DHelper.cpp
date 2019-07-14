#include "D3DHelper.h"


bool D3DHelper::CreateDXGI(_In_ HWND hWnd, _Out_ IDXGIFactory* Factory,
	UINT Width, UINT Height, _Out_ IDXGISwapChain* OutSwapChain)
{
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;

	bool MSAA = GetMSAAFeature();
	
	SwapChainDesc.BufferCount = 1;
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChainDesc.BufferDesc.Height = Height;
	SwapChainDesc.BufferDesc.Width = Width;
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	SwapChainDesc.SampleDesc.Count = SampleCount;
	SwapChainDesc.SampleDesc.Quality = MSAA ? 1 : 0;

	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	SwapChainDesc.Windowed = true;
	SwapChainDesc.OutputWindow = hWnd;
	SwapChainDesc.Flags = 0;
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_SEQUENTIAL;
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	
	if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&Factory))) {
		MessageBox(NULL, L"Failed to create dxgi factory", 0, 0);
		exit(-1);
	}

	if (FAILED(Factory->CreateSwapChain(Device, &SwapChainDesc, &OutSwapChain))) {
		MessageBox(NULL, L"Failed to create Swap-chain", 0, 0);
		exit(-1);
	}

	return true;
}

bool D3DHelper::CreateDevice(ID3D11Device * OutDevice, ID3D11DeviceContext * OutContext)
{
	D3D_FEATURE_LEVEL Levels[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,

	};

	if (FAILED(D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, nullptr, 0, D3D11_SDK_VERSION, &Device, &Levels[0], &OutContext))) {
		MessageBox(NULL, L"Failed to create D3D Device.", 0, 0);
		exit(-1);
	}

	return true;
}

bool D3DHelper::CreateViewport(UINT Width, UINT Height, D3D11_VIEWPORT* OutViewport)
{
	D3D11_VIEWPORT V;

	V.Height = Height;
	V.Width = Width;
	V.MinDepth = 0.0f;
	V.MaxDepth = 1.0f;
	V.TopLeftX = 0;
	V.TopLeftY = 0;

	*OutViewport = V;

	return true;
}

bool D3DHelper::GetMSAAFeature()
{
	UINT Quality;
	if(FAILED(Device->CheckMultisampleQualityLevels(TextureFormat, SampleCount, &Quality)))
		return false;

	return true;
}

bool D3DHelper::CreateRenderTarget(_Out_ ID3D11Texture2D ** RenderTarget, _Out_ ID3D11RenderTargetView ** RenderTargetView)
{


	D3D11_TEXTURE2D_DESC TextureDesc;
	TextureDesc.ArraySize = 1;
	TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	TextureDesc.Height = Height;
	TextureDesc.Width = Width;
	TextureDesc.MipLevels = 1;
	TextureDesc.MiscFlags = 0;
	TextureDesc.SampleDesc.Count = 1;

	if (FAILED(Device->CreateTexture2D(&TextureDesc, NULL, RenderTarget)))
	{
		MessageBox(NULL, L"Failed to create render target texture.", 0, 0);
		exit(-1);
	}

	return true;
}
