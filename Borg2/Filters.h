/*
	Code written by James Fletcher (~2010).
	https://github.com/mrbid

	If parallelized these filters could throw unexpected results.
*/

#ifndef FILTERS_H
#define FILTERS_H

//Filter Structure
struct IIRFilter
{
	float BilinearTransform(float in);
	void Reset();

	float xn1, xn2, yn1, yn2;
	float b0, b1, b2, a1, a2;
};

__forceinline float IIRFilter::BilinearTransform(float in)
{
	const float out =   b0 * in
						+ b1 * xn1
						+ b2 * xn2
						- a1 * yn1
						- a2 * yn2;

	xn2 = xn1;
	xn1 = in;
	yn2 = yn1;
	yn1 = out;
	return out;
}

__forceinline void IIRFilter::Reset()
{
	xn1 = 0, xn2 = 0, yn1 = 0, yn2 = 0;
}


//Filters Class
class Filters
{
public:

	//Init Filters
	void InitFilters(float samplerate);

	//High Boost Filter
	void SetHighBoost(float freq, float boost);
	float DoHighBoost(float in);

	//Low Boost Filter
	void SetLowBoost(float freq, float boost);
	float DoLowBoost(float in);

	//Cutoff & Resonance Filter
	void SetLowPass(float cutoff, float resonance);
	float DoLowPass(float in);

	//Highpass Filter
	void SetHighPass(float freq, float resonance);
	float DoHighPass(float in);

	//Band Stop Filter
	void SetBandStop(float freq, float resonance, float range);
	float DoBandStop(float in);

private:

	//Samplerate
	float m_samplerate;
	float m_rsamplerate;

	//Filter Settings
	IIRFilter m_lowboost;
	IIRFilter m_highboost;
	IIRFilter m_cutoff;
	IIRFilter m_highpass;
	IIRFilter m_bandstop;

};

//Do Highpass
__forceinline float Filters::DoBandStop(float in)
{
	return m_highpass.BilinearTransform(in);
}
//Do Highpass
__forceinline float Filters::DoHighPass(float in)
{
	return m_highpass.BilinearTransform(in);
}

//Do the cutoff filter
__forceinline float Filters::DoLowPass(float in)
{
	return m_cutoff.BilinearTransform(in);
}

//Calculate HighBoost Filter
__forceinline float Filters::DoHighBoost(float in)
{
	return m_highboost.BilinearTransform(in);
}

//Calculate LowBoost Filter
__forceinline float Filters::DoLowBoost(float in)
{
	return m_lowboost.BilinearTransform(in);
}


#endif


