/*
	Code written by James Fletcher (~2010).
	https://github.com/mrbid
*/

#ifndef MATHLIB_H
#define MATHLIB_H

#include <xmmintrin.h>
#include <cmath>

const float x2PI = 6.283185307178f;
const float M_RADIAN = 0.0174532925f;
const float M_RADIAN_360 = 6.28318531f;
const float M_PI = 3.141592653589f;
const float M_PI_2 = 1.570796326794f;
const float M_LN2 = 0.69314718055994530942f;
const float M_LN2_2 = 0.34657359027997265471f;

namespace math
{
	//Degrees to Radians
	__forceinline float Degree(float degrees)
	{
		return degrees * M_RADIAN;
	}

	//Wrap Functon
	__forceinline float wrap(float f)
	{
		if(f < 0.f){return f + 1.0f;}
		if(f > 1.f){return f - 1.0f;}
		return f;
	}

	//Clamp Function (float)
	__forceinline float clamp(float f)
	{
		if(f < 0.f){return 0.0f;}
		if(f > 1.f){return 1.0f;}
		return f;
	}

	//Fit Function from (-1 - 1) to (0 - 1)
	__forceinline float fit(float f)
	{
		return (f + 1.0f) * 0.5f;
	}

	//InvSqrt (Quake 3)
	__forceinline float InvSqrt(float x)
	{
		float xhalf = 0.5f*x;
		int i = *(int*)&x;
		i = 0x5f3759df - (i>>1);
		x = *(float*)&i;
		x = x*(1.5f - xhalf*x*x);
		return x;
	}

	//Fast Sqrt using InvSqrt()
	__forceinline float fsqrt(float f)
	{
		return f * InvSqrt(f);
	}

	//Fast Square Root (wikipedia)
	__forceinline float fastsqrt(float f)
	{
        union{
			int tmp;
			float val;
        } u;
        u.val = f;
        u.tmp -= 1<<23;
        u.tmp >>= 1;
        u.tmp += 1<<29;
        return u.val;
	}

	//Intrinsic Sqrt
	__forceinline float isqrt(float f)
	{
		return _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(f)));
	}

	//Intrinsic Reciprocal Sqrt
	__forceinline float irsqrt(float f)
	{
		return _mm_cvtss_f32(_mm_rsqrt_ss(_mm_set_ss(f)));
	}

	//SmoothStep Interpolation
	__forceinline float SmoothStep(float a, float b, float i)
	{
		if(i <= a){return 0.0f;}
		if(i >= b){return 1.0f;}
		const float v = i - a / b - a;
		return v * v * (3.f - 2.f * v);
	}

	//saturate
	__forceinline float saturate(float in)
	{
		if(in > 1.0f)
			return 1.0f;

		if(in < 0.f)
			return 0.f;
	}

	//SmoothStep Interpolation
	__forceinline float SmoothStep2(float a, float b, float i)
	{
		const float v = i - a / b - a;
		return v * v * (3.f - 2.f * v);
	}

	//Fast sine approximate
	__forceinline float fsin1(float x)
	{
		return 1.273239544f * x + -0.636619772f * x * abs(x);
	}

	//Fast sine approximate
	__forceinline float fsin2(float x)
	{
		float res = 0, pow = x, fact = 1;

		for(int i = 0; i < 5; ++i)
		{
			res += pow / fact;
			pow *= x * x;
			fact *= (2*(i+1))*(2*(i+1)+1);
		}

		return res;
	}

};

#endif








