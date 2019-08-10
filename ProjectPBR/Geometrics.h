#pragma once

#include<D3D11.h>
#include<Windows.h>
#include<DirectXMath.h>
#include"EventHandler.h"
#include"Transform.h"
using namespace DirectX;

struct Vertex {
	DirectX::XMFLOAT3 Position{};
	DirectX::XMFLOAT3 Normal{};
	DirectX::XMFLOAT3 Binormal{};
	DirectX::XMFLOAT3 Tangent{};
	DirectX::XMFLOAT2 UV{};
	DirectX::XMFLOAT4 Color{};
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