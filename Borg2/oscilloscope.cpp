/*
	Code written by James Fletcher (~2010).
	https://github.com/mrbid
*/

#include "Oscilloscope.h"

//Constructor
Oscilloscope::Oscilloscope()
{
    m_freqstep = 0;
    m_frameready = 0;
    m_halfscreen=0;
}

//Destructor
Oscilloscope::~Oscilloscope()
{
    DeleteObject(m_surf);
}
 
//Initialize Scope   
void Oscilloscope::Init(HDC dc, int x, int y, int maxfrequency, int maxamplitude, COLORREF col)
{
	ZeroMemory(&m_bmi, sizeof(BITMAPINFO));
	m_bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_bmi.bmiHeader.biWidth = maxfrequency;
	m_bmi.bmiHeader.biHeight = -maxamplitude; //-?
	m_bmi.bmiHeader.biPlanes = 1;
	m_bmi.bmiHeader.biBitCount = 32;
	m_bmi.bmiHeader.biCompression = BI_RGB;

	m_surf = CreateDIBSection(dc, &m_bmi, DIB_RGB_COLORS, NULL, NULL, 0);

    m_colour = col;
    m_halfscreen = maxamplitude/2;
	ly = 0;

	m_x = x, m_y = y;
}

//Render Scope
void Oscilloscope::Render(HDC hdcMem, HDC hdcWin)
{
	if(m_surf == NULL){return;}
    
    if(m_frameready == true)
    {
		SelectObject(hdcMem, m_surf);
		BitBlt(hdcWin, m_x, m_y, m_bmi.bmiHeader.biWidth, m_bmi.bmiHeader.biHeight, hdcMem, 0, 0, SRCCOPY);
	}
}

//Add a step to the scope
void Oscilloscope::AddStep(int iy)
{
    if(m_frameready == true){return;}
    if(m_surf == NULL){return;}
    
	//hack, if maxamplitude is 100 it will scale a -128 to +127 waveform to -50 to +50
    const int y = (iy*0.392156862745098f); 
    //const int y = iy/(170/halfscreen);
    
	//Draw Step
    dda_line(m_freqstep-1, ly, m_freqstep, m_halfscreen + y, m_colour);
    ly = m_halfscreen + y;
    m_freqstep++;
    
    //New Osc Frame
	if(m_freqstep >= m_bmi.bmiHeader.biWidth)
    {
        m_frameready = 1;
    }
}

//Resets Scope
void Oscilloscope::Reset()
{
	RECT r;
	r.left = 0;
	r.top = 0;
	r.bottom = m_bmi.bmiHeader.biHeight;
	r.right = m_bmi.bmiHeader.biWidth;
	HDC hdc = CreateCompatibleDC(0);
	SelectObject(hdc, m_surf);
    FillRect(hdc, &r, 0);
	DeleteDC(hdc);
    m_freqstep = 0;
    m_frameready = 0;
}





