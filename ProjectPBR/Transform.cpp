#include "Transform.h"



void Transform::operator+(XMVECTOR Vec)
{

	/*** Identity Matrix ***/
	// 1  0  0  0
	// 0  1  0  0
	// 0  0  1  0
	// 0  0  0  1
	/***				***/
	XMMATRIX Mat, Origin;

	Mat = XMMatrixIdentity();
	Origin = XMLoadFloat4x4(&TRSMatrix);

	Position.x = XMVectorGetX(Vec);
	Position.y = XMVectorGetY(Vec);
	Position.z = XMVectorGetZ(Vec);

	Origin *= Mat;

	XMStoreFloat4x4(&TRSMatrix, Origin);
	
	return;
}



Transform::Transform()
{

}


Transform::~Transform()
{
}
