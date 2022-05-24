/*
	Code written by James Fletcher (~2010).
	https://github.com/mrbid
*/

#include "cDial.h"

//Destructor
Dial::Dial(HWND hWnd, int x, int y, int w, int h, float startvalue, float endvalue, float sensitivity, float scalerot, bool lockzero, RECT statrect, int winmsg)
{
	//Set refresh regulator
	m_refresh = 0;
	m_refresh2 = 0;

	//Set status text rectangle
	m_status = statrect;

	//Set chkonce
	m_chkonce = false;
	m_chkonce2 = false;

	//Set windows message id
	m_winmsg = winmsg;

	//Set handle window this widget is part of
	m_hwnd = hWnd;

	//Set position and dimensions
	m_x = x, m_y = y, m_w = w, m_h = h, m_hw = w/2, m_hh = h/2;

	//Set DEG2RAD
	DEG2RAD = 3.14159f / 180.f;

	//Set end value
	m_endvalue = endvalue == 0.f? 99900.f : endvalue;

	//Set Full Rotation Value
	m_scalerot = scalerot;
	m_rscalerot = 1.0f / scalerot;

	//Set rotation amount
	m_rot = startvalue / scalerot;

	//Set Rotation Sensitivity
	m_sensitivity = sensitivity;

	//Set left button state
	m_leftstate = false;

	//Set last mouse Y position to center knob
	m_ly = m_y + m_hh;

	//Set sensitivity multiplier to normal
	m_sensmultiply = 1;

	//Set lockzero
	m_lockzero = lockzero;

	//Set hdcWindow
	hdcWindow = GetDC(m_hwnd);

	//Set Dial Graphics
	m_graphic_handle[0] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(BMP_S1));
	GetObject(m_graphic_handle[0], sizeof(m_graphic[0]), &m_graphic[0]);
	m_graphic_handle[1] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(BMP_S2));
	GetObject(m_graphic_handle[1], sizeof(m_graphic[1]), &m_graphic[1]);
}

//Constructor
Dial::~Dial()
{
	DeleteDC(hdcWindow);
	DeleteObject(m_graphic_handle[0]);
	DeleteObject(m_graphic_handle[1]);
}

//Called on mouse move
void Dial::MouseMove(int x, int y)
{
	/*
		If I use GetCursorPos()
		rather than the x, y values
		here I wont get the problem
		of turning the dial too fast
	*/

	//If mouse is over this button
	if(x > m_x-1 && x < m_x+m_w+1
		&& y > m_y-1 && y < m_y+m_h+1)
	{
		m_hoverstate = true;
		m_chkonce2 = true;

		//Draw m_rot value
		if(GetTickCount() > m_refresh)
		{
			InvalidateRect(m_hwnd, &m_status, FALSE);

			m_refresh = GetTickCount() + 33;
		}
	}
	else
	{
		m_hoverstate = false;

		if(m_chkonce2 == true)
		{
			InvalidateRect(m_hwnd, &m_status, FALSE);
			m_chkonce2 = false;
		}
	}

	//If left mouse button is pressed
	if(m_leftstate == true)
	{
		//Increment Rotation
		const int cpy = (m_y+m_hh);

		if(y > cpy)
			m_rot -= (y - cpy) * (m_sensitivity * m_sensmultiply);

		if(y < cpy)
			m_rot -= (y - cpy) * (m_sensitivity * m_sensmultiply);

		//Stop dial going into minus figures?
		if(m_lockzero == true && m_rot < 0.f)
			m_rot = 0.f;

		//Clamp at end value
		if(GetValue() >= m_endvalue)
			m_rot = m_endvalue / m_scalerot;

		//Stop float from overflowing
		//if(m_rot >= 99900.f)
			//m_rot = 99900.f;

		if(m_rot <= -9900.f)
			m_rot = -9900.f;

		//Lock cursor within knob rect
		//if(y > m_y+m_h)
		if(y > m_y+m_hh+1)
		{
			tagPOINT p;
			p.x = m_x+m_hw;
			p.y = m_y+m_hh;
			ClientToScreen(m_hwnd, &p);
			SetCursorPos(p.x, p.y);
			m_ly = m_y;
			return;
		}
			
		//if(y < m_y)
		if(y < m_y+m_hh-1)
		{
			tagPOINT p;
			p.x = m_x+m_hw;
			p.y = m_y+m_hh;
			ClientToScreen(m_hwnd, &p);
			SetCursorPos(p.x, p.y);
			m_ly = m_y+m_h;
		}

		//Make sure mouse cant move in x
		if(x < m_x)
		{
			tagPOINT p;
			p.x = m_x;
			p.y = y;
			ClientToScreen(m_hwnd, &p);
			SetCursorPos(p.x, p.y);
		}

		if(x > m_x + m_w)
		{
			tagPOINT p;
			p.x = m_x + m_w;
			p.y = y;
			ClientToScreen(m_hwnd, &p);
			SetCursorPos(p.x, p.y);
		}

		//Redraw Widget
		if(GetTickCount() > m_refresh2)
		{
			RedrawWidget();
			m_refresh2 = GetTickCount() + 33;
		}

		//Set last mouse Y position
		m_ly = y;
	}
}

