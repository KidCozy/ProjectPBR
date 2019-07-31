#include "Engine.h"

void Engine::GenerateDescriptors(GBufferDescription* Descriptor)
{

	D3D11_TEXTURE2D_DESC* RenderTargetDesc = &Renderer.GetGBufferDescriptor()->RenderTargetDesc;

	RenderTargetDesc->ArraySize = 1;
	RenderTargetDesc->BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	RenderTargetDesc->CPUAccessFlags = 0;
	RenderTargetDesc->Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	RenderTargetDesc->Height = Height;
	RenderTargetDesc->Width = Width;
	RenderTargetDesc->MipLevels = 1;
	RenderTargetDesc->MiscFlags = 0;
	RenderTargetDesc->Usage = D3D11_USAGE_DEFAULT;
	RenderTargetDesc->SampleDesc.Count = Helper.GetSampleCount();
	RenderTargetDesc->SampleDesc.Quality = Helper.GetSampleQuality();


	D3D11_TEXTURE2D_DESC* DepthStencilDesc = Renderer.GetDepthStencilDesc();
	DepthStencilDesc->ArraySize = 1;
	DepthStencilDesc->BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DepthStencilDesc->CPUAccessFlags = 0;
	DepthStencilDesc->Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthStencilDesc->Height = Height;
	DepthStencilDesc->Width = Width;
	DepthStencilDesc->MipLevels = 1;
	DepthStencilDesc->MiscFlags = 0;
	DepthStencilDesc->Usage = D3D11_USAGE_DEFAULT;
	DepthStencilDesc->SampleDesc.Count = Helper.GetSampleCount();
	DepthStencilDesc->SampleDesc.Quality = Helper.GetSampleQuality();

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

}



void Engine::PostInitialize()
{


	Helper.CreateDevice(&Device, &Context);
	Helper.CreateDXGI(&MainWindow, Factory, Width, Height, &SwapChain, Renderer.GetViewport(), Renderer.GetGBufferDescriptor());
	Renderer = RenderManager(Width, Height, Device, Context, BufferCount);

	GenerateDescriptors(Renderer.GetGBufferDescriptor());

	//Helper.CreateRenderTargetView(Renderer.GetGBufferPointer(), Renderer.GetGBufferDescriptor());
	Helper.CreateDepthStencilView(Renderer.GetDepthStencilViewPointer(), Renderer.GetDepthStencilViewDesc());

//	D3DHelper::ReleaseGBuffer(Renderer.GetGBufferPointer(), Renderer.GetDepthStencilView());

	Helper.Resize(Renderer.GetMergeBufferPointer(),Renderer.GetGBufferPointer(), *Renderer.GetGBufferDescriptor(), Renderer.GetDepthStencilView());
	Helper.CreateDepthStencilView(Renderer.GetDepthStencilViewPointer(), Renderer.GetDepthStencilViewDesc());



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
