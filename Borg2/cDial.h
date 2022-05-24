/*
	Code written by James Fletcher (~2010).
	https://github.com/mrbid
*/

#ifndef DIAL_H
#define DIAL_H

#include "Resource.h"
#include "cWidget.h"
#include <cmath>
#include <cstdio>

//Dial Class
class Dial : private Widget
{
public:
	
	//Constructor
	Dial(HWND hWnd, int x, int y, int w, int h, float startvalue, float endvalue, float sensitivity, float scalerot, bool lockzero, RECT statrect, int winmsg);

	//Destructor
	~Dial();

	//Set Value
	void SetValue(float value);

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

	//Get Knob Value
	float GetValue();

private:

	//Graphics
	HBITMAP m_graphic_handle[2];
	BITMAP m_graphic[2];

	//Hover State
	bool m_hoverstate;

	//Mouse Left Button State
	bool m_leftstate;

	//Half width & height
	int m_hw, m_hh;

	//DEG2RAD Conversion
	float DEG2RAD;

	//Rotation Amount
	float m_rot;

	//m_rot cannot exceed this value
	float m_endvalue;

	//Rotation Sensitivity
	float m_sensitivity;

	//Last mouse Y position before left mouse up
	int m_ly;

	//Sensitivity Multiplier
	int m_sensmultiply;

	//Make sure we only check for left up once after left down
	bool m_chkonce;

	//Refreshing the text gfx
	bool m_chkonce2;

	//Don't allow the knob to go into minus figures
	bool m_lockzero;

	//Rectangle for the status text
	RECT m_status;

	//Window HDC
	HDC hdcWindow;

	//Full rotation value
	float m_scalerot;
	float m_rscalerot;

	//Refresh Reguator
	unsigned int m_refresh, m_refresh2;

};


__forceinline float Dial::GetValue()
{
	return m_rot * m_scalerot;
}

__forceinline void Dial::SetValue(float value)
{
	m_rot = value * m_rscalerot;
}

#endif




