#pragma once
#include<DirectXMath.h>
using namespace DirectX;

struct Position
{
private:
	XMFLOAT4 Pos;
public:
	void operator+(XMVECTOR Vec)
	{
		XMFLOAT4 O;

		XMStoreFloat4(&O, Vec);

		Pos.x += O.x;
		Pos.y += O.y;
		Pos.z += O.z;

		return;
	}

	void operator+(XMFLOAT4 Vec)
	{
		Pos.x += Vec.x;
		Pos.y += Vec.y;
		Pos.z += Vec.z;

		return;
	}

};

struct Rotation
{
private:
	XMFLOAT4 Rot;
public:
	void operator+(XMVECTOR Vec)
	{
		XMFLOAT4 O;

		XMStoreFloat4(&O, Vec);

		Rot.x += O.x;
		Rot.y += O.y;
		Rot.z += O.z;

		return;
	}

	void operator+(XMFLOAT4 Vec)
	{
		Rot.x += Vec.x;
		Rot.y += Vec.y;
		Rot.z += Vec.z;

		return;
	}
};

struct Scale
{

};

struct Transform
{
	XMFLOAT4X4 TRSMatrix;

	XMFLOAT4 Position;
	XMFLOAT4 Rotation;
	XMFLOAT4 Scale;

};

