/*
	Code written by James Fletcher (~2010).
	https://github.com/mrbid
*/

#include "cButton.h"

//Constructor
Button::Button(HWND hWnd, int x, int y, int graphic_unpressed, int graphic_hover, int winmsg)
{
	//Set Chkonce
	m_chkonce = false;

	//Set Windows Message ID
	m_winmsg = winmsg;

	//Set Handle Window
	m_hwnd = hWnd;

	//Keep graphic id's for Save State
	m_gid1 = graphic_unpressed;
	m_gid2 = graphic_hover;

	//Reset Bitmap
	ZeroMemory(&m_graphic[0], sizeof(m_graphic[0]));
	ZeroMemory(&m_graphic[1], sizeof(m_graphic[1]));

	//Load Unpressed Graphic
	m_graphic_handle[0] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(graphic_unpressed));
	GetObject(m_graphic_handle[0], sizeof(m_graphic[0]), &m_graphic[0]);

	//Load Pressed Graphic
	m_graphic_handle[1] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(graphic_hover));
	GetObject(m_graphic_handle[1], sizeof(m_graphic[1]), &m_graphic[1]);

	//Set Position and Dimensions
	m_x = x, m_y = y;

	if(m_graphic_handle[0] != NULL)
		m_w = m_graphic[0].bmWidth, m_h = m_graphic[0].bmHeight;

	if(m_graphic_handle[1] != NULL)
		m_w = m_graphic[1].bmWidth, m_h = m_graphic[1].bmHeight;

	//Set pressed state to unpressed
	m_hoverstate = false;
}

//Destructor
Button::~Button()
{
	DeleteObject(m_graphic_handle[0]);
	DeleteObject(m_graphic_handle[1]);
}

//Called on mouse move
void Button::MouseMove(int x, int y)
{
	//If mouse is over this button
	if(x > m_x && x < m_x+m_w
		&& y > m_y && y < m_y+m_h)
	{
		//Set state to hovering
		m_hoverstate = true;

		//Redraw part of the dialog
		if(m_chkonce == false)
		{
			RedrawWidget();
			m_chkonce = true;
		}
	}
	else
	{
		//Otherwise set state to normal
		m_hoverstate = false;

		//Redraw part of the dialog
		if(m_chkonce == true)
		{
			RedrawWidget();
			m_chkonce = false;
		}
	}
}

//Set unpressed graphic
void Button::SetUnpressedGraphic(int graphic_unpressed)
{
	ZeroMemory(&m_graphic[0], sizeof(m_graphic[0]));

	if(m_graphic_handle[0] != NULL)
		DeleteObject(m_graphic_handle[0]);

	m_gid1 = graphic_unpressed;
	m_graphic_handle[0] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(graphic_unpressed));
	GetObject(m_graphic_handle[0], sizeof(m_graphic[0]), &m_graphic[0]);
}

//Set hover graphic
void Button::SetHoverGraphic(int graphic_hover)
{
	ZeroMemory(&m_graphic[1], sizeof(m_graphic[1]));

	if(m_graphic_handle[1] != NULL)
		DeleteObject(m_graphic_handle[1]);

	m_gid2 = graphic_hover;
	m_graphic_handle[1] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(graphic_hover));
	GetObject(m_graphic_handle[1], sizeof(m_graphic[1]), &m_graphic[1]);
	RedrawWidget();
}

//Save State
void Button::SaveState(FILE* out)
{
	fwrite(&m_gid1, 1, sizeof(m_gid1), out);
}

//Load State
void Button::LoadState(FILE* out)
{
	fread(&m_gid1, 1, sizeof(m_gid1), out);
	SetUnpressedGraphic(m_gid1);
	RedrawWidget();
}






