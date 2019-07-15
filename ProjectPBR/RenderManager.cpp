#include "RenderManager.h"

RenderManager::RenderManager(ID3D11Device * InDevice, ID3D11DeviceContext * InContext, UINT BufferCount)
{
	Device = InDevice;
	Context = InContext;


}

void RenderManager::OnInit()
{
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
