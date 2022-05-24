/*
	Code written by James Fletcher (~2010).
	https://github.com/mrbid
	
	Additive Synthesis
	   - Sine
	   - Square
	   - Saw
	   - Triangle
	   
	Wave Table Oscillator (Aliased at frequencies lower than 10)
	   - Sine
	   - Square
	   - Saw
	   - Triangle
	   
	Wave Table Oscillator (Anti-Aliased)
	   - Sine
	   - Square
	   - Saw
	   - Triangle
	   
	Digital LFO
	   - Square
	   - Saw
	   - Triangle
	   - Tap
	   - Slide (Modified B-Spline)
	   - Impulse
	   - Bipulse
	   - Random1
	   - Random2
	   - BezierRandom (Bezier Random)
	   - SplineRandom (B-Spline Random)
	   - WhiteNoise
	   - CubicNoise
	   - LinearNoise
	   - CosineNoise
	   - HermiteNoise
	
	I have no idea where I got that noise function from, but I didn't write it.
	It doesnt look very fast, so you might want to re-write it. Although it's been modified.
*/

#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "MathLib.h"


//Size of Lookup Tables
const int TABLE_SIZE = 4096;
const float TABLE_SIZE_FLOAT = float(TABLE_SIZE);
const float TABLE_SIZE_FLOAT_RECP = float(TABLE_SIZE);

const int TABLE_SIZEAA = 256;
const float TABLE_SIZEAA_FLOAT = float(TABLE_SIZEAA);
const float TABLE_SIZEAA_FLOAT_RECP = float(1.f/TABLE_SIZEAA);


//Additive Oscillator Functions & Utilities
namespace osc
{
    //Wave Shapes
    __forceinline float GetSinePoint(int point, float freq, float amplitude)
    {
        return amplitude * sin(freq * point);
    }
    
    float GetSquarePoint(int point, float freq, float amplitude, int iterations);
    float GetSawtoothPoint(int point, float freq, float amplitude, int iterations);
    float GetTrianglePoint(int point, float freq, float amplitude, int iterations);

    //Wrap around a lookup table
	__forceinline void wrap(float &in, float len)
	{
		while(in < -0.0f){in += len;}
		while(in >= len){in -= len;}
	}
	
	//Wave Table structure
	struct wtable
    {
		wtable() : amplitude(0), time(0) {}
		wtable(float t, float a) : time(t), amplitude(a) {}

        float amplitude;
        float time;
    };

	//Linear Interpolation
	__forceinline float Lerp(wtable* a, wtable* b, float t)
	{
		return a->amplitude + (((t - a->time) / (b->time - a->time)) * (b->amplitude - a->amplitude));
	}

	//Linear Interpolation AA
	__forceinline float AALerp(wtable* a, wtable* b, float t)
	{
		return a->amplitude + (((t - a->time) * TABLE_SIZEAA_FLOAT) * (b->amplitude - a->amplitude));
	}

	//Linear Interpolation LFO
	__forceinline float LFOLerp(wtable* a, wtable* b, float ts, float t)
	{
		return a->amplitude + (((t - a->time) * ts) * (b->amplitude - a->amplitude));
	}
    
    //Linear Interpolate
	__forceinline float LinearInterpolate(float a, float b, float i)
	{
		return(a * (1.f - i) + b * i);
	}
    
    //Cubic Interpolation
	__forceinline float CubicInterpolate(float y0, float y1, float y2, float y3, float mu)
	{
		const float mu2 = mu * mu;
		const float a0 = (y3 - y2 - y0 + y1);
		return a0 * mu * mu2 + (y0 - y1 - a0) * mu2 + (y2 - y0) * mu + y1;
	}
	
	//Cosine Interpolation
	__forceinline float CosineInterpolate(float y1, float y2, float mu)
	{
		const float mu2 = (1.f - cos(mu*M_PI)) * 0.5f;
		return y1 * (1.f - mu2) + y2 * mu2;
	}
	
