#pragma once
#include<vector>
#include<d3d11.h>
#include"Geometrics.h"
class Geometry
{
protected:
	std::vector<Vertex> Vertices;
	std::vector<UINT> Indices;
public:
	Geometry();
	virtual ~Geometry();
};