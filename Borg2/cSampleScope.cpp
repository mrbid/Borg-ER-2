/*
	Code written by James Fletcher (~2010).
	https://github.com/mrbid
*/

#include "cSampleScope.h"


//Constructor
SampleScope::SampleScope(HWND hWnd, int x, int y, int w, int h, int winmsg)
{
	//Set Windows Message ID
	m_winmsg = winmsg;

	//Set Handle Window
	m_hwnd = hWnd;

	//Set position and dimensions
	m_x = x, m_y = y, m_w = w, m_h = h, m_hh = h/2;

	//Scale the sample in the Y axis to fit the scope dimensions
	m_sampleheightscaler = (m_hh * 0.0078125f);

	//Center of scope in Y axis
	m_centery = (m_y + m_hh);

	//Reset Sample
	m_samplesize = 0;
}

//Render
void SampleScope::Render(HDC hdcMem, HDC hdcWin)
{
	if(m_visible == 0){return;}

	if(m_sample == NULL || m_samplesize == 0)
		return;

	for(int i = 0, lx = 0, ly = m_centery; i < m_w; i++)
	{
		HPEN pen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		SelectObject(hdcWin, pen);

		MoveToEx(hdcWin, lx, ly, NULL);
		lx = m_x + i;

		if(m_samplesize <= m_w)
		{
			if(i > m_samplesize)
				ly = m_centery;
			else
				ly = m_centery + (m_sample[i] * m_sampleheightscaler);
		}
		else
		{
			ly = m_centery + (m_sample[i*m_scopesamplingrate] * m_sampleheightscaler);
		}

		//Clipping
		if(ly < m_y)
			ly = m_y;

		if(ly > m_y+m_h-2)
			ly = m_y+m_h-2;
		
		LineTo(hdcWin, lx, ly);

		DeleteObject(pen);
	}
}