//Called on mouse press
void Dial::MouseState(bool button, bool state)
{
	//Are we hovering over this button?
	if(m_hoverstate == true)
	{
		//Left click will turn the dial
		if(button == false && state == true)
		{
			m_leftstate = true;
			m_chkonce = true;
			ShowCursor(false);
		}

		//Right click changes sensitivity multiplier
		if(button == true && state == true)
		{
			if(m_sensmultiply == 1)
			{
				m_sensmultiply = 80;
				RedrawWidget();
				return;
			}

			if(m_sensmultiply == 80)
			{
				m_sensmultiply = 420;
				RedrawWidget();
				return;
			}

			if(m_sensmultiply == 420)
			{
				m_sensmultiply = 1;
				RedrawWidget();
				return;
			}
		}
	}

	//Unlock Cursor
	if(button == false && state == false && m_chkonce == true)
	{
		m_leftstate = false;
		m_chkonce = false;

		//Set cursor position to center of widget
		tagPOINT p;
		p.x = m_x + m_hw;
		p.y = m_y + m_hh;
		ClientToScreen(m_hwnd, &p);
		SetCursorPos(p.x, p.y);

		//Tell event loop theres a new rotation value
		PostMessage(m_hwnd, WM_COMMAND, m_winmsg, 0);

		//Make cursor visible again
		ShowCursor(true);
	}
}

//Render
void Dial::Render(HDC hdcMem, HDC hdcWin)
{
	if(m_visible == 0){return;}

	const int cpx = m_x + m_hw;
	const int cpy = m_y + m_hh;

	//Draw Value
	char status[32];

	if(m_sensitivity < 0.002f)
	{
		sprintf_s(status, "Value: %.4f", m_rot * m_scalerot);
	}
	else
	{
		sprintf_s(status, "Value: %.2f", m_rot * m_scalerot);
	}

	if(m_hoverstate == true)
	{
		SetTextColor(hdcWindow, RGB(255, 255, 255));
		SetBkColor(hdcWindow, RGB(0,0,0));
		DrawText(hdcWindow, status, strlen(status), &m_status, DT_VCENTER | DT_NOCLIP | DT_SINGLELINE);
	}

	//Draw Multiply Colour
	if(m_sensmultiply == 80)
	{
		SelectObject(hdcMem, m_graphic_handle[0]);
		BitBlt(hdcWin, m_x, m_y, m_w, m_h, hdcMem, 0, 0, SRCCOPY);
	}

	if(m_sensmultiply == 420)
	{
		SelectObject(hdcMem, m_graphic_handle[1]);
		BitBlt(hdcWin, m_x, m_y, m_w, m_h, hdcMem, 0, 0, SRCCOPY);
	}

	//Draw Line
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	SelectObject(hdcWin, pen);
	MoveToEx(hdcWin, cpx, cpy, NULL);

	LineTo(hdcWin, 
		cpx + (sin(-m_rot * DEG2RAD) * (m_hw-1)) + 0.5f,
		cpy + (cos(-m_rot * DEG2RAD) * (m_hh-1)) + 0.5f
		);

	DeleteObject(pen);
}

//Save State
void Dial::SaveState(FILE* out)
{
	fwrite(&m_rot, 1, sizeof(m_rot), out);
	fwrite(&m_sensmultiply, 1, sizeof(m_sensmultiply), out);
}

//Load State
void Dial::LoadState(FILE* out)
{
	fread(&m_rot, 1, sizeof(m_rot), out);
	fread(&m_sensmultiply, 1, sizeof(m_sensmultiply), out);
	RedrawWidget();
}






