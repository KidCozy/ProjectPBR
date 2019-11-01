#include "D3DHelper.h"


bool D3DHelper::CreateDXGI(_In_ HWND* hWnd, _Out_ IDXGIFactory* Factory, UINT Width,
	UINT Height, _Out_ IDXGISwapChain** OutSwapChain, _Out_ D3D11_VIEWPORT* Viewport, GBufferDescription* Descriptor)
{
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;

	bool MSAA = GetMSAAFeature(Device, Descriptor);

	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.BufferCount = 1;
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
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&Factory))) {
		MessageBox(NULL, L"Failed to create dxgi factory", 0, 0);
		exit(-1);
	}

	if (FAILED(Factory->CreateSwapChain(Device, &SwapChainDesc, &SwapChain))) {
		MessageBox(NULL, L"Failed to create Swap-chain", 0, 0);
		exit(-1);
	}

	*OutSwapChain = SwapChain;

	CreateViewport(Width, Height);

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

	UINT CreateDeviceFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)
	CreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif


	if (FAILED(D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, CreateDeviceFlags, nullptr, 0, D3D11_SDK_VERSION, &Device, &Levels[0], &Context))) {
		MessageBox(NULL, L"Failed to create D3D Device.", 0, 0);
		exit(-1);
	} 

	*OutDevice = Device;
	*OutContext = Context;

	return true;
}

bool D3DHelper::CreateViewport(UINT Width, UINT Height)
{
	D3D11_VIEWPORT V{};

	V.Height = (float)Height;
	V.Width = (float)Width;
	V.MinDepth = 0.0f;
	V.MaxDepth = 1.0f;
	V.TopLeftX = 0;
	V.TopLeftY = 0;

	OutViewport = V;

	D3D11_RASTERIZER_DESC RasterizerDesc{};

	ZeroMemory(&RasterizerDesc, sizeof(RasterizerDesc));

	RasterizerDesc.CullMode = D3D11_CULL_NONE;
	RasterizerDesc.FrontCounterClockwise = false;
	RasterizerDesc.AntialiasedLineEnable = false;
	RasterizerDesc.DepthClipEnable = false;
	RasterizerDesc.FillMode = D3D11_FILL_SOLID;

	Device->CreateRasterizerState(&RasterizerDesc, &RasterizerState);
	Context->RSSetState(RasterizerState);
	Context->RSSetViewports(1, &OutViewport);

	return true;
}

bool D3DHelper::GetMSAAFeature(ID3D11Device* Device, GBufferDescription* Descriptor)
{
	UINT Quality;

	for (UINT i = 4; i > 0; i /= 2) {
		if (FAILED(Device->CheckMultisampleQualityLevels(Descriptor->RenderTargetDesc.Format, i, &Quality)))
			continue;
		Descriptor->SamplerDesc.Count = i;
		break;
	}

	return Descriptor->SamplerDesc.Count == 1 ? 0 : Descriptor->SamplerDesc.Count;
}

bool D3DHelper::GenerateInputLayout(ID3D11Device* Device, LPCSTR Function, Material* Resource, ID3D11InputLayout** InputLayout)
{
	ID3D11ShaderReflection* Reflect;
	D3D11_SHADER_DESC ShaderDesc{};
	std::vector<D3D11_INPUT_ELEMENT_DESC> InputDesc;

	ID3DBlob* ErrBlob, *VBlob;

	DWORD ShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	HRESULT hr;

#if defined _DEBUG || defined DEBUG
	ShaderFlags = D3DCOMPILE_DEBUG;
#endif

	D3DX11CompileFromFile(Resource->GetPath(), NULL, NULL, Function, "vs_5_0",
		ShaderFlags, 0, NULL, &VBlob, &ErrBlob, &hr);

	if (FAILED(hr))
	{
		MessageBox(NULL, L"Failed to compile vs", 0, 0);
	}

	
	hr = D3DReflect(VBlob->GetBufferPointer(), VBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&Reflect);
	if(FAILED(hr))
	{
		MessageBox(NULL, L"Failed to reflect", 0, 0);

	}

	if (FAILED(Reflect->GetDesc(&ShaderDesc)))
	{
		MessageBox(NULL, L"Failed to getting decription from ID3D11ShaderReflection.", 0, 0);

	}
	
	for (UINT i = 0; i < ShaderDesc.InputParameters; i++)
	{
		static D3D11_SIGNATURE_PARAMETER_DESC ParamDesc;
		Reflect->GetInputParameterDesc(i, &ParamDesc);
		
		D3D11_INPUT_ELEMENT_DESC Element{};

		Element.SemanticName = ParamDesc.SemanticName;
		Element.SemanticIndex = ParamDesc.SemanticIndex;
		Element.InputSlot = 0;
		Element.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		Element.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		Element.InstanceDataStepRate = 0;

		if (ParamDesc.Mask == 1)
		{
			if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				Element.Format = DXGI_FORMAT_R32_UINT;
			else if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				Element.Format = DXGI_FORMAT_R32_FLOAT;
			else if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				Element.Format = DXGI_FORMAT_R32_SINT;
		}
		else if (ParamDesc.Mask <= 3)
		{
			if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				Element.Format = DXGI_FORMAT_R32G32_UINT;
			else if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				Element.Format = DXGI_FORMAT_R32G32_FLOAT;
			else if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				Element.Format = DXGI_FORMAT_R32G32_SINT;
		}
		else if (ParamDesc.Mask <= 7)
		{
			if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				Element.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				Element.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			else if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				Element.Format = DXGI_FORMAT_R32G32B32_SINT;
		}
		else if (ParamDesc.Mask <= 15)
		{
			if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				Element.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				Element.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			else if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				Element.Format = DXGI_FORMAT_R32G32B32A32_SINT;
		}

		InputDesc.push_back(Element);
	

	}
	


	Reflect->Release();

	if (FAILED(Device->CreateInputLayout(InputDesc.data(), InputDesc.size(),
		VBlob->GetBufferPointer(),VBlob->GetBufferSize(), InputLayout)))
		return false;

	

	return true;
}

