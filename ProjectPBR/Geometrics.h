#pragma once
#include"Transform.h"
#include<D3D11.h>
#include<Windows.h>
#include<DirectXMath.h>
using namespace DirectX;

struct Vertex {
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Normal;
//	DirectX::XMFLOAT2 UV;
};

struct BaseBuffer {
	ID3D11Buffer* VBuffer;
	ID3D11Buffer* IBuffer;
	ID3D11Buffer* ABuffer;
};

struct ConstantBuffer {
	XMMATRIX World;
	XMMATRIX View;
	XMMATRIX Projection;
};