	//Hermite Interpolation
	__forceinline float HermiteInterpolate(float y0, float y1, float y2, float y3, float mu, float tension, float bias)
	{
		const float mu2 = mu * mu;
		const float mu3 = mu2 * mu;
		return  (2.f*mu3 - 3.f*mu2 + 1.f) * y1 +
				(mu3 - 2.f*mu2 + mu) 	  * ((y1-y0)*(1.f+bias)*(1.f-tension)*0.5f + (y2-y1)*(1.f-bias)*(1.f-tension)*0.5f) +
				(mu3 - mu2) 		 	  * ((y2-y1)*(1.f+bias)*(1.f-tension)*0.5f + (y3-y2)*(1.f-bias)*(1.f-tension)*0.5f) +
				(-2.f*mu3 + 3.f*mu2) 	  * y2;
	}

	//Fast Random Function (From OGRE3D asm_math.h)
	__forceinline float frand()
	{
		static unsigned __int64 q = time(0);

		_asm
		{
			movq mm0, q
			pshufw mm1, mm0, 0x1E
			paddd mm0, mm1
			movq q, mm0
			emms
		}

		return float(q);
	}

	//Random Noise!
    __forceinline float Noise(float x)
    {
    	const int n = (int((x * 57 + rand() * 131)) << 13) ^ n;
        return (1.0f - ( (n * (n * n * 15731 + 789221) + 1376312589)&0x7fffffff) * 0.000000000931322574615478515625f);
    }

	__forceinline float Noise(int x)
    {
    	const int n = (int((x * 57 + rand() * 131)) << 13) ^ n;
        return (1.0f - ( (n * (n * n * 15731 + 789221) + 1376312589)&0x7fffffff) * 0.000000000931322574615478515625f);
    }
};


//Simple 1D Cubic Bezier Class
class CubicBezier
{
public:

	//Contructor
	CubicBezier()
	{
		p[0] = 0.f;
		p[1] = 0.f;
		p[2] = 0.f;
		p[3] = 0.f;
	}
	
	//Get's a point on the curve using Bezier Base Functions
	inline float GetBezierPoint(float t)
	{
		const float tm1 = 1.0f - t;
		const float x2tm1 = tm1 * tm1;
		const float tt = t * t;
		return (x2tm1 * tm1)   * p[0] 
			 + (3 * t * x2tm1) * p[1] 
			 + (3 * tt * tm1)  * p[2] 
			 + (tt*t)          * p[3];
	}
	
	//Get's a point on the curve using Bezier Spline Base Functions
	inline float GetBSplinePoint(float t)
	{						
		float tm1 = 1.0f - t;
		float tt = t * t;
		float ttt = tt * t;				
		return (0.1666666666666667f * (tm1 * tm1 * tm1))               * p[0] 
			 + (0.1666666666666667f * (3 * ttt - 6 * tt + 4))          * p[1] 
			 + (0.1666666666666667f * (-3 * ttt + 3 * tt + 3 * t + 1)) * p[2] 
			 + (0.1666666666666667f * ttt)                             * p[3];
	}
	
	//Modified B-Spline
    inline float GetSlidePoint(float t)
	{						
        const float tm1 = 1 - t;
		const float x3t = 3 * t;
		const float x6t = 6 * t;
		const float x3tx3t = x3t * x3t;
		const float x3tx3tx3t = x3tx3t * x3t;						
		return (0.1666666666666667f * ( tm1 * tm1 * tm1 ))               * p[0] 
			 + (0.1666666666666667f * ( x3tx3tx3t - (x6t * x6t) + 4 ))   * p[1] 
			 + (0.1666666666666667f * ( -x3tx3tx3t + x3tx3t + x3t + 1 )) * p[2] 
			 + (0.1666666666666667f * ( t*t*t ))                         * p[3];
	}
	
	__forceinline void Set(float p1, float p2, float p3, float p4)
	{
		p[0] = p1, p[1] = p2, p[2] = p3, p[3] = p4;
	}
	
	//Control Points
	float p[4];

};


//Oscillator Class
class Oscillator
{
public:

	//Generate Loopup Tables
	void Init(int numosc, int sr, int i_sqr, int i_saw, int i_tri);

	//Destructor
	Oscillator();

	//Set Phase
    void SetPhase(int oscid, float p);
	void IncrementPhase(int oscid, float freq, float i);
	void IncrementPhaseAliased(int oscid, float freq, float i);

	//Real
	float GetSineReal(int oscid, float frequency, float amplitude);
    float GetSquareReal(int oscid, float frequency, float amplitude);
    float GetSawtoothReal(int oscid, float frequency, float amplitude);
    float GetTriangleReal(int oscid, float frequency, float amplitude);

