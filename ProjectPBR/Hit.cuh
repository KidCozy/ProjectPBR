#pragma once
#ifndef HITTABLEH
#define HITTABLEH

#include"Ray.cuh"

class Material;

struct HitRecord
{
	float T;
	XMFLOAT3 P;
	XMFLOAT3 Normal;
	Material* MatPtr;
};

class Hittable
{
public:
	virtual bool Hit(const Ray& R, float T_Min, float T_Max, HitRecord& Rec) const = 0;
};

#endif