/*
	Code written by James Fletcher (~2010).
	https://github.com/mrbid
*/

#ifndef SAMPLESCOPE_H
#define SAMPLESCOPE_H

#include "cWidget.h"

//SampleScope Class
class SampleScope : private Widget
{
public:
	
	//Constructor
	SampleScope(HWND hWnd, int x, int y, int w, int h, int winmsg);
	
	//Render
	void Render(HDC hdcMem, HDC hdcWin);

	//Update Sample
	void UpdateSample(float* samp, int size);

private:

	//Half widget height
	int m_hh;
	
	//Sample
	float* m_sample;
	int m_samplesize;

	//Hover State
	bool m_hoverstate;

	//Scale of how many samples the scope takes
	int m_scopesamplingrate;

	//Scale the sample in the Y axis to fit the scope dimensions
	float m_sampleheightscaler;

	//m_y + m_hh
	float m_centery;

	//Control re-drawing of the button
	bool m_chkonce;

};

__forceinline void SampleScope::UpdateSample(float* samp, int size)
{
	m_sample = samp;
	m_samplesize = size;
	m_scopesamplingrate = (m_samplesize / m_w);
	RedrawWidget();
}

#endif