	//Aliased
    float GetSine(int oscid, float frequency, float amplitude);
    float GetSquare(int oscid, float frequency, float amplitude);
    float GetSawtooth(int oscid, float frequency, float amplitude);
    float GetTriangle(int oscid, float frequency, float amplitude);
    
	//Anti-Aliased
    float GetAASine(int oscid, float frequency, float amplitude);
    float GetAASquare(int oscid, float frequency, float amplitude);
    float GetAASawtooth(int oscid, float frequency, float amplitude);
    float GetAATriangle(int oscid, float frequency, float amplitude);

	//Low Frequency
    float GetLFOSquare(int oscid, float frequency, float amplitude);
    float GetLFOSawtooth(int oscid, float frequency, float amplitude);
    float GetLFOTriangle(int oscid, float frequency, float amplitude);
	float GetLFOTap(int oscid, float frequency, float amplitude);
    float GetLFOSlide(int oscid, float frequency, float amplitude);
	float GetLFOImpulse(int oscid, float frequency, float amplitude);
	float GetLFOBipulse(int oscid, float frequency, float amplitude);
	float GetLFORandom1(int oscid, float frequency, float amplitude);
    float GetLFORandom2(int oscid, float frequency, float amplitude);
    float GetLFORandom3(int oscid, float frequency, float amplitude);
    float GetLFORandom4(int oscid, float frequency, float amplitude);
	float GetLFOWhiteNoise(int oscid, float amplitude);
    float GetLFOCubicNoise(int oscid, float frequency, float amplitude);
    float GetLFOLinearNoise(int oscid, float frequency, float amplitude);
    float GetLFOCosineNoise(int oscid, float frequency, float amplitude);
    float GetLFOHermiteNoise(int oscid, float frequency, float amplitude, float tension, float bias);

private:

	//Each oscillator must have an individual phase
	float* m_phase;

	//Sample Rate
	float m_rsamplerate;

	//Anti-Aliased Lookup Tables
	osc::wtable m_aa_sintable[TABLE_SIZEAA];
	osc::wtable m_aa_sqrtable[TABLE_SIZEAA];
	osc::wtable m_aa_sawtable[TABLE_SIZEAA];
	osc::wtable m_aa_tritable[TABLE_SIZEAA];

	//LFO Lookup Tables
	osc::wtable m_lfo_sqrtable[4];
	osc::wtable m_lfo_sawtable[2];
	osc::wtable m_lfo_tritable[3];
	osc::wtable m_lfo_slitable[2];
	osc::wtable m_lfo_rndtable[4];
	osc::wtable m_lfo_taptable[9];
	osc::wtable m_lfo_sntable[2];
	osc::wtable m_lfo_imptable[6];
	osc::wtable m_lfo_biplstable[8];
	CubicBezier m_lfo_slide2;
	CubicBezier m_bez;

	//Aliased Lookup Tables
	float m_sintable[TABLE_SIZE];
	float m_sqrtable[TABLE_SIZE];
	float m_sawtable[TABLE_SIZE];
	float m_tritable[TABLE_SIZE];

	//Keep track of continuity for curves
	float cont;

}; 


__forceinline float Oscillator::GetSineReal(int oscid, float frequency, float amplitude)
{
	const float retval = amplitude * sin(m_phase[oscid]);
	m_phase[oscid] += (frequency * M_RADIAN_360) * m_rsamplerate;
	osc::wrap(m_phase[oscid], M_RADIAN_360);
	return retval;
}

__forceinline float Oscillator::GetSquareReal(int oscid, float frequency, float amplitude)
{
	float retval = amplitude * sin(m_phase[oscid]);
 
	for(int h = 3; h < 50; h+=2)
		 retval += amplitude/h * sin(m_phase[oscid]*h);

	m_phase[oscid] += (frequency * M_RADIAN_360) * m_rsamplerate;
	osc::wrap(m_phase[oscid], M_RADIAN_360);
	return retval;	
}

__forceinline float Oscillator::GetSawtoothReal(int oscid, float frequency, float amplitude)
{
	float retval = amplitude * sin(m_phase[oscid]);

	for(int h = 2; h < 50; h++)
		retval += amplitude/h * sin(m_phase[oscid]*h);

	m_phase[oscid] += (frequency * M_RADIAN_360) * m_rsamplerate;
	osc::wrap(m_phase[oscid], M_RADIAN_360);
	return retval;
}

