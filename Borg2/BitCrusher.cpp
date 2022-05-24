/*
	Code written by James Fletcher (~2010).
	https://github.com/mrbid
*/

#include "BitCrusher.h"
#include "Oscillator.h"


//Do BitCrush
float BitCrusher::DoBitCrush(float in, float* m_fsample, int i)
{
	//Keep track of how many samples we have progressed
	m_bc++;

	//Have we passed our sample loss increment?
	if(m_bc > m_crush)
	{
		m_bc = 0; //Reset

		//If we are interpolating between sample loss we need to back track
		if(m_type != 0)
		{
			for(int i2 = i-m_crush; i2 < i; i2++)
			{
				float r = 1.f - ((i-i2) * m_rBCV);
				
				if(r > 1.f)
					r = 1.f;

				if(m_type == 1)
				{
					//m_fsample[i2] = osc::CubicInterpolate(-1.f, m_ls, in, 1.f, r);
					m_fsample[i2] = osc::CosineInterpolate(m_ls, in, r);
				}
				else
				{
					if(m_type == 2)
						m_fsample[i2] = osc::LinearInterpolate(m_ls, in, r);
				}
			}
		}

	}
	else
	{
		//If there is no interpolation, just return the last recorded sample.
		if(m_type == 0)
			return m_ls;

		//Return anything, it's going to get written over by interpolation anyway.
		return 0.f;
	}

	//Backup last sample for bit crusher
	m_ls = in;

	//Return input, nothing happened!
	return in;
}




