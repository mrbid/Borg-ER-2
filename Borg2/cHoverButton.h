/*
	Code written by James Fletcher (~2010).
	https://github.com/mrbid
*/

#ifndef HOVERBUTTON_H
#define HOVERBUTTON_H

#include "cWidget.h"

//HoverButton Class
class HoverButton : private Widget
{
public:
	
	//Constructor
	HoverButton(HWND hWnd, int x, int y, int w, int h, int winmsg);

	//Called on mouse move
	void MouseMove(int x, int y);

	//Called on mouse press
	void MouseState(bool button, bool state);
	
	//Render
	void Render(HDC hdcMem, HDC hdcWin);

	//Update
	void Update();

private:

	//Animation Variables
	int m_yp;

	//Hover State
	bool m_hoverstate;

	//Control re-drawing of the button
	bool m_chkonce;

	//Keep instance od window HDC for Update() func
	HDC m_win;

};


__forceinline void HoverButton::MouseState(bool button, bool state)
{
	if(m_hoverstate == true && state == true && button == false)
	{
		PostMessage(m_hwnd, WM_COMMAND, m_winmsg, 0); 
	}
}

__forceinline void HoverButton::Render(HDC hdcMem, HDC hdcWin)
{
	if(m_visible == 0){return;}

	if(m_hoverstate == true)
	{
		m_win = hdcWin;

		HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		SelectObject(hdcWin, pen);
		MoveToEx(m_win, m_x, m_yp, NULL);
		LineTo(m_win, m_x+m_w, m_yp);
		DeleteObject(pen);
	}
}

__forceinline void HoverButton::Update()
{
	if(m_hoverstate == true)
	{
		RedrawWidget();

		m_yp++;
		if(m_yp >= m_y + m_h){m_yp = m_y;}
	}
}

#endif