__forceinline float Oscillator::GetTriangleReal(int oscid, float frequency, float amplitude)
{
	float retval = amplitude * sin(m_phase[oscid]);
	bool eo = 0; //everyother

	for(int h = 3; h < 7; h+=2)
	{
		if(eo == 1){retval += amplitude/(h*h) * sin(m_phase[oscid]*h);} 
		if(eo == 0){retval -= amplitude/(h*h) * sin(m_phase[oscid]*h);;} 
		eo = !eo;
	}

	m_phase[oscid] += (frequency * M_RADIAN_360) * m_rsamplerate;
	osc::wrap(m_phase[oscid], M_RADIAN_360);
	return retval;
}

__forceinline void Oscillator::SetPhase(int oscid, float p) // 0.f - 1.f
{
	m_phase[oscid] = p;
}

__forceinline void Oscillator::IncrementPhase(int oscid, float freq, float i)
{
	m_phase[oscid] += (freq*m_rsamplerate)*i;
}

__forceinline void Oscillator::IncrementPhaseAliased(int oscid, float freq, float i)
{
	m_phase[oscid] += ((freq*TABLE_SIZE_FLOAT)*m_rsamplerate)*i;
}

__forceinline float Oscillator::GetSine(int oscid, float frequency, float amplitude)
{
    const float retval = amplitude * m_sintable[(int)m_phase[oscid]];
    m_phase[oscid] += (frequency*TABLE_SIZE_FLOAT)*m_rsamplerate;
    osc::wrap(m_phase[oscid], TABLE_SIZE_FLOAT);
    return retval;
}

__forceinline float Oscillator::GetSquare(int oscid, float frequency, float amplitude)
{
    const float retval = amplitude * m_sqrtable[(int)m_phase[oscid]];
    m_phase[oscid] += (frequency*TABLE_SIZE_FLOAT)*m_rsamplerate;
    osc::wrap(m_phase[oscid], TABLE_SIZE_FLOAT);
    return retval;
}

__forceinline float Oscillator::GetSawtooth(int oscid, float frequency, float amplitude)
{
    const float retval = amplitude * m_sawtable[(int)m_phase[oscid]];
    m_phase[oscid] += (frequency*TABLE_SIZE_FLOAT)*m_rsamplerate;
    osc::wrap(m_phase[oscid], TABLE_SIZE_FLOAT);
    return retval;
}

__forceinline float Oscillator::GetTriangle(int oscid, float frequency, float amplitude)
{
    const float retval = amplitude * m_tritable[(int)m_phase[oscid]];
    m_phase[oscid] += (frequency*TABLE_SIZE_FLOAT)*m_rsamplerate;
    osc::wrap(m_phase[oscid], TABLE_SIZE_FLOAT);
    return retval;
}

inline float Oscillator::GetAASine(int oscid, float frequency, float amplitude)
{	
	m_phase[oscid] += frequency*m_rsamplerate;
    osc::wrap(m_phase[oscid], 1.0f);
    
	for(int i = 0; i < TABLE_SIZEAA; i++)
	{
		if(m_phase[oscid] >= m_aa_sintable[i].time && m_phase[oscid] <= m_aa_sintable[i+1].time)
        {
			return amplitude * osc::AALerp(&m_aa_sintable[i], &m_aa_sintable[i+1], m_phase[oscid]);
		}
	}
	
	if(m_phase[oscid] >= m_aa_sintable[TABLE_SIZEAA-1].time && m_phase[oscid] <= 1.0f)
    {
		return amplitude * osc::AALerp(&m_aa_sintable[TABLE_SIZEAA-1], &osc::wtable(1.f, 0.f), m_phase[oscid]);
	}
}

inline float Oscillator::GetAASquare(int oscid, float frequency, float amplitude)
{
    m_phase[oscid] += frequency*m_rsamplerate;
    osc::wrap(m_phase[oscid], 1.0f);
    
	for(int i = 0; i < TABLE_SIZEAA; i++)
	{
		if(m_phase[oscid] >= m_aa_sqrtable[i].time && m_phase[oscid] <= m_aa_sqrtable[i+1].time)
        {
			return amplitude * osc::AALerp(&m_aa_sqrtable[i], &m_aa_sqrtable[i+1], m_phase[oscid]);
		}
	}
	
	if(m_phase[oscid] >= m_aa_sqrtable[TABLE_SIZEAA-1].time && m_phase[oscid] <= 1.0f)
    {
		return amplitude * osc::AALerp(&m_aa_sqrtable[TABLE_SIZEAA-1], &osc::wtable(1.f, 0.f), m_phase[oscid]);
	}
}

