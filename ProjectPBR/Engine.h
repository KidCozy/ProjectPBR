#include<DirectXMath.h>
#include<DirectXColors.h>
#include "VirtualObject.h"
#include"RenderManager.h"
#include"D3DHelper.h"
#include"ExtendD3DHelper.h"

using namespace DirectX;

class Engine :
	public VirtualObject, public EventHandler
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
	void GenerateDescriptors(GBufferDescription* Descriptor);
	void InitializeImGui();
protected:

	virtual void PostInitialize() override;

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnRender() override;
	virtual void OnRelease() override;

public:

	Engine() {}
	Engine(_In_ HWND hwnd, UINT InWidth, UINT InHeight) : MainWindow(hwnd), Width(InWidth), Height(InHeight) {}
	virtual ~Engine() {}

	// EventHandler을(를) 통해 상속됨
	virtual void RDragNotify(WinMessage* PreviousEvent, WinMessage* NewEvent) override;

	// EventHandler을(를) 통해 상속됨
	virtual void LDragNotify(WinMessage * Event, WinMessage * NewEvent) override;
	virtual void KeyEnterNotify(WinMessage * Event, WinMessage * NewEvent) override;
	virtual void KeyPressNotify(WinMessage * Event, WinMessage * NewEvent) override;
	virtual void KeyReleaseNotify(WinMessage * Event, WinMessage * NewEvent) override;
};
