/*
	Code written by James Fletcher (~2010).
	https://github.com/mrbid
*/

#ifndef GFXBOX_H
#define GFXBOX_H

#include "cWidget.h"

//GfxBox Class
class GfxBox : private Widget
{
public:
	
	//Constructor
	GfxBox(HWND hWnd, int x, int y, int startvalue, int* graphic_array, int graphic_array_size, int winmsg);

	//Destructor
	~GfxBox();
	
	//Render
	void Render(HDC hdcMem, HDC hdcWin);

	//Scroll displaybox to the left
	void ScrollLeft();

	//Scroll displaybox to the Right
	void ScrollRight();

	//Set Display box graphic
	void Set(int i);

	//Get graphic value
	int GetValue();

	//Save State
	void SaveState(FILE* out);

	//Load State
	void LoadState(FILE* out);

private:
	
	//Graphics
	HBITMAP* m_graphic_handle;
	BITMAP* m_graphic;

	//Current Displayed Graphic
	int m_display;

	//Max display graphic
	int m_maxdisplay;

};


__forceinline int GfxBox::GetValue(){return m_display;}

__forceinline void GfxBox::Render(HDC hdcMem, HDC hdcWin)
{
	if(m_visible == 0){return;}

	if(m_graphic[m_display].bmWidth == 0)
		return;

	SelectObject(hdcMem, m_graphic_handle[m_display]);
	BitBlt(hdcWin, m_x, m_y, m_w, m_h, hdcMem, 0, 0, SRCCOPY);
}

__forceinline void GfxBox::ScrollLeft()
{
	if(m_display-1 < 0)
		m_display = m_maxdisplay;

	m_display--;
	RedrawWidget();
}

__forceinline void GfxBox::ScrollRight()
{
	if(m_display+1 > m_maxdisplay-1)
		m_display = -1;

	m_display++;
	RedrawWidget();
}

__forceinline void GfxBox::Set(int i)
{
	if(i < 0){return;}
	if(i > m_maxdisplay-1){return;}
	m_display = i;
	RedrawWidget();
}

#endif

