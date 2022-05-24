/*
	Code written by James Fletcher (~2010).
	https://github.com/mrbid
*/

#include "BitReflecter.h"
#include "Oscillator.h"
#include <stdio.h>
#include <string.h>

//Do Bitstretch
void BitReflecter::DoBitReflect(float *m_fsample, long len)
{
	//Reflect Left & Higher Freq
	if(m_type == 0)
	{
		for(int l = 0; l < m_stretch; ++l)
		{
			for(long i = l; i < len-(l+1); ++i)
			{
				const float temp = m_fsample[i];
				m_fsample[i] = m_fsample[i+1];
				m_fsample[i+1] = temp;
			}
		}

		return;
	}

	//Sharper
	if(m_type == 1)
	{
		for(int l = 0; l < m_stretch; ++l)
		{
			for(long i = l; i < len-(m_stretch+1); ++i)
			{
				const float temp = m_fsample[i];
				m_fsample[i] = m_fsample[i+m_stretch];
				m_fsample[i+m_stretch] = temp;
			}
		}

		return;
	}

	//Glitcher
	if(m_type == 2)
	{
		for(int l = 0; l < m_stretch; ++l)
		{
			for(long i = 0; i < len-(l+1); ++i)
			{
				const float temp = m_fsample[i];
				m_fsample[i] = m_fsample[i+l];
				m_fsample[i+l] = temp;
			}
		}

		return;
	}
}


/*
	if(m_type == 1)
	{
		//for(int l = 0; l < m_stretch; ++l)
		//{
			for(long i = 0; i < len-(m_stretch+1); ++i)
			{
				const float temp = m_fsample[i];
				m_fsample[i] = m_fsample[i+m_stretch];
				m_fsample[i+m_stretch] = temp;
			}
		//}

		return;
	}
*/


	//Reflect Left & Right
	/*if(m_type == 1)
	{
		bool s = false;

		for(int l = 0; l < m_stretch; ++l)
		{
			s = !s;

			if(s == true)
			{
				for(long i = l; i < len-(l+1); ++i)
				{
					const float temp = m_fsample[i];
					m_fsample[i] = m_fsample[i+1];
					m_fsample[i+1] = temp;
				}
			}

			if(s == false)
			{
				for(long i = len-(l+1); i > l; --i)
				{
					const float temp = m_fsample[i];
					m_fsample[i] = m_fsample[i-1];
					m_fsample[i-1] = temp;
				}
			}
		}

		return;
	}*/
