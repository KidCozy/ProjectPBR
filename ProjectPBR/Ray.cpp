#include "Ray.h"



Ray::Ray()
{
}

Ray::Ray(DirectX::XMVECTOR Point, DirectX::XMVECTOR Direction)
{
	DirectX::XMStoreFloat4(&From, Point);
	DirectX::XMStoreFloat4(&To, Direction);

}


Ray::~Ray()
{
}