inline float Oscillator::GetAASawtooth(int oscid, float frequency, float amplitude)
{
    m_phase[oscid] += frequency*m_rsamplerate;
    osc::wrap(m_phase[oscid], 1.0f);
    
	for(int i = 0; i < TABLE_SIZEAA; i++)
	{
		if(m_phase[oscid] >= m_aa_sawtable[i].time && m_phase[oscid] <= m_aa_sawtable[i+1].time)
        {
			return amplitude * osc::AALerp(&m_aa_sawtable[i], &m_aa_sawtable[i+1], m_phase[oscid]);
		}
	}
	
	if(m_phase[oscid] >= m_aa_sawtable[TABLE_SIZEAA-1].time && m_phase[oscid] <= 1.0f)
    {
		return amplitude * osc::AALerp(&m_aa_sawtable[TABLE_SIZEAA-1], &osc::wtable(1.f, 0.f), m_phase[oscid]);
	}
}

inline float Oscillator::GetAATriangle(int oscid, float frequency, float amplitude)
{
    m_phase[oscid] += frequency*m_rsamplerate;
    osc::wrap(m_phase[oscid], 1.0f);
    
	for(int i = 0; i < TABLE_SIZEAA; i++)
	{
		if(m_phase[oscid] >= m_aa_tritable[i].time && m_phase[oscid] <= m_aa_tritable[i+1].time)
        {
			return amplitude * osc::AALerp(&m_aa_tritable[i], &m_aa_tritable[i+1], m_phase[oscid]);
		}
	}
	
	if(m_phase[oscid] >= m_aa_tritable[TABLE_SIZEAA-1].time && m_phase[oscid] <= 1.0f)
    {
		return amplitude * osc::AALerp(&m_aa_tritable[TABLE_SIZEAA-1], &osc::wtable(1.f, 0.f), m_phase[oscid]);
	}
}

inline float Oscillator::GetLFOSquare(int oscid, float frequency, float amplitude)
{
    m_phase[oscid] += frequency*m_rsamplerate;
    osc::wrap(m_phase[oscid], 1.0f);
    
    if(m_phase[oscid] >= m_lfo_sqrtable[0].time && m_phase[oscid] < m_lfo_sqrtable[1].time)
    {
        return amplitude * osc::LFOLerp(&m_lfo_sqrtable[0], &m_lfo_sqrtable[1], 2.f, m_phase[oscid]);
    }
    
    if(m_phase[oscid] >= m_lfo_sqrtable[1].time && m_phase[oscid] < m_lfo_sqrtable[2].time+0.5f)
    {
        return amplitude * osc::LFOLerp(&m_lfo_sqrtable[2], &m_lfo_sqrtable[3], 2.f, m_phase[oscid]);
    }
}

__forceinline float Oscillator::GetLFOSawtooth(int oscid, float frequency, float amplitude)
{
    osc::wrap(m_phase[oscid], 1.0f);
    m_phase[oscid] += frequency*m_rsamplerate;
    
    if(m_phase[oscid] >= m_lfo_sawtable[0].time && m_phase[oscid] < m_lfo_sawtable[1].time+0.5f)
    {
        return amplitude * osc::LFOLerp(&m_lfo_sawtable[0], &m_lfo_sawtable[1], 1.f, m_phase[oscid]);
    }
}

__forceinline float Oscillator::GetLFOTriangle(int oscid, float frequency, float amplitude)
{
    m_phase[oscid] += frequency*m_rsamplerate;
    osc::wrap(m_phase[oscid], 1.0f);
    
    if(m_phase[oscid] >= m_lfo_tritable[0].time && m_phase[oscid] < m_lfo_tritable[1].time)
    {
        return amplitude * osc::LFOLerp(&m_lfo_tritable[0], &m_lfo_tritable[1], 2.f, m_phase[oscid]);
    }
    
    if(m_phase[oscid] >= m_lfo_tritable[1].time && m_phase[oscid] <= m_lfo_tritable[2].time+0.5f) //Hack
    {
        return amplitude * osc::LFOLerp(&m_lfo_tritable[1], &m_lfo_tritable[2], 2.f, m_phase[oscid]);
    }
}

