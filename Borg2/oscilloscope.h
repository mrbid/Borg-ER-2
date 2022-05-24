/*
	Code written by James Fletcher (~2010).
	https://github.com/mrbid
*/

#ifndef OSCILLOSCOPE_H
#define OSCILLOSCOPE_H

#include <math.h>
#include <windows.h>

//Oscilloscope
class Oscilloscope
{
public:

	//Constructor & Destructor
    Oscilloscope();
    ~Oscilloscope();
    
    //Functions
    void Init(HDC dc, int x, int y, int maxamplitude, int maxfrequency, COLORREF col);
    void Render(HDC hdcMem, HDC hdcWin);
    void AddStep(int y);
    void Reset();
       
private:

	inline void dda_line(float x1, float y1, float x2, float y2, COLORREF colour)
	{    
		float x = x1, y = y1;
		const float dx = x2-x1;
		const float dy = y2-y1;
		const float m = fabs(dx) >= fabs(dy) ? fabs(dx) : fabs(dy);

		HDC hdc = CreateCompatibleDC(0);
		SelectObject(hdc, m_surf);

	    SetPixelV(hdc, x, y, colour);
		for(int i = 1; i <= m; i++)
		{
			x = x+dx/m;
			y = y+dy/m;
			SetPixelV(hdc, x, y, colour);
		}

		DeleteDC(hdc);
	}

	//Local Screen Space Position
	int m_x, m_y;
	
	//Graphics
    HBITMAP m_surf;
	BITMAPINFO m_bmi;
    COLORREF m_colour;

	//Variables
    int m_freqstep;
    bool m_frameready;
    int m_halfscreen;
	float ly;
};

#endif
