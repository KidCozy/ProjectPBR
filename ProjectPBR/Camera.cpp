#include "Camera.h"

XMVECTOR Camera::GetDirection() const
{
	return XMVectorSet(Matrix.GetTransform()._13,Matrix.GetTransform()._23, Matrix.GetTransform()._33, 1.0f);
}

Camera::Camera(float InAR, float InFOV, float InNear, float InFar)
{
	XMMATRIX MatView, MatProjection;
	XMVECTOR Eye, At, Up;

	AspectRatio = InAR;
	FOV = InFOV;
	Near = InNear;
	Far = InFar;

	Eye = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	At = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	Up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);

	MatView = XMMatrixLookAtLH(Eye, At, Up);
	MatProjection = XMMatrixPerspectiveFovLH(FOV, AspectRatio, Near, Far);

	Matrix.SetMatrix(MatView);

	//XMStoreFloat4x4(&, MatView);
	XMStoreFloat4x4(&Projection, MatProjection);

	
}

Camera::~Camera() { }

void Camera::RDragNotify(WinMessage* Event, WinMessage* NewEvent)
{
	static RECT rt;
	rt.bottom = 720;
	rt.left = 0;
	rt.right = 1280;
	rt.top = 0;
//	std::cout << "Camera Notify" << std::endl;
//	ShowCursor(false);

	int Px = HIWORD(Event->lParam);
	int Py = LOWORD(Event->lParam);
	
	int Cx = HIWORD(NewEvent->lParam);
	int Cy = LOWORD(NewEvent->lParam);

	float DeltaX = ((Cx - Px) * 0.01f);
	float DeltaY = ((Cy - Py) * 0.01f);

	Matrix.Rotate(XMVectorSet(DeltaX, DeltaY, 0.0f, 1.0f));

	//Matrix.Translation(XMVectorSet(0.001f, 0.0f, 0.0f, 1.0f));
}

void Camera::LDragNotify(WinMessage * Event, WinMessage * NewEvent)
{
}

void Camera::KeyEnterNotify(WinMessage * Event, WinMessage * NewEvent)
{

	if(GetAsyncKeyState('W') & 0x8000)
		Matrix.Translation(XMVectorSet(0.0f, 0.0f, -0.1f, 1.0f));
	if (GetAsyncKeyState('S') & 0x8000)
		Matrix.Translation(XMVectorSet(0.0f, 0.0f, 0.1f, 1.0f));

	if (GetAsyncKeyState('A') & 0x8000)
		Matrix.Translation(XMVectorSet(0.1f, 0.0f, 0.0f, 1.0f));

	if (GetAsyncKeyState('D') & 0x8000)
		Matrix.Translation(XMVectorSet(-0.1f, 0.0f, 0.0f, 1.0f));
}

void Camera::KeyPressNotify(WinMessage * Event, WinMessage * NewEvent)
{
}

void Camera::KeyReleaseNotify(WinMessage * Event, WinMessage * NewEvent)
{
}