void D3DHelper::Resize(ID3D11RenderTargetView** Merger, RTTexture* GBuffer,GBufferVariables* GBufferVars, GBufferDescription& GBufferDescription, ID3D11DepthStencilView** DepthStencilView)
{
	HRESULT hr;
	ID3D11Texture2D* BackBuffer;
	ID3D11RenderTargetView* MergeBuffer;
	if (*Merger != nullptr)
	{
		MergeBuffer = *Merger;
		MergeBuffer->Release();
	}

	if (FAILED(SwapChain->ResizeBuffers(1, GBufferDescription.RenderTargetDesc.Width, GBufferDescription.RenderTargetDesc.Height, SWAPCHAIN_FORMAT, 0)))
	{
		MessageBox(NULL, L"Failed to resizing swapchain buffers.", 0, 0);
	}
	
	if (FAILED(SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer)))
	{
		MessageBox(NULL, L"Failed to resizing", 0, 0);
	}

	if (FAILED(Device->CreateRenderTargetView(BackBuffer, 0, Merger)))
	{
		MessageBox(NULL, L"Failed to create render target view.", 0, 0);
	}

	if (FAILED(Device->CreateDepthStencilState(&GBufferDescription.DepthStencilInfoDesc, &DepthStencilState)))
	{
		MessageBox(NULL, L"Failed to create depth stencil state.", 0, 0);
	}

	CreateDepthStencilView(DepthStencilView,GBuffer, &GBufferDescription);

	//Context->OMSetDepthStencilState(DepthStencilState, 0);

	GBufferVars->ViewportDimensions = XMVectorSet(1.0f / WINDOW_WIDTH, 1.0f / WINDOW_HEIGHT, 1.0f, 1.0f);

	BackBuffer->Release();


//	ReleaseGBuffer(GBuffer, *DepthStencilView);

	for (UINT i = 0; i < (BUFFERCOUNT - 2); i++)
	{
		if (FAILED(Device->CreateTexture2D(&GBufferDescription.RenderTargetDesc, 0, &GBuffer[i].Texture)))
		{
			MessageBox(NULL, L"Failed to create texture", 0, 0);
		}
		if (FAILED(Device->CreateShaderResourceView(GBuffer[i].Texture, &GBufferDescription.SRVDesc, &GBuffer[i].SRV)))
		{
			MessageBox(NULL, L"Failed to create shader resource view.", 0, 0);
		}
		if (FAILED(Device->CreateRenderTargetView(GBuffer[i].Texture, &GBufferDescription.RTVDesc, &GBuffer[i].RTV)))
		{
			MessageBox(NULL, L"Failed to create render target view.", 0, 0);
		}
	}




}

bool D3DHelper::CreateRenderTarget(_In_ IDXGISwapChain* SwapChain, 
	_Out_ RTTexture* Buffer, _In_ D3D11_TEXTURE2D_DESC* RenderTargetDesc)
{
	for (UINT i = 0; i < BUFFERCOUNT-2; i++)
	{
		if (FAILED(Device->CreateTexture2D(RenderTargetDesc, 0, &Buffer[i].Texture)))
		{
			MessageBox(NULL, L"Failed to create render target texture.", 0, 0);
			exit(-1);
		}
	}

	return true;

}

