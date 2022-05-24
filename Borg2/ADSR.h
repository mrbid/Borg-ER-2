/*
	Code written by James Fletcher (~2010).
	https://github.com/mrbid

	Total sample time should = iAv + iDv + iSv + iRv
*/

#ifndef ADSR_H
#define ADSR_H

#include "Oscillator.h"

//ADSR Class
class ADSR
{
public:

	//Constructor
	ADSR();
	
	//Functions
	void SetADSR(float iAv, float iDv, float iSv, float iSLv, float iRv, int m_samplerate);
	float CalcADSRSmooth(float in, int i);
	float CalcADSRLinear(float in, int i);

private:

	//Samples for smooth adsr
	CubicBezier m_bezattack, m_bezdecay; //one is just 1.0- the other

	//ADSR Parameters
	int m_Av, m_Dv, m_Sv, m_SLv, m_Rv;

	//Some pre-calculated variables
	float m_rAv, m_rDv, m_rRv;
	float m_sustainscale;
	float m_SLvXss;

	//Tracking Variables
	char m_stage;
	int m_li; //last index

};

#endif


