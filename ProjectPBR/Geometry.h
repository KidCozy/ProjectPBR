#pragma once
#include<vector>
#include<d3d11.h>
#include"Material.h"
#include"Geometrics.h"

struct Line
{
	std::vector<Vertex> LineVertices;
	std::vector<WORD> LineIndices;
	BaseBuffer Buffer;
};

class Geometry
{
protected:

	Transform Matrix;

	std::vector<Vertex> Vertices;
	std::vector<WORD> Indices;

	Line DebugLine[3];

	BaseBuffer Buffer;
	Material MaterialResource = Material(&Matrix);


public:

	void AddTexture(ID3D11Device* Device, LPCWSTR Path);
	void AddEnvironmentTexture(ID3D11Device* Device, LPCWSTR Path);

	void SetMaterial(Material* NewMaterial) { MaterialResource = *NewMaterial; }

	Material* GetMaterial() { return &MaterialResource; }
	Transform* GetTransform() { return &Matrix; }

	Line* GetDebugLine() { return DebugLine; }

	UINT GetVertexCount() { return Vertices.size(); }
	UINT GetIndexCount() { return Indices.size(); }
	
	std::vector<Vertex>* GetVertices() { return &Vertices; }
	std::vector<WORD>* GetIndices() { return &Indices; }

	BaseBuffer* GetBuffer() { return &Buffer; }


	Geometry();
	virtual ~Geometry();
};