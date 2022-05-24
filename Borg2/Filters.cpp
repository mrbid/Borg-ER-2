/*
	Code written by James Fletcher (~2010).
	https://github.com/mrbid
*/

#include "Filters.h"
#include "MathLib.h"
#include <math.h>

//Init Filters
void Filters::InitFilters(float samplerate)
{
	m_samplerate = samplerate;
	m_rsamplerate = 1.f/m_samplerate;
}

//Setup the high boost filter
void Filters::SetHighBoost(float freq, float boost)
{
	m_highboost.Reset();
	
	const float omega = x2PI * freq * m_rsamplerate;
	const float sn = sin(omega);
	const float cs = cos(omega);
	const float A = exp(log(10.f) * boost * 0.025f);
	const float beta = math::isqrt(A + A);

	m_highboost.b0 = A * ((A + 1.f) + (A - 1.f) * cs + beta * sn);
    m_highboost.b1 = -2.f * A * ((A - 1.f) + (A + 1.f) * cs);
    m_highboost.b2 = A * ((A + 1.f) + (A - 1.f) * cs - beta * sn);
    m_highboost.a1 = 2.f * ((A - 1.f) - (A + 1.f) * cs);
    m_highboost.a2 = (A + 1.f) - (A - 1.f) * cs - beta * sn;
	const float ra0 = 1.0f / ((A + 1.f) - (A - 1.f) * cs + beta * sn);

	m_highboost.b0 *= ra0;
	m_highboost.b1 *= ra0;
	m_highboost.b2 *= ra0;
	m_highboost.a1 *= ra0;
	m_highboost.a2 *= ra0;
}

//Setup the low boost filter
void Filters::SetLowBoost(float freq, float boost)
{
	m_lowboost.Reset();
	
	const float omega = x2PI * freq * m_rsamplerate;
	const float sn = sin(omega);
	const float cs = cos(omega);
	const float A = exp(log(10.f) * boost * 0.025f);
	const float beta = math::isqrt(A + A);

	m_lowboost.b0 = A * ((A + 1.f) - (A - 1.f) * cs + beta * sn);
	m_lowboost.b1 = 2.f * A * ((A - 1.f) - (A + 1.f) * cs);
	m_lowboost.b2 = A * ((A + 1.f) - (A - 1.f) * cs - beta * sn);
	m_lowboost.a1 = -2.f * ((A - 1.f) + (A + 1.f) * cs);
	m_lowboost.a2 = (A + 1.f) + (A - 1.f) * cs - beta * sn;
	const float ra0 = 1.0f / ((A + 1.f) + (A - 1.f) * cs + beta * sn);

	m_lowboost.b0 *= ra0;
	m_lowboost.b1 *= ra0;
	m_lowboost.b2 *= ra0;
	m_lowboost.a1 *= ra0;
	m_lowboost.a2 *= ra0;
}

//Set Cutoff Filter Parameters
void Filters::SetLowPass(float cutoff, float resonance)
{
	m_cutoff.Reset();

	const float bandwidth = 1.0f;

    const float omega = x2PI * cutoff * m_rsamplerate;
    const float sn = sin(omega);
    const float cs = cos(omega);

    //alpha = sn / resonance;
    const float alpha = (sn / resonance) * sinh(M_LN2_2 * bandwidth * (omega / sn));
	//alpha = sn * sinh(M_LN2 /2 * bandwidth * omega /sn);

	m_cutoff.b0 = (1.f - cs) * 0.5f;
    m_cutoff.b1 = 1.f - cs;
    m_cutoff.b2 = (1.f - cs) * 0.5f;
    m_cutoff.a1 = -2.f * cs;
    m_cutoff.a2 = 1.f - alpha;
	const float ra0 = 1.0f / (1.f + alpha);

	m_cutoff.b0 *= ra0;
	m_cutoff.b1 *= ra0;
	m_cutoff.b2 *= ra0;
	m_cutoff.a1 *= ra0;
	m_cutoff.a2 *= ra0;
}

//Set Highpass Frequency
void Filters::SetHighPass(float freq, float resonance)
{
	m_highpass.Reset();

	const float bandwidth = 1.f;

    const float omega = x2PI * freq * m_rsamplerate;
    const float sn = sin(omega);
    const float cs = cos(omega);
	const float alpha = (sn / resonance) * sinh(M_LN2_2 * bandwidth * omega / sn);

	m_highpass.b0 = (1.f + cs) * 0.5f;
    m_highpass.b1 = -(1.f + cs);
    m_highpass.b2 = (1.f + cs) * 0.5f;
    m_highpass.a1 = -2.f * cs;
    m_highpass.a2 = 1.f - alpha;
	const float ra0 = 1.0f / (1.f + alpha);

	m_highpass.b0 *= ra0;
	m_highpass.b1 *= ra0;
	m_highpass.b2 *= ra0;
	m_highpass.a1 *= ra0;
	m_highpass.a2 *= ra0;
}

//Set BandPass filter
void Filters::SetBandStop(float freq, float resonance, float range)
{
	m_bandstop.Reset();

    const float omega = x2PI * freq * m_rsamplerate;
    const float sn = sin(omega);
    const float cs = cos(omega);
	const float alpha = (sn / resonance) * sinh(M_LN2_2 * range * omega / sn);

	m_bandstop.b0 = 1.f;
    m_bandstop.b1 = -2.f * cs;
    m_bandstop.b2 = 1.f;
    m_bandstop.a1 = -2.f * cs;
    m_bandstop.a2 = 1.f - alpha;
	const float ra0 = 1.0f / (1.f + alpha);

	m_bandstop.b0 *= ra0;
	m_bandstop.b1 *= ra0;
	m_bandstop.b2 *= ra0;
	m_bandstop.a1 *= ra0;
	m_bandstop.a2 *= ra0;
}