bool D3DHelper::CreateDepthStencil(_Out_ ID3D11Texture2D ** DepthStencil, _In_ D3D11_TEXTURE2D_DESC* DepthStencilDesc)
{
	if (FAILED(Device->CreateTexture2D(DepthStencilDesc, 0, &*DepthStencil))) 
	{
		MessageBox(NULL, L"Failed to create depth stencil texture.", 0, 0);
		exit(-1);
	}

	return true;
}


bool D3DHelper::CreateRenderTargetView(RTTexture* Buffer, GBufferDescription* GBufferDescriptor)
{

	D3D11_TEXTURE2D_DESC* RenderTargetDesc = &GBufferDescriptor->RenderTargetDesc;

	RenderTargetDesc->ArraySize = 1;
	RenderTargetDesc->BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	RenderTargetDesc->CPUAccessFlags = 0;
	RenderTargetDesc->Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	RenderTargetDesc->Height = Height;
	RenderTargetDesc->Width = Width;
	RenderTargetDesc->MipLevels = 1;
	RenderTargetDesc->MiscFlags = 0;
	RenderTargetDesc->Usage = D3D11_USAGE_DEFAULT;
	RenderTargetDesc->SampleDesc.Count = SampleCount;
	RenderTargetDesc->SampleDesc.Quality = SampleQuality;

	CreateRenderTarget(SwapChain, Buffer, RenderTargetDesc);

	for (UINT i = 0; i < (BUFFERCOUNT-2); i++)
	{
		if (FAILED(Device->CreateRenderTargetView(Buffer[i].Texture, &GBufferDescriptor->RTVDesc, &Buffer[i].RTV)))
		{
			MessageBox(NULL, L"Failed to create render target view.", 0, 0);
			exit(-1);
		}

		if (FAILED(Device->CreateShaderResourceView(Buffer[i].Texture, &GBufferDescriptor->SRVDesc, &Buffer[i].SRV)))
		{
			MessageBox(NULL, L"Failed to create shader resource view.", 0, 0);
		}


	}

	return true;
}

bool D3DHelper::CreateDepthStencilView(ID3D11DepthStencilView ** DepthStencilView,RTTexture* GBuffer, GBufferDescription* GBufferDesc)
{
	ID3D11Texture2D* DepthStencil;

	CreateDepthStencil(&DepthStencil, &GBufferDesc->DepthStencilDesc);

	if (FAILED(Device->CreateDepthStencilView(DepthStencil, &GBufferDesc->DSVDesc, DepthStencilView)))
	{
		MessageBox(NULL, L"Failed to create depth stencil view.", 0, 0);
		exit(-1);
	}

	if (FAILED(Device->CreateShaderResourceView(DepthStencil, &GBufferDesc->DepthSRVDesc, &GBuffer[BufferCount - 1].SRV)))
	{
		MessageBox(NULL, L"Failed to create shader resource view. (Depth buffer)", 0, 0);
		exit(-1);
	}

	DepthStencil->Release();

	return true;
}
bool D3DHelper::AllocConstantBuffer(ID3D11Device* Device, Geometry* Geometry)
{

	D3D11_BUFFER_DESC VBuffer{}, IBuffer{}, CBuffer{};
	D3D11_SUBRESOURCE_DATA VData{}, IData{};

	VBuffer.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBuffer.ByteWidth = sizeof(Vertex) * Geometry->GetVertices()->size();
	VBuffer.Usage = D3D11_USAGE_IMMUTABLE;

	IBuffer.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IBuffer.ByteWidth = sizeof(WORD) * Geometry->GetIndices()->size();
	IBuffer.Usage = D3D11_USAGE_IMMUTABLE;

	CBuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	CBuffer.ByteWidth = sizeof(ConstantBuffer);
	CBuffer.Usage = D3D11_USAGE_DEFAULT;

	VData.pSysMem = Geometry->GetVertices()->data();
	IData.pSysMem = Geometry->GetIndices()->data();

	if (FAILED(Device->CreateBuffer(&VBuffer, &VData, &Geometry->GetBuffer()->VBuffer))) {
		MessageBox(NULL, L"Failed to create vertex buffer", 0, 0);
		return false;
	}

	if (FAILED(Device->CreateBuffer(&IBuffer, &IData, &Geometry->GetBuffer()->IBuffer))) {
		MessageBox(NULL, L"Failed to create index buffer", 0, 0);
		return false;
	}

	if (FAILED(Device->CreateBuffer(&CBuffer, NULL, &Geometry->GetBuffer()->ABuffer))) {
		MessageBox(NULL, L"Failed to create constant buffer", 0, 0);
		return false;
	}

	return true;
}
bool D3DHelper::AllocDebugLineBuffer(ID3D11Device * Device, Geometry * Geometry, UINT Index)
{
	D3D11_BUFFER_DESC VBuffer{}, IBuffer{}, CBuffer{};
	D3D11_SUBRESOURCE_DATA VData{}, IData{};

	VBuffer.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBuffer.ByteWidth = sizeof(Vertex) * Geometry->GetDebugLine()[Index].LineVertices.size();
	VBuffer.Usage = D3D11_USAGE_IMMUTABLE;

	IBuffer.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IBuffer.ByteWidth = sizeof(WORD) * Geometry->GetDebugLine()[Index].LineIndices.size();
	IBuffer.Usage = D3D11_USAGE_IMMUTABLE;

	CBuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	CBuffer.ByteWidth = sizeof(ConstantBuffer);
	CBuffer.Usage = D3D11_USAGE_DEFAULT;

	
	VData.pSysMem = Geometry->GetDebugLine()[Index].LineVertices.data();
	IData.pSysMem = Geometry->GetDebugLine()[Index].LineIndices.data();

	if (FAILED(Device->CreateBuffer(&VBuffer, &VData, &Geometry->GetDebugLine()[Index].Buffer.VBuffer))) {
		MessageBox(NULL, L"Failed to create vertex buffer", 0, 0);
		return false;
	}

	if (FAILED(Device->CreateBuffer(&IBuffer, &IData, &Geometry->GetDebugLine()[Index].Buffer.IBuffer))) {
		MessageBox(NULL, L"Failed to create index buffer", 0, 0);
		return false;
	}

	if (FAILED(Device->CreateBuffer(&CBuffer, NULL, &Geometry->GetDebugLine()[Index].Buffer.ABuffer))) {
		MessageBox(NULL, L"Failed to create constant buffer", 0, 0);
		return false;
	}

	return true;
}
bool D3DHelper::GenerateEffect(ID3D11Device * Device, Material* Resource)
{
	ID3DBlob* ErrBlob;

	DWORD ShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	HRESULT hr;

#if defined _DEBUG || defined DEBUG
	ShaderFlags = D3DCOMPILE_DEBUG;
#endif

	if (FAILED(D3DX11CompileEffectFromFile(Resource->GetPath(), nullptr, nullptr,
		ShaderFlags, 0, Device, Resource->GetEffectPointer(), &ErrBlob)))
	{
		return false;
	}

	return true;
}

