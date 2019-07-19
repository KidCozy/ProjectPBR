#include "Camera.h"

Camera::Camera(float InAR, float InFOV, float InNear, float InFar)
{
	XMMATRIX MatView, MatProjection;
	XMVECTOR Eye, At, Up;

	AspectRatio = InAR;
	FOV = InFOV;
	Near = InNear;
	Far = InFar;


	Eye = XMVectorSet(1.0f, 5.0f, 1.0f, 1.0f);
	At = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	Up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);

	MatView = XMMatrixLookAtLH(Eye, At, Up);
	MatProjection = XMMatrixPerspectiveFovLH(FOV, AspectRatio, Near, Far);

	XMStoreFloat4x4(&View, MatView);
	XMStoreFloat4x4(&Projection, MatProjection);
}

Camera::~Camera()
{
}
