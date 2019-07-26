#include "D3DHelper.h"


bool D3DHelper::CreateDXGI(_In_ HWND* hWnd, _Out_ IDXGIFactory* Factory,
	UINT Width, UINT Height, _Out_ IDXGISwapChain** OutSwapChain, _Out_ D3D11_VIEWPORT* Viewport)
{
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;

	bool MSAA = GetMSAAFeature();

	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.BufferCount = BufferCount;
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChainDesc.BufferDesc.Height = Height;
	SwapChainDesc.BufferDesc.Width = Width;
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	SwapChainDesc.SampleDesc.Count = SampleCount;
	SwapChainDesc.SampleDesc.Quality = SampleQuality; // Need to check out this point when rendering completed.

	SwapChainDesc.Windowed = true;
	SwapChainDesc.OutputWindow = *hWnd;
	SwapChainDesc.Flags = 0;
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_SEQUENTIAL;

	
	if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&Factory))) {
		MessageBox(NULL, L"Failed to create dxgi factory", 0, 0);
		exit(-1);
	}

	if (FAILED(Factory->CreateSwapChain(Device, &SwapChainDesc, &SwapChain))) {
		MessageBox(NULL, L"Failed to create Swap-chain", 0, 0);
		exit(-1);
	}

	*OutSwapChain = SwapChain;

	CreateViewport(Width, Height, Viewport);

	return true;
}

bool D3DHelper::CreateDevice(ID3D11Device** OutDevice, ID3D11DeviceContext** OutContext)
{
	D3D_FEATURE_LEVEL Levels[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,

	};

	if (FAILED(D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, nullptr, 0, D3D11_SDK_VERSION, &Device, &Levels[0], &Context))) {
		MessageBox(NULL, L"Failed to create D3D Device.", 0, 0);
		exit(-1);
	} 

	*OutDevice = Device;
	*OutContext = Context;

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

	D3D11_RASTERIZER_DESC RasterizerDesc = {};

	RasterizerDesc.CullMode = D3D11_CULL_BACK;
	RasterizerDesc.AntialiasedLineEnable = false;
	RasterizerDesc.DepthClipEnable = false;
	RasterizerDesc.FillMode = D3D11_FILL_SOLID;

	Device->CreateRasterizerState(&RasterizerDesc, &RasterizerState);
	Context->RSSetState(RasterizerState);
	Context->RSSetViewports(1, OutViewport);

	return true;
}

bool D3DHelper::GetMSAAFeature()
{
	UINT Quality;

	for (UINT i = 4; i > 0; i /= 2) {
		if (FAILED(Device->CheckMultisampleQualityLevels(RenderTargetFormat, i, &Quality)))
			continue;
		SampleCount = i;
		break;
	}

	return SampleCount == 1 ? 0 : SampleCount;
}

bool D3DHelper::CreateRenderTarget(_In_ IDXGISwapChain* SwapChain, _Out_ ID3D11Texture2D ** RenderTarget, _In_ D3D11_TEXTURE2D_DESC* RenderTargetDesc)
{
	for (UINT i = 0; i < BufferCount; i++)
	{
		if (FAILED(SwapChain->GetBuffer(i, __uuidof(ID3D11Texture2D), (void**)&RenderTarget[i])))
		{
			MessageBox(NULL, L"Failed to create render target texture.", 0, 0);
			exit(-1);
		}
	}

	return true;

}

bool D3DHelper::CreateDepthStencil(_Out_ ID3D11Texture2D ** DepthStencil, _In_ D3D11_TEXTURE2D_DESC* DepthStencilDesc)
{
	if (FAILED(Device->CreateTexture2D(DepthStencilDesc, nullptr, DepthStencil))) 
	{
		MessageBox(NULL, L"Failed to create depth stencil texture.", 0, 0);
		exit(-1);
	}

	return true;
}

bool D3DHelper::CreateRenderTargetView(ID3D11RenderTargetView ** RenderTargetView, D3D11_RENDER_TARGET_VIEW_DESC* RenderTargetViewDesc)
{
	ID3D11Texture2D* RenderTarget[BUFFERCOUNT];

	D3D11_TEXTURE2D_DESC RenderTargetDesc{};

	RenderTargetDesc.ArraySize = 1;
	RenderTargetDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	RenderTargetDesc.CPUAccessFlags = 0;
	RenderTargetDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	RenderTargetDesc.Height = Height;
	RenderTargetDesc.Width = Width;
	RenderTargetDesc.MipLevels = 1;
	RenderTargetDesc.MiscFlags = 0;
	RenderTargetDesc.Usage = D3D11_USAGE_DEFAULT;
	RenderTargetDesc.SampleDesc.Count = SampleCount;
	RenderTargetDesc.SampleDesc.Quality = SampleQuality;

	CreateRenderTarget(SwapChain, RenderTarget, &RenderTargetDesc);

	for (UINT i = 0; i < BufferCount; i++)
	{
		if (FAILED(Device->CreateRenderTargetView(RenderTarget[i], 0, &RenderTargetView[i])))
		{
			MessageBox(NULL, L"Failed to create render target view.", 0, 0);
			exit(-1);
		}
	}

	for (UINT i = 0; i < BufferCount; i++)
	{
		RenderTarget[i]->Release();
	}

	Context->OMSetRenderTargets(BufferCount, RenderTargetView, nullptr);

	return true;
}

