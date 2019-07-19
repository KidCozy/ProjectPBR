#pragma once
#include<vector>
#include<d3d11.h>
#include"Material.h"
#include"Geometrics.h"
class Geometry
{
protected:

	Transform Matrix;

	std::vector<Vertex> Vertices;
	std::vector<WORD> Indices;

	BaseBuffer Buffer;
	Material MaterialResource = Material(&Matrix);
public:

	Material* GetMaterial() { return &MaterialResource; }
	Transform* GetTransform() { return &Matrix; }

	UINT GetVertexCount() { return Vertices.size(); }
	UINT GetIndexCount() { return Indices.size(); }
	
	std::vector<Vertex>* GetVertices() { return &Vertices; }
	std::vector<WORD>* GetIndices() { return &Indices; }

	BaseBuffer* GetBuffer() { return &Buffer; }


	Geometry();
	virtual ~Geometry();
};