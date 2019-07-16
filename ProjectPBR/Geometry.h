#pragma once
#include<vector>
#include<d3d11.h>
#include"Material.h"
#include"Geometrics.h"

class Geometry
{
protected:
	std::vector<Vertex> Vertices;
	std::vector<UINT> Indices;

	BaseBuffer* Buffer;
	Material MaterialResource;
public:

	Material* GetMaterial() { return &MaterialResource; }

	Geometry();
	virtual ~Geometry();
};