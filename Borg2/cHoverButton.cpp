/*
	Code written by James Fletcher (~2010).
	https://github.com/mrbid
*/

#include "cHoverButton.h"


//Constructor
HoverButton::HoverButton(HWND hWnd, int x, int y, int w, int h, int winmsg)
{
	//Set chkonce start state
	m_chkonce = false;

	//Set Windows Message ID
	m_winmsg = winmsg;

	//Set Handle Window this widget is part of
	m_hwnd = hWnd;
	
	//Set position and dimensions
	m_x = x, m_y = y, m_w = w, m_h = h;

	//Set hover state
	m_hoverstate = false;

	//Set animation position
	m_yp = y;
}

//Called on mouse move
void HoverButton::MouseMove(int x, int y)
{
	//If mouse is over this button
	if(x > m_x && x < m_x+m_w
		&& y > m_y && y < m_y+m_h)
	{
		//Set state to hovering
		m_hoverstate = true;
		m_chkonce = false;
	}
	else
	{
		//Otherwise set state to normal
		m_hoverstate = false;

		//Redraw part of the dialog
		if(m_chkonce == false)
		{
			RedrawWidget();
			m_chkonce = true;
		}
	}
}