__forceinline float Oscillator::GetLFOSlide(int oscid, float frequency, float amplitude)
{
    m_phase[oscid] -= frequency*m_rsamplerate;
	osc::wrap(m_phase[oscid], 1.0f);
    return amplitude * (m_lfo_slide2.GetSlidePoint(m_phase[oscid]) + 0.5f);
}

__forceinline float Oscillator::GetLFOImpulse(int oscid, float frequency, float amplitude)
{
    m_phase[oscid] += frequency*m_rsamplerate;
    osc::wrap(m_phase[oscid], 1.0f);
    
    if(m_phase[oscid] >= m_lfo_imptable[0].time && m_phase[oscid] < m_lfo_imptable[1].time)
    {
        return amplitude * osc::LFOLerp(&m_lfo_imptable[0], &m_lfo_imptable[1], 1.3f, m_phase[oscid]);
    }
    
    if(m_phase[oscid] >= m_lfo_imptable[2].time && m_phase[oscid] < m_lfo_imptable[3].time)
    {
        return amplitude * osc::LFOLerp(&m_lfo_imptable[2], &m_lfo_imptable[3], 100.f, m_phase[oscid]);
    }

	if(m_phase[oscid] >= m_lfo_imptable[4].time && m_phase[oscid] < m_lfo_imptable[5].time+0.5f)
    {
        return amplitude * osc::LFOLerp(&m_lfo_imptable[4], &m_lfo_imptable[5], 4.2f, m_phase[oscid]);
    }
}

__forceinline float Oscillator::GetLFOBipulse(int oscid, float frequency, float amplitude)
{
    m_phase[oscid] += frequency*m_rsamplerate;
    osc::wrap(m_phase[oscid], 1.0f);
    
    if(m_phase[oscid] >= m_lfo_biplstable[0].time && m_phase[oscid] < m_lfo_biplstable[1].time)
    {
        return amplitude * osc::LFOLerp(&m_lfo_biplstable[0], &m_lfo_biplstable[1], 2.9f, m_phase[oscid]);
    }
    
    if(m_phase[oscid] >= m_lfo_biplstable[2].time && m_phase[oscid] < m_lfo_biplstable[3].time)
    {
        return amplitude * osc::LFOLerp(&m_lfo_biplstable[2], &m_lfo_biplstable[3], 5.9f, m_phase[oscid]);
    }

	if(m_phase[oscid] >= m_lfo_biplstable[4].time && m_phase[oscid] < m_lfo_biplstable[5].time)
    {
        return amplitude * osc::LFOLerp(&m_lfo_biplstable[4], &m_lfo_biplstable[5], 5.9f, m_phase[oscid]);
    }

	if(m_phase[oscid] >= m_lfo_biplstable[6].time && m_phase[oscid] < m_lfo_biplstable[7].time+0.5f)
    {
        return amplitude * osc::LFOLerp(&m_lfo_biplstable[6], &m_lfo_biplstable[7], 2.9f, m_phase[oscid]);
    }
}

__forceinline float Oscillator::GetLFORandom3(int oscid, float frequency, float amplitude)
{
	m_phase[oscid] += frequency*m_rsamplerate;
	
	while(m_phase[oscid] < 0.f){m_phase[oscid] += 1.0f;}
	while(m_phase[oscid] >= 1.0f)
    {
        const float ncont = osc::Noise(m_phase[oscid]);
		m_bez.Set(cont, osc::Noise(m_phase[oscid]), osc::Noise(m_phase[oscid]), ncont);
    	cont = ncont;
		m_phase[oscid] -= 1.0f;
	}
	
    return amplitude * m_bez.GetBezierPoint(m_phase[oscid]);
}

