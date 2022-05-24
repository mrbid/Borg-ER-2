/*
	Code written by James Fletcher (~2010).
	https://github.com/mrbid
*/

#include "cGfxBox.h"

//Constructor
GfxBox::GfxBox(HWND hWnd, int x, int y, int startvalue, int* graphic_array, int graphic_array_size, int winmsg)
{
	//Set Windows Message ID
	m_winmsg = winmsg;

	//Set handle window this widget is part of
	m_hwnd = hWnd;

	//Set display graphic to zero
	m_display = startvalue;

	//set max display graphic
	m_maxdisplay = graphic_array_size;

	//Allocate Memory for graphics
	m_graphic_handle = new HBITMAP[graphic_array_size];
	m_graphic = new BITMAP[graphic_array_size];

	//Load Graphics
	for(int i = 0; i < graphic_array_size; i++)
	{
		m_graphic_handle[i] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(graphic_array[i]));
		GetObject(m_graphic_handle[i], sizeof(m_graphic[i]), &m_graphic[i]);
	}

	//If first element of graphics array is zero, clear the info
	if(graphic_array[0] == 0)
	{
		ZeroMemory(&m_graphic[0], sizeof(m_graphic[0]));
		m_graphic[0].bmWidth = 0;
	}

	//Set position and dimensions
	m_x = x, m_y = y, m_w = m_graphic[1].bmWidth, m_h = m_graphic[1].bmHeight;
}

//Destructor
GfxBox::~GfxBox()
{
	for(int i = 0; i < m_maxdisplay; i++)
		DeleteObject(m_graphic_handle[i]);

	delete [] m_graphic_handle;
	delete [] m_graphic;
}

//Save State
void GfxBox::SaveState(FILE* out)
{
	fwrite(&m_display, 1, sizeof(m_display), out);
}

//Load State
void GfxBox::LoadState(FILE* out)
{
	fread(&m_display, 1, sizeof(m_display), out);
	RedrawWidget();
}














