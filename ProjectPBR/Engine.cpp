#include "Engine.h"

void Engine::OnInit()
{
	Helper.CreateDevice(Device, Context);
	Helper.CreateDXGI(MainWindow, Factory, Width, Height, SwapChain);
	
}

void Engine::OnUpdate()
{
}

void Engine::OnRender()
{
}

void Engine::OnRelease()
{
}
