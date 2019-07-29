#include "Engine.h"

void Engine::GenerateDescriptors()
{


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

	//D3D11_RENDER_TARGET_VIEW_DESC* RenderTargetViewDesc = Renderer.GetRenderTargetViewDesc();
	//RenderTargetViewDesc->Format = RenderTargetDesc->Format;
	//RenderTargetViewDesc->ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	//RenderTargetViewDesc->Texture2D.MipSlice = 1;
	
}

void Engine::ClearScreen(XMVECTORF32 ClearColor)
{
	for (UINT i = 0; i < BufferCount; i++) {
		Context->ClearRenderTargetView(Renderer.GetRenderTargetViewPointer()[i], ClearColor);
	}
	return;
}

void Engine::PostInitialize()
{


	Helper.CreateDevice(&Device, &Context);
	Helper.CreateDXGI(&MainWindow, Factory, Width, Height, &SwapChain, Renderer.GetViewport());
	Renderer = RenderManager(Width, Height, Device, Context, BufferCount);

	GenerateDescriptors();

	Helper.CreateRenderTargetView(Renderer.GetGBufferPointer(), Renderer.GetRenderTargetViewDesc());
	Helper.CreateDepthStencilView(Renderer.GetDepthStencilView(), Renderer.GetDepthStencilViewDesc());

	Helper.Resize(Renderer.GetGBufferPointer(), *Renderer.GetGBufferDescriptor());

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
	ClearScreen(Colors::Black);

	Renderer.Render();

	SwapChain->Present(0, 0);
}

void Engine::OnRelease()
{
	Renderer.Release();
}
