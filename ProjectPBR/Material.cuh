#pragma once
#ifndef MATERIALH
#define MATERIALH

#include"Hit.cuh"
#include"Random.cuh"

XMVECTOR RadnomInUnitSphere();
XMVECTOR Reflect(const XMVECTOR& V, const XMVECTOR& N);
bool Refract(const XMVECTOR& V, const XMVECTOR& N, float Niovernt, XMVECTOR& Refracted);
float Schlick(float Cosine, float RefIdx);

class Material
{
public:
	virtual bool Scatter(const Ray& RayIn, const HitRecord& Rec, XMVECTOR& Attenuation, Ray& Scattered) const = 0;
};
#endif