bool D3DHelper::CreateDepthStencilView(ID3D11DepthStencilView ** DepthStencilView, D3D11_DEPTH_STENCIL_VIEW_DESC* DepthStencilViewDesc)
{
	ID3D11Texture2D* DepthStencil;

	D3D11_TEXTURE2D_DESC DepthStencilDesc{};
	DepthStencilDesc.ArraySize = 1;
	DepthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DepthStencilDesc.CPUAccessFlags = 0;
	DepthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthStencilDesc.Height = Height;
	DepthStencilDesc.Width = Width;
	DepthStencilDesc.MipLevels = 1;
	DepthStencilDesc.MiscFlags = 0;
	DepthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	DepthStencilDesc.SampleDesc.Count = SampleCount;
	DepthStencilDesc.SampleDesc.Quality = SampleQuality;

	CreateDepthStencil(&DepthStencil, &DepthStencilDesc);

	if (FAILED(Device->CreateDepthStencilView(DepthStencil, 0, DepthStencilView)))
	{
		MessageBox(NULL, L"Failed to create depth stencil view.", 0, 0);
		exit(-1);
	}
	return true;
}

bool D3DHelper::AllocConstantBuffer(ID3D11Device* Device, BaseBuffer* BaseBuffer, std::vector<Vertex>* VContainer, std::vector<WORD>* IContainer)
{

	D3D11_BUFFER_DESC VBuffer{}, IBuffer{}, CBuffer{};
	D3D11_SUBRESOURCE_DATA VData{}, IData{};

	VBuffer.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBuffer.ByteWidth = sizeof(Vertex) * VContainer->size();
	VBuffer.Usage = D3D11_USAGE_IMMUTABLE;

	IBuffer.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IBuffer.ByteWidth = sizeof(WORD) * IContainer->size();
	IBuffer.Usage = D3D11_USAGE_IMMUTABLE;

	CBuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	CBuffer.ByteWidth = sizeof(ConstantBuffer);
	CBuffer.Usage = D3D11_USAGE_DEFAULT;

	VData.pSysMem = VContainer->data();
	IData.pSysMem = IContainer->data();

	if (FAILED(Device->CreateBuffer(&VBuffer, &VData, &BaseBuffer->VBuffer))) {
		MessageBox(NULL, L"Failed to create vertex buffer", 0, 0);
		return false;

	}

	if (FAILED(Device->CreateBuffer(&IBuffer, &IData, &BaseBuffer->IBuffer))) {
		MessageBox(NULL, L"Failed to create index buffer", 0, 0);

		return false;

	}

	if (FAILED(Device->CreateBuffer(&CBuffer, NULL, &BaseBuffer->ABuffer))) {
		MessageBox(NULL, L"Failed to create constant buffer", 0, 0);

		return false;

	}

	return true;
}

bool D3DHelper::GenerateEffect(ID3D11Device * Device, Material* Resource)
{
	ID3DBlob* ErrBlob;



	if (FAILED(D3DX11CompileEffectFromFile(Resource->GetPath(), nullptr, nullptr,
		D3DCOMPILE_ENABLE_STRICTNESS, 0, Device, Resource->GetEffectPointer(), &ErrBlob)))
	{
		return false;
	}

	return true;
}

bool D3DHelper::CompileShader(ID3D11Device * Device, Material* Resource)
{
	ID3DBlob* VertexBlob, *PixelBlob, *ErrorBlob;
	HRESULT hr;
	D3DX11_PASS_DESC PassDesc;

	D3DX11CompileFromFile(Resource->GetPath(), NULL, NULL, "VS", "vs_4_0",
		D3DCOMPILE_ENABLE_STRICTNESS, 0, NULL, &VertexBlob, &ErrorBlob, &hr);

	if (FAILED(hr))
	{
		MessageBox(NULL, L"Failed to compile vertex shader", 0, 0);
		return false;
	}

	D3DX11CompileFromFile(Resource->GetPath(), NULL, NULL, "PS", "ps_4_0",
		D3DCOMPILE_ENABLE_STRICTNESS, 0, NULL, &PixelBlob, &ErrorBlob, &hr);
	
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Failed to compile pixel shader", 0, 0);
		return false;
	}

	if (FAILED(GenerateEffect(Device, Resource)))
	{
		MessageBox(NULL, L"Failed to generate effect", 0, 0);
		return false;
	}

	Resource->


	if (FAILED(Device->CreateInputLayout(Resource->GetInputLayoutVector()->data(), Resource->GetInputLayoutVector()->size(),
		VertexBlob->GetBufferPointer(), VertexBlob->GetBufferSize(), &Resource->GetInputLayout())))
	{
		MessageBox(NULL, L"Failed to create input layout", 0, 0);
	}

	VertexBlob->Release();
	PixelBlob->Release();

	return true;
}
