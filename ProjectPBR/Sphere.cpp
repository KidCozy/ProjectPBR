#include "stdafx.h"
#include "Sphere.h"

void Sphere::OnInit()
{
	Vertices.clear();
	Indices.clear();

	Vertex topVertex{ { 0.0f, +Radius, 0.0f}, { +1.0f, 0.0f, 1.0f},{0.0f,1.0f,0.0f }, { 1.0f,0.0f,0.0f }, { 0.0f,0.0f } };
	Vertex bottomVertex{ { 0.0f, -Radius, 0.0f}, {-1.0f, 0.0f, 1.0f}, {0.0f,-1.0f,0.0f}, {-1.0f,0.0f,0.0f}, {0.0f,1.0f} };

	Vertices.push_back(topVertex);

	float phiStep = XM_PI / Stack;
	float thetaStep = 2.0f*XM_PI / Slice;

	for (UINT i = 1; i <= Stack - 1; ++i)
	{
		float phi = i * phiStep;

		for (UINT j = 0; j <= Slice; ++j)
		{
			float theta = j * thetaStep;

			Vertex v;

			v.Position.x = Radius * sinf(phi)*cosf(theta);
			v.Position.y = Radius * cosf(phi);
			v.Position.z = Radius * sinf(phi)*sinf(theta);

			v.Tangent.x = -Radius * sinf(phi)*sinf(theta);
			v.Tangent.y = 0.0f;
			v.Tangent.z = +Radius * sinf(phi)*cosf(theta);

			v.UV.x = theta / XM_2PI;
			v.UV.y = phi / XM_PI;

			XMVECTOR p = XMLoadFloat3(&v.Position);
			XMVECTOR b;
			
			//
			//XMStoreFloat3(&v.Binormal, XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&v.Normal), XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f))));
			//XMStoreFloat3(&v.Tangent, XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&v.Binormal), XMLoadFloat3(&v.Normal))));

			XMStoreFloat3(&v.Normal, XMVector3Normalize(p));
			XMStoreFloat3(&v.Binormal, XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f));

			Vertices.push_back(v);
		}
	}

	Vertices.push_back(bottomVertex);

	const int s = Vertices.size() * 2;

	DebugLine[0].LineVertices.resize(s);
	DebugLine[0].LineIndices.resize(s);
	DebugLine[1].LineVertices.resize(s);
	DebugLine[1].LineIndices.resize(s);
	DebugLine[2].LineVertices.resize(s);
	DebugLine[2].LineIndices.resize(s);

	for (int i = 0, j=0; i < s; i += 2, j++)
	{
		
		Vertex n;
		XMVECTOR XMNormalVector, XMTangentVector;
		n = Vertices[j];

		DebugLine[0].LineVertices[i] = n;
		DebugLine[0].LineVertices[i + 1] = n;

		DebugLine[1].LineVertices[i] = n;
		DebugLine[1].LineVertices[i + 1] = n;

		XMNormalVector = XMLoadFloat3(&DebugLine[0].LineVertices[i + 1].Position);
		XMNormalVector *= 1.1f;

		XMTangentVector = XMLoadFloat3(&DebugLine[1].LineVertices[i + 1].Tangent);
		XMTangentVector *= 1.1f;

		XMStoreFloat3(&DebugLine[0].LineVertices[i + 1].Position, XMNormalVector);
		XMStoreFloat3(&DebugLine[1].LineVertices[i + 1].Position, XMTangentVector);

		DebugLine[0].LineIndices[i] = i;
		DebugLine[0].LineIndices[i + 1] = i + 1;

		DebugLine[1].LineIndices[i] = i;
		DebugLine[1].LineIndices[i + 1] = i + 1;


		XMStoreFloat4(&DebugLine[0].LineVertices[i].Color, XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));
		XMStoreFloat4(&DebugLine[0].LineVertices[i+1].Color, XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));
		XMStoreFloat4(&DebugLine[1].LineVertices[i].Color, XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f));
		XMStoreFloat4(&DebugLine[1].LineVertices[i+1].Color, XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f));
		//XMStoreFloat3(&DebugLine[0].LineVertices[i].Color, XMVectorSet(0.0f, 1.0f, 0.0f));
	}

	for (UINT i = 1; i <= Slice; ++i)
	{
		Indices.push_back(0);
		Indices.push_back(i + 1);
		Indices.push_back(i);
	}

	UINT baseIndex = 1;
	UINT ringVertexCount = Slice + 1;
	for (UINT i = 0; i < Stack - 2; ++i)
	{
		for (UINT j = 0; j < Slice; ++j)
		{
			Indices.push_back(baseIndex + i * ringVertexCount + j);
			Indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			Indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);

			Indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);
			Indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			Indices.push_back(baseIndex + (i + 1)*ringVertexCount + j + 1);
		}
	}

	UINT southPoleIndex = (UINT)Indices.size() - 1;

	baseIndex = southPoleIndex - ringVertexCount;

	for (UINT i = 0; i < Slice; ++i)
	{
		Indices.push_back(southPoleIndex);
		Indices.push_back(baseIndex + i);
		Indices.push_back(baseIndex + i + 1);
	}

}

void Sphere::OnUpdate()
{
}

void Sphere::OnRender()
{
}

void Sphere::OnRelease()
{
}

void Sphere::SetProperty(float InRadius, UINT InSlice, UINT InStack)
{
	Radius = InRadius;
	Slice = InSlice;
	Stack = InStack;

	return;
}

Sphere::Sphere()
{
	Radius = 1.0f;
	Slice = 32;
	Stack = 32;
}


Sphere::~Sphere()
{
}
