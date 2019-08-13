#pragma once
#include<D3D11.h>

#include<DirectXMath.h>
class Ray
{
private:
	DirectX::XMFLOAT4 From;
	DirectX::XMFLOAT4 To;


public:

	DirectX::XMVECTOR GetPoint() const { return XMLoadFloat4(&From); }
	DirectX::XMVECTOR GetDirection() const { return XMLoadFloat4(&To); }

	Ray();
	Ray(DirectX::XMVECTOR Point, DirectX::XMVECTOR Direction);
	~Ray();
};

