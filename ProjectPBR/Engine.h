#include<DirectXMath.h>
#include<DirectXColors.h>

#include "VirtualObject.h"
#include"RenderManager.h"
#include"D3DHelper.h"

using namespace DirectX;

class Engine :
	public VirtualObject
{
private:
	HWND MainWindow;
	UINT Width;
	UINT Height;
	UINT BufferCount = 1;

	IDXGIFactory* Factory;
	IDXGISwapChain* SwapChain;

	D3DHelper Helper = D3DHelper(Device, Context, SwapChain, Width, Height);
	RenderManager Renderer;


private:
	void GenerateDescriptors();

	void ClearScreen(XMVECTORF32 ClearColor);

protected:
	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnRender() override;
	virtual void OnRelease() override;

public:
	Engine() {}
	Engine(_In_ HWND hwnd, UINT InWidth, UINT InHeight) : MainWindow(hwnd), Width(InWidth), Height(InHeight) {}
	virtual ~Engine() {}
};
