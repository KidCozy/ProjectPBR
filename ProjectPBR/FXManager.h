#pragma once
#include"Material.h"
#include<d3dx11effect.h>
#include<vector>

class FXManager
{
private:
	std::vector<Material> MaterialVector;
public:
	FXManager();
	~FXManager();
};

