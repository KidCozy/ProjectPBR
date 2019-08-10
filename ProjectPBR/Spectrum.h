#pragma once
#include<D3D11.h>
class Spectrum
{
protected:
	ID3D11Texture2D* Sample;
public:
	Spectrum();
	virtual ~Spectrum();
};

