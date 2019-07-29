#pragma once
struct ID3D11Device;
struct ID3D11DeviceContext;

class VirtualObject
{
protected:
	ID3D11Device* Device;
	ID3D11DeviceContext* Context;

protected:
	
	virtual void PostInitialize() {}

	virtual void OnInit() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnRender() = 0;
	virtual void OnRelease() = 0;



public:

	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void Release();

	VirtualObject();
	virtual ~VirtualObject();
};