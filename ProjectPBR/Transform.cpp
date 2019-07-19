#include "Transform.h"

void Transform::Translation(XMVECTOR Offset)
{
	XMMATRIX Mat;

	Mat = XMLoadFloat4x4(&MatrixOrigin);

	Mat = XMMatrixTranslationFromVector(Offset);

	
	

	return;
}

void Transform::Rotate(XMVECTOR Offset)
{
	XMFLOAT4 Float4Offset;

	XMStoreFloat4(&Float4Offset, Offset);


	return;

}

void Transform::SetScale(XMVECTOR NewScale)
{
	XMFLOAT4 Float4NewScale;

	XMStoreFloat4(&Float4NewScale, NewScale);



	return;

}

XMFLOAT3 Transform::GetScale() const
{
	XMVECTOR Scale1, Scale2, Scale3;

	Scale1 = XMVectorSet(MatrixOrigin._11, MatrixOrigin._12, MatrixOrigin._13, MatrixOrigin._14);
	Scale2 = XMVectorSet(MatrixOrigin._21, MatrixOrigin._22, MatrixOrigin._23, MatrixOrigin._24);
	Scale3 = XMVectorSet(MatrixOrigin._31, MatrixOrigin._32, MatrixOrigin._33, MatrixOrigin._34);

	Scale1 = XMVector3Length(Scale1);
	Scale2 = XMVector3Length(Scale2);
	Scale3 = XMVector3Length(Scale3);

	XMFLOAT3 X, Y, Z;

	XMStoreFloat3(&X, Scale1);
	XMStoreFloat3(&Y, Scale2);
	XMStoreFloat3(&Z, Scale3);

	return XMFLOAT3(X.x, Y.y, Z.z);
}

Transform::Transform()
{

	XMStoreFloat4x4(&MatrixOrigin, XMMatrixIdentity());

	Position.SetVectorPointer(&MatrixOrigin._41, &MatrixOrigin._42, &MatrixOrigin._43, &MatrixOrigin._44);
	
	

}

Transform::~Transform()
{
}

bool MatrixPtr::SetMatrixPointer(XMFLOAT4X4* Address)
{
	if (Address == nullptr)
		return false;

	_11 = &Address->_11;
	_12 = &Address->_12;
	_13 = &Address->_13;
	_14 = &Address->_14;
	_21 = &Address->_21;
	_22 = &Address->_22;
	_23 = &Address->_23;
	_24 = &Address->_24;
	_31 = &Address->_31;
	_32 = &Address->_32;
	_33 = &Address->_33;
	_34 = &Address->_34;
	_41 = &Address->_41;
	_42 = &Address->_42;
	_43 = &Address->_43;
	_44 = &Address->_44;

	return true;
}

bool VectorPtr::SetVectorPointer(float* Element1, float* Element2, float* Element3, float* Element4)
{
	if (Element1 == nullptr || Element2 == nullptr || Element3 == nullptr || Element4 == nullptr)
		return false;

	x = Element1;
	y = Element2;
	z = Element3;
	w = Element4;

	return true;
}
