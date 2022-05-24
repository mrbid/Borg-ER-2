#ifndef FLOATCAST_H
#define FLOATCAST_H

#include <math.h>

__forceinline long int lrint(double flt)
{
	int intgr;

	_asm
	{
		fld flt
		fistp intgr
	};

	return intgr;
}

__forceinline long int lrintf(float flt)
{
	int intgr;

	_asm
	{
		fld flt
		fistp intgr
	};

	return intgr;
}

#endif




