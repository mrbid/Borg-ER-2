/*
	Code written by James Fletcher (~2010).
	https://github.com/mrbid
*/

#ifndef WIDGET_H
#define WIDGET_H

#include <windows.h>
#include <cstdio>

//Widget Class
class Widget
{
public:

	//Constructor
	Widget() : m_x(0), m_y(0), m_w(0), m_h(0), m_enabled(true), m_visible(true) {}
	virtual ~Widget(){}

	//Functions
	void RedrawWidget();

	//Input Functions
	virtual void MouseMove(int x, int y){}
	virtual void MouseState(bool button, bool state){}

	//Tick Functions
	virtual void Render(HDC hdcMem, HDC hdcWin){}
	virtual void Update(){}

	//Save / Load States
	virtual void LoadState(FILE* out){}
	virtual void SaveState(FILE* out){}

	//Widget Settings
	void Enable(bool state);
	void Visible(bool state);
	bool IsEnabled();
	bool IsVisible();

	//Accessors
	int GetWinmsg();

protected:

	//Widget Settings
	bool m_enabled;
	bool m_visible;

	//Widget Screen Space Location (top left corner)
	int m_x, m_y;

	//Widget Dimensions
	int m_w, m_h;

	//Hand Window that widget is in
	HWND m_hwnd;

	//Windows Message ID
	int m_winmsg;

};

__forceinline void Widget::Enable(bool state){m_enabled = state;}
__forceinline void Widget::Visible(bool state){m_visible = state;}
__forceinline bool Widget::IsEnabled(){return m_enabled;}
__forceinline bool Widget::IsVisible(){return m_visible;}
__forceinline int Widget::GetWinmsg(){return m_winmsg;}

__forceinline void Widget::RedrawWidget()
{
	RECT r;
	r.bottom = m_y + m_h;
	r.left = m_x;
	r.right = m_x + m_w;
	r.top = m_y;
	InvalidateRect(m_hwnd, &r, FALSE);
}

#endif





