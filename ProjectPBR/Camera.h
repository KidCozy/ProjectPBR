#pragma once
#include"Geometrics.h"
class Camera
{
private:
	Transform Matrix;

	XMFLOAT4X4 View;
	XMFLOAT4X4 Projection;

	float AspectRatio;
	float FOV;
	float Near;
	float Far;

public:

	XMMATRIX GetView() const { return XMLoadFloat4x4(&View); }
	XMMATRIX GetProjection() const { return XMLoadFloat4x4(&Projection); }

	Camera(float InAR, float InFOV, float InNear, float InFar);
	~Camera();
};

