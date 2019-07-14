#include "VirtualObject.h"
#include"D3DHelper.h"
class Engine :
	public VirtualObject
{
private:
	HWND MainWindow;
	UINT Width;
	UINT Height;

	ID3D11Device* Device;
	ID3D11DeviceContext* Context;

	IDXGIFactory* Factory;
	IDXGISwapChain* SwapChain;

	D3DHelper Helper = D3DHelper(Device, Width, Height);

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
