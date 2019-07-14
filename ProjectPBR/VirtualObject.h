#pragma once
class VirtualObject
{
protected:
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