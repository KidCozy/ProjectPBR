#pragma once

#ifndef RANDOMH
#define RANDOMH

#include<cstdlib>

inline double RadnomDouble()
{
	return rand() / (RAND_MAX + 1.0f);
}

#endif