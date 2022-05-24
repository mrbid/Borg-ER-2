/*
	Code written by James Fletcher (~2010).
	https://github.com/mrbid
*/

#include "ADSR.h"


//Constructor
ADSR::ADSR()
{
	//Init attack and release beziers
	m_bezattack.Set(0.f, 1.f, 1.0f, 1.f);
	m_bezdecay.Set(1.f, 0.f, 0.f, 0.f);
}

//Set ADSR Parameters
void ADSR::SetADSR(float iAv, float iDv, float iSv, float iSLv, float iRv, int samplerate)
{
	//Get parameter values
	m_Av = (samplerate * iAv * 2.f);
	m_Dv = (samplerate * iDv * 2.f);
	m_Sv = (samplerate * iSv * 2.f);
	m_SLv = iSLv;
	m_Rv = (samplerate * iRv * 2.f);

	//Pre-calculate some variables to speed up the functions
	m_rAv = 1.f / m_Av;
	m_rDv = 1.f / m_Dv;
	m_rRv = 1.f / m_Rv;
	m_sustainscale = m_SLv * float(0.003921568f); //SLv / 255.f;
	m_SLvXss = m_SLv * m_sustainscale;

	//Reset ADSR
	m_stage = 0;
	m_li = 0;
}

//Scale the sample using a bezier ADSR
float ADSR::CalcADSRSmooth(float in, int m_ci)
{
	//Release Stage
	if(m_stage == 3)
	{
		if(m_ci-m_li < m_Rv)
			return in * m_bezdecay.GetBezierPoint(m_rRv * (m_ci-m_li)) * m_sustainscale;

		m_stage = 4;
		m_li = m_ci;
		return CalcADSRSmooth(in, m_ci);
	}

	//Sustain Stage
	if(m_stage == 2)
	{
		if(m_ci-m_li < m_Sv)
			return in * m_sustainscale;

		m_stage = 3;
		m_li = m_ci;
		return CalcADSRSmooth(in, m_ci);
	}

	//Decay Stage
	if(m_stage == 1)
	{
		if(m_ci-m_li < m_Dv)
		{
			const float decayscalar = m_bezdecay.GetBezierPoint(m_rDv * (m_ci-m_li));

			//Time to kill the decay? (nasty)
			if(decayscalar <= m_sustainscale)
			{
				m_stage = 2;
				m_li = m_ci;
				return CalcADSRSmooth(in, m_ci);
			}

			return in * decayscalar;
		}

		m_stage = 2;
		m_li = m_ci;
		return CalcADSRSmooth(in, m_ci);
	}

	//Attack Stage
	if(m_stage == 0)
	{
		if(m_ci < m_Av)
			return in * m_bezattack.GetBezierPoint(m_rAv * m_ci);

		m_stage = 1;
		m_li = m_ci;
		return CalcADSRSmooth(in, m_ci);
	}

	//Final Stage (silent)
	if(m_stage == 4)
		return 0.f;
}

//Scale the sample using a linear ADSR
float ADSR::CalcADSRLinear(float in, int m_ci)
{
	//Release Stage
	if(m_stage == 3)
	{
		if(m_ci-m_li < m_Rv)
			return in * (1.f-(m_rRv * (m_ci-m_li))) * m_sustainscale;

		m_stage = 4;
		m_li = m_ci;
		return CalcADSRLinear(in, m_ci);
	}

	//Sustain Stage
	if(m_stage == 2)
	{
		if(m_ci-m_li < m_Sv)
			return in * m_sustainscale;

		m_stage = 3;
		m_li = m_ci;
		return CalcADSRLinear(in, m_ci);
	}

	//Decay Stage
	if(m_stage == 1)
	{
		if(m_ci-m_li < m_Dv)
		{
			const float decayscalar = (1.f-(m_rDv * (m_ci-m_li)));

			//Time to kill the decay? (hmm)
			if(decayscalar <= m_sustainscale)
			{
				m_stage = 2;
				m_li = m_ci;
				return CalcADSRLinear(in, m_ci);
			}

			return in * decayscalar;
		}

		m_stage = 2;
		m_li = m_ci;
		return CalcADSRLinear(in, m_ci);
	}

	//Attack Stage
	if(m_stage == 0)
	{
		if(m_ci < m_Av)
			return in * (m_rAv * m_ci);

		m_stage = 1;
		m_li = m_ci;
		return CalcADSRLinear(in, m_ci);
	}

	//Final Stage (silent)
	if(m_stage == 4)
		return 0.f;
}


