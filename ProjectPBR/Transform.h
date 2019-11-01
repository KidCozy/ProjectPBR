#pragma once
#include<DirectXMath.h>

#ifdef _DEBUG
	#include<iostream>
#endif

using namespace DirectX;

struct MatrixPtr 
{
	float* _11, *_12, *_13, *_14;
	float* _21, *_22, *_23, *_24;
	float* _31, *_32, *_33, *_34;
	float* _41, *_42, *_43, *_44;

	bool SetMatrixPointer(XMFLOAT4X4* Matrix);
};

struct VectorPtr
{
	float* x, *y, *z, *w;

	bool SetVectorPointer(float* Element1, float* Element2, float* Element3, float* Element4);
};

struct Transform
{
private:
	XMFLOAT4X4 MatrixOrigin;
	MatrixPtr Matrix;

	VectorPtr Position;
	VectorPtr Rotation;
	VectorPtr Scale;

	

public:
	void Translation(XMVECTOR Offset);
	void Rotate(XMVECTOR Offset);
	void SetScale(XMVECTOR NewScale);
	_inline void SetMatrix(XMMATRIX Mat) { XMStoreFloat4x4(&MatrixOrigin, Mat); }

	MatrixPtr* GetMatrixPointer() { return &Matrix; }

	XMFLOAT4X4 GetTransform() const { return MatrixOrigin; }
	XMVECTOR GetPosition() const { return XMVectorSet(MatrixOrigin._41, MatrixOrigin._42, MatrixOrigin._43, 1.0f); }
	XMVECTOR GetRotation() const { return XMVectorSet(MatrixOrigin._11, MatrixOrigin._22, MatrixOrigin._33, 1.0f); }
	XMFLOAT3 GetScale() const;

	Transform();
	~Transform();

};

