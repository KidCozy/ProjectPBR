#include "Quad.h"



Quad::Quad()
{
}


Quad::~Quad()
{
}

void Quad::OnInit()
{
	Vertex v1{ {-1.0f, -1.0f, 0.0f}};
	Vertex v2{ {1.0f, -1.0f, 0.0f}};
	Vertex v3{ {-1.0f, 1.0f, 0.0f}};
	Vertex v4{ {1.0f, 1.0f, 0.0f}};

	WORD i[6] = { 0,1,2,
				  1,2,3 };

	Vertices.push_back(v1);
	Vertices.push_back(v2);
	Vertices.push_back(v3);
	Vertices.push_back(v4);

	Indices.push_back(i[0]);
	Indices.push_back(i[1]);
	Indices.push_back(i[2]);
	Indices.push_back(i[3]);
	Indices.push_back(i[4]);
	Indices.push_back(i[5]);

	Position = MaterialResource.GetEffect()->GetVariableByName("PositionBuffer")->AsShaderResource();
	Normal = MaterialResource.GetEffect()->GetVariableByName("NormalBuffer")->AsShaderResource();
}

void Quad::OnUpdate()
{
}

void Quad::OnRender()
{
}

void Quad::OnRelease()
{
}
