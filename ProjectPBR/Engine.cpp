#include "Engine.h"

void Engine::GenerateDescriptors(GBufferDescription* Descriptor)
{

	D3D11_TEXTURE2D_DESC* RenderTargetDesc = &Renderer.GetGBufferDescriptor()->RenderTargetDesc;

	RenderTargetDesc->ArraySize = 1;
	RenderTargetDesc->BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	RenderTargetDesc->Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	RenderTargetDesc->Height = Height;
	RenderTargetDesc->Width = Width;
	RenderTargetDesc->MipLevels = 1;
	RenderTargetDesc->Usage = D3D11_USAGE_DEFAULT;
	RenderTargetDesc->SampleDesc.Count = Helper.GetSampleCount();
	RenderTargetDesc->SampleDesc.Quality = Helper.GetSampleQuality();

	D3D11_TEXTURE2D_DESC* DepthStencilDesc = Renderer.GetDepthStencilDesc();
	DepthStencilDesc->ArraySize = 1;
	DepthStencilDesc->BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DepthStencilDesc->Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthStencilDesc->Height = Height;
	DepthStencilDesc->Width = Width;
	DepthStencilDesc->MipLevels = 1;
	DepthStencilDesc->Usage = D3D11_USAGE_DEFAULT;
	DepthStencilDesc->SampleDesc.Count = 1;
	DepthStencilDesc->SampleDesc.Quality = 0;

	D3D11_SHADER_RESOURCE_VIEW_DESC* ShaderResourceViewDesc = &Renderer.GetGBufferDescriptor()->SRVDesc;
	ShaderResourceViewDesc->Format = Renderer.GetGBufferDescriptor()->RenderTargetDesc.Format;
	ShaderResourceViewDesc->ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	ShaderResourceViewDesc->Texture2D.MipLevels = 1;

	D3D11_RENDER_TARGET_VIEW_DESC* RenderTargetViewDesc = Renderer.GetRenderTargetViewDesc();
	RenderTargetViewDesc->Format = Renderer.GetGBufferDescriptor()->RenderTargetDesc.Format;
	RenderTargetViewDesc->ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	
	D3D11_DEPTH_STENCIL_VIEW_DESC* DepthStencilViewDesc = &Descriptor->DSVDesc;
	DepthStencilViewDesc->Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthStencilViewDesc->ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	
	D3D11_DEPTH_STENCIL_DESC* DepthStencilInfoDesc = Renderer.GetDepthStencilInfoDesc();
	DepthStencilInfoDesc->DepthEnable = false;
	DepthStencilInfoDesc->DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	DepthStencilInfoDesc->DepthFunc = D3D11_COMPARISON_LESS;
	DepthStencilInfoDesc->StencilEnable = false;
	DepthStencilInfoDesc->StencilReadMask = 0xFF;
	DepthStencilInfoDesc->StencilWriteMask = 0xFF;

	DepthStencilInfoDesc->FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilInfoDesc->FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	DepthStencilInfoDesc->FrontFace.StencilPassOp= D3D11_STENCIL_OP_KEEP;
	DepthStencilInfoDesc->FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	
	DepthStencilInfoDesc->BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilInfoDesc->BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	DepthStencilInfoDesc->BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilInfoDesc->BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;



}



void Engine::PostInitialize()
{


	Helper.CreateDevice(&Device, &Context);
	Helper.CreateDXGI(&MainWindow, Factory, Width, Height, &SwapChain, Renderer.GetViewport(), Renderer.GetGBufferDescriptor());
	Renderer = RenderManager(Width, Height, Device, Context, BufferCount);

	GenerateDescriptors(Renderer.GetGBufferDescriptor());

//	Helper.CreateRenderTargetView(Renderer.GetGBufferPointer(), Renderer.GetGBufferDescriptor());
	//Helper.CreateDepthStencilView(&*Renderer.GetDepthStencilViewPointer(), Renderer.GetDepthStencilViewDesc());
	
	Helper.Resize(Renderer.GetMergeBufferPointer(),Renderer.GetGBufferPointer(), *Renderer.GetGBufferDescriptor(),Renderer.GetDepthStencilViewPointer());
	//Helper.CreateDepthStencilView(Renderer.GetDepthStencilViewPointer(), Renderer.GetDepthStencilViewDesc());



}

void Engine::OnInit()
{

	Renderer.Init();

}

void Engine::OnUpdate()
{
	Renderer.Update();


}

void Engine::OnRender()
{


	Renderer.Render();

	SwapChain->Present(0, 0);
}

void Engine::OnRelease()
{
	Renderer.Release();
}
