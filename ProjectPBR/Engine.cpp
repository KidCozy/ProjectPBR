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
		Context->ClearRenderTargetView(Renderer.GetRenderTargetView()[i], ClearColor);

	}
	return;
}

void Engine::OnInit()
{
	Helper.CreateDevice(&Device, &Context);
	Helper.CreateDXGI(&MainWindow, Factory, Width, Height, &SwapChain, Renderer.GetViewport());

	Renderer = RenderManager(Device, Context, BufferCount);

	GenerateDescriptors();

	//Helper.CreateRenderTarget(SwapChain,Renderer.GetRenderTargetArray(), Renderer.GetRenderTargetDesc());
	//Helper.CreateDepthStencil(Renderer.GetDepthStencil(), Renderer.GetDepthStencilDesc());
	Helper.CreateRenderTargetView(Renderer.GetRenderTargetView(), Renderer.GetRenderTargetViewDesc());
	//Helper.CreateDepthStencilView(Renderer.GetDepthStencil(), Renderer.GetDepthStencilView(), nullptr);




}

void Engine::OnUpdate()
{
}

void Engine::OnRender()
{
	ClearScreen(Colors::Black);
	SwapChain->Present(0, 0);
}

void Engine::OnRelease()
{
}
