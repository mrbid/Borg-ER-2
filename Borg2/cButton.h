/*
	Code written by James Fletcher (~2010).
	https://github.com/mrbid
*/

#ifndef BUTTON_H
#define BUTTON_H

#include "cWidget.h"

//Button Class
class Button : private Widget
{
public:
	
	//Constructor
	Button(HWND hWnd, int x, int y, int graphic_unpressed, int graphic_hover, int winmsg);

	//Destructor
	~Button();

	//Called on mouse move
	void MouseMove(int x, int y);

	//Called on mouse press
	void MouseState(bool button, bool state);
	
	//Render
	void Render(HDC hdcMem, HDC hdcWin);

	//Save State
	void SaveState(FILE* out);

	//Load State
	void LoadState(FILE* out);

	//Set Unpressed Graphic
	void SetUnpressedGraphic(int graphic_unpressed);

	//Set Pressed Graphic
	void SetHoverGraphic(int graphic_hover);

	//Unset Unpressed Graphic
	void UnSetUnpressedGraphic();

	//Unset Pressed Graphic
	void UnSetHoverGraphic();

	//Get Unpressed Graphic ID
	int GetUnpressedGraphic();

	//Get Pressed Graphic ID
	int GetHoverGraphic();

	//Toggle between pressed and unpressed
	void Toggle();

private:
	
	//Graphics
	HBITMAP m_graphic_handle[2];
	BITMAP m_graphic[2];
	int m_gid1, m_gid2;

	//Hover State
	bool m_hoverstate;

	//Control re-drawing of the button
	bool m_chkonce;

};


__forceinline void Button::Toggle()
{
	if(GetUnpressedGraphic() == m_gid2)
	{
		UnSetUnpressedGraphic();
	}
	else
	{
		SetUnpressedGraphic(m_gid2);
	}
}

__forceinline int Button::GetUnpressedGraphic()
{
	return m_gid1;
}

__forceinline int Button::GetHoverGraphic()
{
	return m_gid2;
}

__forceinline void Button::MouseState(bool button, bool state)
{
	if(m_hoverstate == true && state == true && button == false)
	{
		PostMessage(m_hwnd, WM_COMMAND, m_winmsg, 0); 
	}
}

__forceinline void Button::Render(HDC hdcMem, HDC hdcWin)
{
	if(m_visible == 0){return;}

	if(m_hoverstate == false && m_graphic[0].bmWidth != 0)
	{
		SelectObject(hdcMem, m_graphic_handle[0]);
		BitBlt(hdcWin, m_x, m_y, m_w, m_h, hdcMem, 0, 0, SRCCOPY);
	}

	if(m_hoverstate == true && m_graphic[1].bmWidth != 0)
	{
		SelectObject(hdcMem, m_graphic_handle[1]);
		BitBlt(hdcWin, m_x, m_y, m_w, m_h, hdcMem, 0, 0, SRCCOPY);
	}
}

__forceinline void Button::UnSetUnpressedGraphic()
{
	ZeroMemory(&m_graphic[0], sizeof(m_graphic[0]));
	m_gid1 = 0;
	RedrawWidget();
}

__forceinline void Button::UnSetHoverGraphic()
{
	ZeroMemory(&m_graphic[1], sizeof(m_graphic[1]));
	m_gid2 = 0;
	RedrawWidget();
}

#endif

