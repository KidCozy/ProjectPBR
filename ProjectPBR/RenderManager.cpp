#include "RenderManager.h"

void RenderManager::SetInputElements()
{

	DefaultInputLayout.push_back(
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 } );
	DefaultInputLayout.push_back(
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 });


}

RenderManager::RenderManager(ID3D11Device * InDevice, ID3D11DeviceContext * InContext, UINT BufferCount)
{
	Device = InDevice;
	Context = InContext;

	

}

void RenderManager::OnInit()
{
	SetInputElements();

	StaticSphere.SetProperty(3.0f, 64, 64);
	StaticSphere.Init();

	StaticSphere.GetMaterial()->CompileShader(Context, Device, DefaultInputLayout, L"DefaultShader.fx");

}

void RenderManager::OnUpdate()
{

}

void RenderManager::OnRender()
{
}

void RenderManager::OnRelease()
{
}