__forceinline float Oscillator::GetLFORandom4(int oscid, float frequency, float amplitude)
{
	m_phase[oscid] += frequency*m_rsamplerate;
	
	while(m_phase[oscid] < 0.f){m_phase[oscid] += 1.0f;}
	while(m_phase[oscid] >= 1.0f)
    {
        const float ncont = osc::Noise(m_phase[oscid]);
	    m_bez.Set(cont, osc::Noise(m_phase[oscid]), osc::Noise(m_phase[oscid]), ncont);
    	cont = ncont;
		m_phase[oscid] -= 1.0f;
	}
	
    return amplitude * m_bez.GetBSplinePoint(m_phase[oscid]);
}

__forceinline float Oscillator::GetLFOWhiteNoise(int oscid, float amplitude)
{
	//return amplitude * osc::Noise(osc::frand()); //(this 40 times faster requires SSE2)
	return amplitude * osc::Noise(rand());
}

inline float Oscillator::GetLFOCubicNoise(int oscid, float frequency, float amplitude)
{
    m_phase[oscid] += frequency*m_rsamplerate;
    
    while(m_phase[oscid] < 0.f){m_phase[oscid] += 1.0f;}
	while(m_phase[oscid] >= 1.0f)
    {
		const float ncont = osc::Noise(m_phase[oscid]);
        m_lfo_sntable[0].amplitude = cont;
        m_lfo_sntable[1].amplitude = ncont;
        cont = ncont;
		m_phase[oscid] -= 1.0f;
	}
    
    if(m_phase[oscid] >= m_lfo_sntable[0].time && m_phase[oscid] < m_lfo_sntable[1].time)
    {
        return amplitude * osc::CubicInterpolate(-1, m_lfo_sntable[0].amplitude, m_lfo_sntable[1].amplitude, 1, m_phase[oscid]);
    }
}

inline float Oscillator::GetLFOLinearNoise(int oscid, float frequency, float amplitude)
{
    m_phase[oscid] += frequency*m_rsamplerate;
    
    while(m_phase[oscid] < 0.f){m_phase[oscid] += 1.0f;}
	while(m_phase[oscid] >= 1.0f)
    {
		const float ncont = osc::Noise(m_phase[oscid]);
        m_lfo_sntable[0].amplitude = cont;
        m_lfo_sntable[1].amplitude = ncont;
        cont = ncont;
		m_phase[oscid] -= 1.0f;
	}
    
    if(m_phase[oscid] >= m_lfo_sntable[0].time && m_phase[oscid] < m_lfo_sntable[1].time)
    {
        return amplitude * osc::LinearInterpolate(m_lfo_sntable[0].amplitude, m_lfo_sntable[1].amplitude, m_phase[oscid]);
    }
}

inline float Oscillator::GetLFOCosineNoise(int oscid, float frequency, float amplitude)
{
    m_phase[oscid] += frequency*m_rsamplerate;
    
    while(m_phase[oscid] < 0.f){m_phase[oscid] += 1.0f;}
	while(m_phase[oscid] >= 1.0f)
    {
		const float ncont = osc::Noise(m_phase[oscid]);
        m_lfo_sntable[0].amplitude = cont;
        m_lfo_sntable[1].amplitude = ncont;
        cont = ncont;
		m_phase[oscid] -= 1.0f;
	}
    
    if(m_phase[oscid] >= m_lfo_sntable[0].time && m_phase[oscid] < m_lfo_sntable[1].time)
    {
        return amplitude * osc::CosineInterpolate(m_lfo_sntable[0].amplitude, m_lfo_sntable[1].amplitude, m_phase[oscid]);
    }
}

inline float Oscillator::GetLFOHermiteNoise(int oscid, float frequency, float amplitude, float tension, float bias)
{
    m_phase[oscid] += frequency*m_rsamplerate;
    
    while(m_phase[oscid] < 0.f){m_phase[oscid] += 1.0f;}
	while(m_phase[oscid] >= 1.0f)
    {
		const float ncont = osc::Noise(m_phase[oscid]);
        m_lfo_sntable[0].amplitude = cont;
        m_lfo_sntable[1].amplitude = ncont;
        cont = ncont;
		m_phase[oscid] -= 1.0f;
	}
    
    if(m_phase[oscid] >= m_lfo_sntable[0].time && m_phase[oscid] < m_lfo_sntable[1].time)
    {
        return amplitude * osc::HermiteInterpolate(-1, m_lfo_sntable[0].amplitude, m_lfo_sntable[1].amplitude, 1, m_phase[oscid], 0.5f, 0.5f);
    }
}


#endif








