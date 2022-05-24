/*
	Code written by James Fletcher (~2010).
	https://github.com/mrbid
*/

#include "WidgetManager.h"
#include "Borg2.h"

//Destructor
WidgetManager::~WidgetManager()
{
	for(std::vector<Widget*>::iterator iter = m_widgets.begin(); iter != m_widgets.end(); ++iter)
		delete (*iter);
}

//Delete all widgets
void WidgetManager::DeleteAllWidgets()
{
	for(std::vector<Widget*>::iterator iter = m_widgets.begin(); iter != m_widgets.end(); ++iter)
		delete (*iter);

	m_widgets.clear();
}

//Save Widget States
void WidgetManager::SaveState(char* file)
{
	FILE* out = fopen(file, "wb");
	if(out != NULL)
	{
		for(std::vector<Widget*>::iterator iter = m_widgets.begin(); iter != m_widgets.end(); ++iter)
				(*iter)->SaveState(out);

		//Hack
		SynthEngine.SaveState(out);

		fclose(out);
	}
}

//Load Widget States
void WidgetManager::LoadState(char* file)
{
	FILE* out = fopen(file, "rb");
	if(out != NULL)
	{
		for(std::vector<Widget*>::iterator iter = m_widgets.begin(); iter != m_widgets.end(); ++iter)
				(*iter)->LoadState(out);

		//Hack
		SynthEngine.LoadState(out);

		fclose(out);
	}
}

//Set status text rectangle
void WidgetManager::SetStatusRect(int top, int bottom, int left, int right)
{
	m_status.top = top;
	m_status.bottom = bottom;
	m_status.left = left;
	m_status.right = right;
}

//Create a button widget
int WidgetManager::CreateButton(HWND hwnd, int x, int y, int graphic_unpressed, int graphic_hover)
{
	const int winmsg = m_widgets.size() + 31000;
	Button* newwidget = new Button(hwnd, x, y, graphic_unpressed, graphic_hover, winmsg);
	m_widgets.push_back((Widget*)newwidget);
	return winmsg;
}

//Create a hoverbutton widget
int WidgetManager::CreateHoverButton(HWND hwnd, int x, int y, int w, int h)
{
	const int winmsg = m_widgets.size() + 31000;
	HoverButton* newwidget = new HoverButton(hwnd, x, y, w, h, winmsg);
	m_widgets.push_back((Widget*)newwidget);
	return winmsg;
}

//Create a dial widget
int WidgetManager::CreateDial(HWND hwnd, int x, int y, int w, int h, float startvalue, float endvalue, float sensitivity, float scalerot, bool lockzero)
{
	const int winmsg = m_widgets.size() + 31000;
	Dial* newwidget = new Dial(hwnd, x, y, w, h, startvalue, endvalue, sensitivity, scalerot, lockzero, m_status, winmsg);
	m_widgets.push_back((Widget*)newwidget);
	return winmsg;
}

//Create a graphics box widget
int WidgetManager::CreateGraphicBox(HWND hwnd, int x, int y, int startvalue, int* graphic_array, int graphic_array_size)
{
	const int winmsg = m_widgets.size() + 31000;
	GfxBox* newwidget = new GfxBox(hwnd, x, y, startvalue, graphic_array, graphic_array_size, winmsg);
	m_widgets.push_back((Widget*)newwidget);
	return winmsg;
}

//Create a sample scope
int WidgetManager::CreateSampleScope(HWND hWnd, int x, int y, int w, int h)
{
	const int winmsg = m_widgets.size() + 31000;
	SampleScope* newwidget = new SampleScope(hWnd, x, y, w, h, winmsg);
	m_widgets.push_back((Widget*)newwidget);
	return winmsg;
}





