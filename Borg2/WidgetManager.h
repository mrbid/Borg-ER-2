/*
	Code written by James Fletcher (~2010).
	https://github.com/mrbid
*/

#ifndef WIDGETMANAGER_H
#define WIDGETMANAGER_H

#include "cButton.h"
#include "cHoverButton.h"
#include "cDial.h"
#include "cGfxBox.h"
#include "cSampleScope.h"
#include <vector>

//WidgetManager Singleton
class WidgetManager
{
public:

	//Singleton Instance
	static WidgetManager& Inst()
	{
		static WidgetManager singleton;
		return singleton;
	}

	//Destructor
	~WidgetManager();

	//Create Widget Functions
	int CreateButton(HWND hwnd, int x, int y, int graphic_unpressed, int graphic_hover);
	int CreateHoverButton(HWND hwnd, int x, int y, int w, int h);
	int CreateDial(HWND hwnd, int x, int y, int w, int h, float startvalue, float endvalue, float sensitivity, float scalerot, bool lockzero);
	int CreateGraphicBox(HWND hWnd, int x, int y, int startvalue, int* graphic_array, int graphic_array_size);
	int CreateSampleScope(HWND hWnd, int x, int y, int w, int h);

	//Delete all widgets
	void DeleteAllWidgets();

	//Get pointer to a widget from the winmsg id
	Widget* GetWidgetPointer(int winmsg);

	//Render Widgets
	void Render(HDC hdcMem, HDC hdcWin);

	//Update Widgets
	void Update();

	//Relay Mouse Move to widgets
	void MouseMove(int x, int y);

	//Relay Mouse State to widgets
	void MouseState(bool button, bool state);

	//Save Widget States
	void SaveState(char* file);

	//Load Widget States
	void LoadState(char* file);

	//Set status text rectangle
	void SetStatusRect(int top, int bottom, int left, int right);

private:

	//Status text location
	RECT m_status;

	//Widgets
	std::vector<Widget*> m_widgets;

	WidgetManager(){} //Constructor
	WidgetManager(const WidgetManager&); //Prevent Copy
	WidgetManager& operator=(const WidgetManager&); //Prevent Assignment

};

//Return Instance to Widget Manager
__forceinline WidgetManager& WidgetManager(){return WidgetManager::Inst();}


__forceinline Widget* WidgetManager::GetWidgetPointer(int winmsg)
{
	for(std::vector<Widget*>::iterator iter = m_widgets.begin(); iter != m_widgets.end(); ++iter)
		if((*iter)->GetWinmsg() == winmsg)
			return (*iter);

	#ifdef _DEBUG
	MessageBox(0, "Could not locate a Widget with specified windows message.\n Makesure in widget constructor you set m_winmsg and m_hwnd.", "Error", MB_OK | MB_TOPMOST);
	#endif
}

__forceinline void WidgetManager::Render(HDC hdcMem, HDC hdcWin)
{
	for(std::vector<Widget*>::iterator iter = m_widgets.begin(); iter != m_widgets.end(); ++iter)
		(*iter)->Render(hdcMem, hdcWin);
}

__forceinline void WidgetManager::Update()
{
	for(std::vector<Widget*>::iterator iter = m_widgets.begin(); iter != m_widgets.end(); ++iter)
		(*iter)->Update();
}

__forceinline void WidgetManager::MouseMove(int x, int y)
{
	for(std::vector<Widget*>::iterator iter = m_widgets.begin(); iter != m_widgets.end(); ++iter)
		(*iter)->MouseMove(x, y);
}

__forceinline void WidgetManager::MouseState(bool button, bool state)
{
	for(std::vector<Widget*>::iterator iter = m_widgets.begin(); iter != m_widgets.end(); ++iter)
		(*iter)->MouseState(button, state);
}

#endif




