#pragma once
#include"Geometrics.h"
class Camera
	: public EventHandler
{
private:
	Transform Matrix;

	XMFLOAT4X4 Projection;

	float AspectRatio;
	float FOV;
	float Near;
	float Far;

public:

	XMMATRIX GetView() const { return XMLoadFloat4x4(&Matrix.GetTransform()); }
	XMMATRIX GetProjection() const { return XMLoadFloat4x4(&Projection); }

	XMVECTOR GetDirection() const;

	Camera(float InAR, float InFOV, float InNear, float InFar);
	~Camera();

	// EventHandler을(를) 통해 상속됨
	virtual void RDragNotify(WinMessage* Event, WinMessage* NewEvent) override;

	// EventHandler을(를) 통해 상속됨
	virtual void LDragNotify(WinMessage * Event, WinMessage * NewEvent) override;
	virtual void KeyEnterNotify(WinMessage * Event, WinMessage * NewEvent) override;
	virtual void KeyPressNotify(WinMessage * Event, WinMessage * NewEvent) override;
	virtual void KeyReleaseNotify(WinMessage * Event, WinMessage * NewEvent) override;
};