void D3DHelper::ReleaseGBuffer(RTTexture* GBuffer, ID3D11DepthStencilView* DSV)
{

	DSV->Release();

	for (UINT i = 0; i < BUFFERCOUNT; i++)
	{
		if (GBuffer[i].RTV != nullptr)
		{
			GBuffer[i].RTV->Release();
		}

		if (GBuffer[i].SRV != nullptr)
		{
			GBuffer[i].SRV->Release();
		}

		if (GBuffer[i].Texture != nullptr)
		{
			GBuffer[i].Texture->Release();

		}
	}

}

bool D3DHelper::CompileShader(ID3D11Device * Device, Material* Resource)
{
	ID3DBlob* VertexBlob, *PixelBlob, *ErrorBlob;
	ID3D11InputLayout* TempLayout;
	ID3D11ShaderReflection* Reflect;
	HRESULT hr;
	D3DX11_PASS_DESC Desc{};

	if (!GenerateEffect(Device, Resource))
	{
		MessageBox(NULL, L"Failed to generate effect", 0, 0);
		return false;
	}

	
	Resource->SetWorldMatrixPointer(Resource->GetEffect()->GetVariableByName("World")->AsMatrix());
	Resource->SetViewMatrixPointer(Resource->GetEffect()->GetVariableByName("View")->AsMatrix());
	Resource->SetProjectionMatrixPointer(Resource->GetEffect()->GetVariableByName("Projection")->AsMatrix());

	for(UINT i=0;;++i)
	{
		if (!Resource->GetEffect()->GetVariableByIndex(i)->IsValid())
			break;

		D3DX11_EFFECT_VARIABLE_DESC Desc;
		ID3DX11EffectVariable* M = Resource->GetEffect()->GetVariableByIndex(i);

		M->GetDesc(&Desc);

		std::string str = Desc.Name;

		Resource->GetShaderVariables()._Insert_or_assign(str, M);

	}

	//if (FAILED(Resource->GetEffect()->GetTechniqueByName("GeometryTech")->GetPassByIndex(0)->GetDesc(&Desc)))
	//{
	//	MessageBox(NULL, L"Failed to getting descriptor from the pass.", 0, 0);
	//}

	GenerateInputLayout(Device,"VS", Resource, &TempLayout);
	Resource->GetInputLayouts()->push_back(TempLayout);
	GenerateInputLayout(Device, "BasicVS", Resource, &TempLayout);
	Resource->GetInputLayouts()->push_back(TempLayout);

	return true;
}
