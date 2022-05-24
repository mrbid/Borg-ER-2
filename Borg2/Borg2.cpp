/*
	Code written by James Fletcher (~2010).
	https://github.com/mrbid
*/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Commdlg.h>
#include <shellapi.h>
#include <stdio.h>
#include <Mmsystem.h>
#include "resource.h"

#include "WidgetManager.h"
#include "SynthEngine.h"
#include "WinAudio.h"

//Instance of the Synth Engine
SynthEngine SynthEngine;


//Variables
PAINTSTRUCT ps;
HDC hdcWindow;
const UINT TimerGen = 1;
bool CreditEnabled = false;
int m_refreshreg = 0;
int mousex = 0, mousey = 0;
int m_hovering = 0;

//Background Graphic
HBITMAP hBack, hCredit;
BITMAP bmBack, bmCredit;
HBITMAP hOSC[8];
BITMAP bmOSC[8];

//Direct Pointers
SampleScope *pSampleScope;
Dial *pA, *pS, *pD, *pR;

//Position for oscillator highlight
int osch_x = 9, osch_y = 120;
int hover_x = 9, hover_y = 120;

//WinMessages
int BUTTON_OSC1_L = 0;
int BUTTON_OSC1_R = 0;
int BUTTON_OSC2_L = 0;
int BUTTON_OSC2_R = 0;
int BUTTON_OSC3_L = 0;
int BUTTON_OSC3_R = 0;
int BUTTON_OSC4_L = 0;
int BUTTON_OSC4_R = 0;
int BUTTON_OSC5_L = 0;
int BUTTON_OSC5_R = 0;
int BUTTON_OSC6_L = 0;
int BUTTON_OSC6_R = 0;
int BUTTON_OSC7_L = 0;
int BUTTON_OSC7_R = 0;
int BUTTON_OSC8_L = 0;
int BUTTON_OSC8_R = 0;

int BUTTON_AM1 = 0;
int BUTTON_FM1 = 0;
int BUTTON_AM2 = 0;
int BUTTON_FM2 = 0;
int BUTTON_AM3 = 0;
int BUTTON_FM3 = 0;
int BUTTON_AM4 = 0;
int BUTTON_FM4 = 0;
int BUTTON_AM5 = 0;
int BUTTON_FM5 = 0;
int BUTTON_AM6 = 0;
int BUTTON_FM6 = 0;
int BUTTON_AM7 = 0;
int BUTTON_FM7 = 0;
int BUTTON_AM8 = 0;
int BUTTON_FM8 = 0;
int BUTTON_AM9 = 0;
int BUTTON_FM9 = 0;
int BUTTON_AM10 = 0;
int BUTTON_FM10 = 0;

int BUTTON_ADD1 = 0;
int BUTTON_ADD2 = 0;
int BUTTON_ADD3 = 0;
int BUTTON_ADD4 = 0;
int BUTTON_ADD5 = 0;
int BUTTON_ADD6 = 0;
int BUTTON_ADD7 = 0;
int BUTTON_ADD8 = 0;
int BUTTON_ADD9 = 0;
int BUTTON_ADD10 = 0;

int DIAL_OSC1_FREQ = 0;
int DIAL_OSC1_AMP = 0;
int DIAL_OSC2_FREQ = 0;
int DIAL_OSC2_AMP = 0;
int DIAL_OSC3_FREQ = 0;
int DIAL_OSC3_AMP = 0;
int DIAL_OSC4_FREQ = 0;
int DIAL_OSC4_AMP = 0;
int DIAL_OSC5_FREQ = 0;
int DIAL_OSC5_AMP = 0;
int DIAL_OSC6_FREQ = 0;
int DIAL_OSC6_AMP = 0;
int DIAL_OSC7_FREQ = 0;
int DIAL_OSC7_AMP = 0;
int DIAL_OSC8_FREQ = 0;
int DIAL_OSC8_AMP = 0;

int GFXBOX_OSC1 = 0;
int GFXBOX_OSC2 = 0;
int GFXBOX_OSC3 = 0;
int GFXBOX_OSC4 = 0;
int GFXBOX_OSC5 = 0;
int GFXBOX_OSC6 = 0;
int GFXBOX_OSC7 = 0;
int GFXBOX_OSC8 = 0;

int BUTTON_SHARP = 0;
int BUTTON_SMOOTH = 0;
int HOVER_ABOUT = 0;
int HOVER_PLAY = 0;
int HOVER_EXPORT = 0;
int HOVER_LOAD = 0;
int HOVER_SAVE = 0;
int HOVER_NEW = 0;

int DIAL_BITCRUSH = 0;
int BUTTON_BCSHARP = 0;
int BUTTON_BCSMOOTH = 0;

int DIAL_BITREFLECT = 0;
int BUTTON_BRSHARP = 0;
int BUTTON_BRSMOOTH = 0;

int DIAL_A = 0;
int DIAL_D = 0;
int DIAL_S = 0;
int DIAL_SL = 0;
int DIAL_R = 0;

int SAMPLESCOPE = 0;

int DIAL_LOWBOOSTFREQ = 0;
int DIAL_LOWBOOST = 0;

int DIAL_HIGHBOOSTFREQ = 0;
int DIAL_HIGHBOOST = 0;

int DIAL_HIGHPASS_CUTOFF = 0;
int DIAL_HIGHPASS_RESONANCE = 0;

int DIAL_LOWPASS_CUTOFF = 0;
int DIAL_LOWPASS_RESONANCE = 0;

int DIAL_FREQ_OFFSET = 0;
int DIAL_AMP_OFFSET = 0;




//Draw Oscillator Highlight
void DrawOscSelect(HDC hdcWindow, int ix, int iy)
{
	int x = ix, y = iy;

	HPEN pen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	SelectObject(hdcWindow, pen);

	MoveToEx(hdcWindow, x, y, NULL);
	x += 20;
	LineTo(hdcWindow, x, y);

	MoveToEx(hdcWindow, x, y, NULL);
	y += 2;
	LineTo(hdcWindow, x, y);

	MoveToEx(hdcWindow, x, y, NULL);
	x -= 18;
	LineTo(hdcWindow, x, y);

	MoveToEx(hdcWindow, x, y, NULL);
	y += 79;
	LineTo(hdcWindow, x, y);

	MoveToEx(hdcWindow, x, y, NULL);
	x += 18;
	LineTo(hdcWindow, x, y);

	MoveToEx(hdcWindow, x, y, NULL);
	y += 2;
	LineTo(hdcWindow, x, y);

	MoveToEx(hdcWindow, x, y, NULL);
	x -= 20;
	LineTo(hdcWindow, x, y);

	MoveToEx(hdcWindow, x, y, NULL);
	y -= 83;
	LineTo(hdcWindow, x, y);

	x=ix+67, y=iy;

	MoveToEx(hdcWindow, x, y, NULL);
	x += 20;
	LineTo(hdcWindow, x, y);

	MoveToEx(hdcWindow, x, y, NULL);
	y += 83;
	LineTo(hdcWindow, x, y);

	MoveToEx(hdcWindow, x, y, NULL);
	x -= 20;
	LineTo(hdcWindow, x, y);

	MoveToEx(hdcWindow, x, y, NULL);
	y -= 2;
	LineTo(hdcWindow, x, y);

	MoveToEx(hdcWindow, x, y, NULL);
	x += 18;
	LineTo(hdcWindow, x, y);

	MoveToEx(hdcWindow, x, y, NULL);
	y -= 79;
	LineTo(hdcWindow, x, y);

	MoveToEx(hdcWindow, x, y, NULL);
	x -= 18;
	LineTo(hdcWindow, x, y);

	MoveToEx(hdcWindow, x, y, NULL);
	y -= 2;
	LineTo(hdcWindow, x, y);

	DeleteObject(pen);
}

//Is mouse within oscillator?
bool IsMouseInOscillator(HWND hwnd, int mx, int my, int x, int y)
{
	y -= 4;

	if(mx > x && mx < x+86 && my > y && my < y+14)
	{
		InvalidateRect(hwnd, 0, FALSE);
		return true;
	}

	return false;
}

//Is mouse within oscillator?
bool IsMouseInOscillator2(HWND hwnd, int mx, int my, int x, int y)
{
	y -= 4;

	if(mx > x && mx < x+86 && my > y && my < y+14)
	{
		if(GetTickCount() > m_refreshreg)
		{
			RECT r;
			r.top = hover_y-2;
			r.bottom = (hover_y-2)+8;
			r.left = hover_x+31;
			r.right = hover_x+31+26;
			InvalidateRect(hwnd, &r, FALSE);
			m_refreshreg = GetTickCount() + 33;
		}

		return true;
	}

	return false;
}

//Urgh... Hack, this gui was badly coded but this is two years on and i'm not bothered on a re-write.
void UpdateGFXSelectedOSC(HWND hwnd)
{
	if(SynthEngine.m_selected == 0)
	{
		osch_x = 9, osch_y = 120;
		SynthEngine.ShowSelectedOscillator();
	}
	else if(SynthEngine.m_selected == 1)
	{
		osch_x = 132, osch_y = 120;
		SynthEngine.ShowSelectedOscillator();
	}
	else if(SynthEngine.m_selected == 2)
	{
		osch_x = 255, osch_y = 120;
		SynthEngine.ShowSelectedOscillator();
	}
	else if(SynthEngine.m_selected == 3)
	{
		osch_x = 378, osch_y = 120;
		SynthEngine.ShowSelectedOscillator();
	}
	else if(SynthEngine.m_selected == 4)
	{
		osch_x = 9, osch_y = 9;
		SynthEngine.ShowSelectedOscillator();
	}
	else if(SynthEngine.m_selected == 5)
	{
		osch_x = 132, osch_y = 9;
		SynthEngine.ShowSelectedOscillator();
	}
	else if(SynthEngine.m_selected == 6)
	{
		osch_x = 255, osch_y = 9;
		SynthEngine.ShowSelectedOscillator();
	}
	else if(SynthEngine.m_selected == 7)
	{
		osch_x = 378, osch_y = 9;
		SynthEngine.ShowSelectedOscillator();
	}

	InvalidateRect(hwnd, 0, FALSE);
}

//Set selected oscillator
void HighlightSelectedOscillator(HWND hwnd, int mx, int my)
{
	if(IsMouseInOscillator(hwnd, mx, my, 9, 120) == true)
	{
		SynthEngine.m_selected = 0;
		osch_x = 9, osch_y = 120;

		SynthEngine.ShowSelectedOscillator();
		pSampleScope->UpdateSample(SynthEngine.GetSelectedSample(), SynthEngine.GetSelectedSampleSize());
	}

	if(IsMouseInOscillator(hwnd, mx, my, 132, 120) == true)
	{
		SynthEngine.m_selected = 1;
		osch_x = 132, osch_y = 120;

		SynthEngine.ShowSelectedOscillator();
		pSampleScope->UpdateSample(SynthEngine.GetSelectedSample(), SynthEngine.GetSelectedSampleSize());
	}

	if(IsMouseInOscillator(hwnd, mx, my, 255, 120) == true)
	{
		SynthEngine.m_selected = 2;
		osch_x = 255, osch_y = 120;

		SynthEngine.ShowSelectedOscillator();
		pSampleScope->UpdateSample(SynthEngine.GetSelectedSample(), SynthEngine.GetSelectedSampleSize());
	}

	if(IsMouseInOscillator(hwnd, mx, my, 378, 120) == true)
	{
		SynthEngine.m_selected = 3;
		osch_x = 378, osch_y = 120;

		SynthEngine.ShowSelectedOscillator();
		pSampleScope->UpdateSample(SynthEngine.GetSelectedSample(), SynthEngine.GetSelectedSampleSize());
	}

	if(IsMouseInOscillator(hwnd, mx, my, 9, 9) == true)
	{
		SynthEngine.m_selected = 4;
		osch_x = 9, osch_y = 9;

		SynthEngine.ShowSelectedOscillator();
		pSampleScope->UpdateSample(SynthEngine.GetSelectedSample(), SynthEngine.GetSelectedSampleSize());
	}

	if(IsMouseInOscillator(hwnd, mx, my, 132, 9) == true)
	{
		SynthEngine.m_selected = 5;
		osch_x = 132, osch_y = 9;

		SynthEngine.ShowSelectedOscillator();
		pSampleScope->UpdateSample(SynthEngine.GetSelectedSample(), SynthEngine.GetSelectedSampleSize());
	}

	if(IsMouseInOscillator(hwnd, mx, my, 255, 9) == true)
	{
		SynthEngine.m_selected = 6;
		osch_x = 255, osch_y = 9;

		SynthEngine.ShowSelectedOscillator();
		pSampleScope->UpdateSample(SynthEngine.GetSelectedSample(), SynthEngine.GetSelectedSampleSize());
	}

	if(IsMouseInOscillator(hwnd, mx, my, 378, 9) == true)
	{
		SynthEngine.m_selected = 7;
		osch_x = 378, osch_y = 9;

		SynthEngine.ShowSelectedOscillator();
		pSampleScope->UpdateSample(SynthEngine.GetSelectedSample(), SynthEngine.GetSelectedSampleSize());
	}
}

//Hover selected oscillator
void HoverSelectedOscillator(HWND hwnd, int mx, int my)
{
	if(IsMouseInOscillator2(hwnd, mx, my, 9, 120) == true)
	{
		hover_x = 9, hover_y = 120;
		m_hovering = 0;
		return;
	}

	if(IsMouseInOscillator2(hwnd, mx, my, 132, 120) == true)
	{
		hover_x = 132, hover_y = 120;
		m_hovering = 1;
		return;
	}

	if(IsMouseInOscillator2(hwnd, mx, my, 255, 120) == true)
	{
		hover_x = 255, hover_y = 120;
		m_hovering = 2;
		return;
	}

	if(IsMouseInOscillator2(hwnd, mx, my, 378, 120) == true)
	{
		hover_x = 378, hover_y = 120;
		m_hovering = 3;
		return;
	}

	if(IsMouseInOscillator2(hwnd, mx, my, 9, 9) == true)
	{
		hover_x = 9, hover_y = 9;
		m_hovering = 4;
		return;
	}

	if(IsMouseInOscillator2(hwnd, mx, my, 132, 9) == true)
	{
		hover_x = 132, hover_y = 9;
		m_hovering = 5;
		return;
	}

	if(IsMouseInOscillator2(hwnd, mx, my, 255, 9) == true)
	{
		hover_x = 255, hover_y = 9;
		m_hovering = 6;
		return;
	}

	if(IsMouseInOscillator2(hwnd, mx, my, 378, 9) == true)
	{
		hover_x = 378, hover_y = 9;
		m_hovering = 7;
		return;
	}

	
	//Unhover
	m_hovering = -1;

	if(GetTickCount() > m_refreshreg)
	{
		RECT r;
		r.top = hover_y-2;
		r.bottom = (hover_y-2)+8;
		r.left = hover_x+31;
		r.right = hover_x+31+26;
		InvalidateRect(hwnd, &r, FALSE);
		m_refreshreg = GetTickCount() + 33;
	}
}

//Function to create a scroll effect button
void DoAddSubButton(int id)
{
	Button* pBTN = ((Button*)WidgetManager().GetWidgetPointer(id));
				
	if(pBTN->GetHoverGraphic() == BMP_NO)
	{
		pBTN->SetHoverGraphic(BMP_ADD);
		pBTN->SetUnpressedGraphic(BMP_ADD);
		return;
	}

	if(pBTN->GetHoverGraphic() == BMP_ADD)
	{
		pBTN->SetHoverGraphic(BMP_SUB);
		pBTN->SetUnpressedGraphic(BMP_SUB);
		return;
	}

	if(pBTN->GetHoverGraphic() == BMP_SUB)
	{
		pBTN->SetHoverGraphic(BMP_TIMES);
		pBTN->SetUnpressedGraphic(BMP_TIMES);
		return;
	}

	if(pBTN->GetHoverGraphic() == BMP_TIMES)
	{
		pBTN->SetHoverGraphic(BMP_NO);
		pBTN->SetUnpressedGraphic(0);
		return;
	}
}


//Get's a file name from file path
void GetFileName(const char* path, char* output)
{
	if(strcmp(path, ""))
	{
		char* file_path = new char[strlen(path)+1];
		long int null_len=0;
		sprintf(file_path, path);

		for(long int i = strlen(file_path); file_path[i] != '\\'; i--)
		{
			file_path[i] = 0x00;
		}

		null_len = strlen(file_path);
		ZeroMemory(file_path, sizeof(file_path));
		sprintf(file_path, path);

		for(int i = null_len; i <= strlen(path); i++)
		{
			output[i-null_len] = file_path[i];
		}

		output[strlen(output)] = 0x00;
		delete file_path;
	}
}

//Create Dialog Widgets
void CreateWidgets(HWND hWnd)
{
	//Set Dialog Window Text
	SetWindowText(hWnd, "Borg ER-2");

	//Create Buttons
	SAMPLESCOPE = WidgetManager().CreateSampleScope(hWnd, 0, 324, 477, 80);

	BUTTON_OSC4_L = WidgetManager().CreateButton(hWnd, 17, 144, 0, BMP_L);
	BUTTON_OSC4_R = WidgetManager().CreateButton(hWnd, 77, 144, 0, BMP_R);
	BUTTON_OSC3_L = WidgetManager().CreateButton(hWnd, 140, 144, 0, BMP_L);
	BUTTON_OSC3_R = WidgetManager().CreateButton(hWnd, 200, 144, 0, BMP_R);
	BUTTON_OSC2_L = WidgetManager().CreateButton(hWnd, 263, 144, 0, BMP_L);
	BUTTON_OSC2_R = WidgetManager().CreateButton(hWnd, 323, 144, 0, BMP_R);
	BUTTON_OSC1_L = WidgetManager().CreateButton(hWnd, 386, 144, 0, BMP_L);
	BUTTON_OSC1_R = WidgetManager().CreateButton(hWnd, 446, 144, 0, BMP_R);

	BUTTON_OSC8_L = WidgetManager().CreateButton(hWnd, 17, 33, 0, BMP_L);
	BUTTON_OSC8_R = WidgetManager().CreateButton(hWnd, 77, 33, 0, BMP_R);
	BUTTON_OSC7_L = WidgetManager().CreateButton(hWnd, 140, 33, 0, BMP_L);
	BUTTON_OSC7_R = WidgetManager().CreateButton(hWnd, 200, 33, 0, BMP_R);
	BUTTON_OSC6_L = WidgetManager().CreateButton(hWnd, 263, 33, 0, BMP_L);
	BUTTON_OSC6_R = WidgetManager().CreateButton(hWnd, 323, 33, 0, BMP_R);
	BUTTON_OSC5_L = WidgetManager().CreateButton(hWnd, 386, 33, 0, BMP_L);
	BUTTON_OSC5_R = WidgetManager().CreateButton(hWnd, 446, 33, 0, BMP_R);

	BUTTON_AM1 = WidgetManager().CreateButton(hWnd, 350, 141, 0, BMP_AM);
	BUTTON_FM1 = WidgetManager().CreateButton(hWnd, 350, 172, 0, BMP_FM);
	BUTTON_AM2 = WidgetManager().CreateButton(hWnd, 227, 141, 0, BMP_AM);
	BUTTON_FM2 = WidgetManager().CreateButton(hWnd, 227, 172, 0, BMP_FM);
	BUTTON_AM3 = WidgetManager().CreateButton(hWnd, 104, 141, 0, BMP_AM);
	BUTTON_FM3 = WidgetManager().CreateButton(hWnd, 104, 172, 0, BMP_FM);
	BUTTON_AM4 = WidgetManager().CreateButton(hWnd, 390, 100, 0, BMP_AM);
	BUTTON_FM4 = WidgetManager().CreateButton(hWnd, 433, 100, 0, BMP_FM);
	BUTTON_AM5 = WidgetManager().CreateButton(hWnd, 267, 100, 0, BMP_AM);
	BUTTON_FM5 = WidgetManager().CreateButton(hWnd, 310, 100, 0, BMP_FM);
	BUTTON_AM6 = WidgetManager().CreateButton(hWnd, 144, 100, 0, BMP_AM);
	BUTTON_FM6 = WidgetManager().CreateButton(hWnd, 187, 100, 0, BMP_FM);
	BUTTON_AM7 = WidgetManager().CreateButton(hWnd, 21, 100, 0, BMP_AM);
	BUTTON_FM7 = WidgetManager().CreateButton(hWnd, 64, 100, 0, BMP_FM);
	BUTTON_AM8 = WidgetManager().CreateButton(hWnd, 104, 30, 0, BMP_AM);
	BUTTON_FM8 = WidgetManager().CreateButton(hWnd, 104, 61, 0, BMP_FM);
	BUTTON_AM9 = WidgetManager().CreateButton(hWnd, 227, 30, 0, BMP_AM);
	BUTTON_FM9 = WidgetManager().CreateButton(hWnd, 227, 61, 0, BMP_FM);
	BUTTON_AM10 = WidgetManager().CreateButton(hWnd, 350, 30, 0, BMP_AM);
	BUTTON_FM10 = WidgetManager().CreateButton(hWnd, 350, 61, 0, BMP_FM);

	BUTTON_ADD1 = WidgetManager().CreateButton(hWnd, 354, 156, 0, BMP_NO);
	BUTTON_ADD2 = WidgetManager().CreateButton(hWnd, 231, 156, 0, BMP_NO);
	BUTTON_ADD3 = WidgetManager().CreateButton(hWnd, 108, 156, 0, BMP_NO);
	BUTTON_ADD4 = WidgetManager().CreateButton(hWnd, 415, 88, 0, BMP_NO);
	BUTTON_ADD5 = WidgetManager().CreateButton(hWnd, 292, 88, 0, BMP_NO);
	BUTTON_ADD6 = WidgetManager().CreateButton(hWnd, 169, 88, 0, BMP_NO);
	BUTTON_ADD7 = WidgetManager().CreateButton(hWnd, 46, 88, 0, BMP_NO);
	BUTTON_ADD8 = WidgetManager().CreateButton(hWnd, 108, 45, 0, BMP_NO);
	BUTTON_ADD9 = WidgetManager().CreateButton(hWnd, 231, 45, 0, BMP_NO);
	BUTTON_ADD10 = WidgetManager().CreateButton(hWnd, 354, 45, 0, BMP_NO);

	HOVER_ABOUT = WidgetManager().CreateHoverButton(hWnd, 7, 404, 36, 16);
	HOVER_PLAY = WidgetManager().CreateHoverButton(hWnd, 50, 404, 24, 16);
	HOVER_EXPORT = WidgetManager().CreateHoverButton(hWnd, 81, 404, 37, 16);

	HOVER_LOAD = WidgetManager().CreateHoverButton(hWnd, 408, 404, 26, 16);
	HOVER_SAVE = WidgetManager().CreateHoverButton(hWnd, 442, 404, 26, 16);
	HOVER_NEW = WidgetManager().CreateHoverButton(hWnd, 373, 404, 26, 16);

	DIAL_OSC1_FREQ = WidgetManager().CreateDial(hWnd, 393, 165, 19, 19, 180.f, 0.f, 0.002f, 1.f, false);
	DIAL_OSC1_AMP = WidgetManager().CreateDial(hWnd, 432, 165, 19, 19, 90.f, 0.f, 0.002f, 1.f, false);
	DIAL_OSC2_FREQ = WidgetManager().CreateDial(hWnd, 270, 165, 19, 19, 180.f, 0.f, 0.002f, 1.f, false);
	DIAL_OSC2_AMP = WidgetManager().CreateDial(hWnd, 309, 165, 19, 19, 90.f, 0.f, 0.002f, 1.f, false);
	DIAL_OSC3_FREQ = WidgetManager().CreateDial(hWnd, 147, 165, 19, 19, 180.f, 0.f, 0.002f, 1.f, false);
	DIAL_OSC3_AMP = WidgetManager().CreateDial(hWnd, 186, 165, 19, 19, 90.f, 0.f, 0.002f, 1.f, false);
	DIAL_OSC4_FREQ = WidgetManager().CreateDial(hWnd, 24, 165, 19, 19, 180.f, 0.f, 0.002f, 1.f, false);
	DIAL_OSC4_AMP = WidgetManager().CreateDial(hWnd, 63, 165, 19, 19, 70.f, 0.f, 0.002f, 1.f, false);

	DIAL_OSC5_FREQ = WidgetManager().CreateDial(hWnd, 393, 54, 19, 19, 180.f, 0.f, 0.002f, 1.f, false);
	DIAL_OSC5_AMP = WidgetManager().CreateDial(hWnd, 432, 54, 19, 19, 90.f, 0.f, 0.002f, 1.f, false);
	DIAL_OSC6_FREQ = WidgetManager().CreateDial(hWnd, 270, 54, 19, 19, 180.f, 0.f, 0.002f, 1.f, false);
	DIAL_OSC6_AMP = WidgetManager().CreateDial(hWnd, 309, 54, 19, 19, 90.f, 0.f, 0.002f, 1.f, false);
	DIAL_OSC7_FREQ = WidgetManager().CreateDial(hWnd, 147, 54, 19, 19, 180.f, 0.f, 0.002f, 1.f, false);
	DIAL_OSC7_AMP = WidgetManager().CreateDial(hWnd, 186, 54, 19, 19, 90.f, 0.f, 0.002f, 1.f, false);
	DIAL_OSC8_FREQ = WidgetManager().CreateDial(hWnd, 24, 54, 19, 19, 180.f, 0.f, 0.002f, 1.f, false);
	DIAL_OSC8_AMP = WidgetManager().CreateDial(hWnd, 63, 54, 19, 19, 90.f, 0.f, 0.002f, 1.f, false);

	DIAL_A = WidgetManager().CreateDial(hWnd, 17, 221, 19, 19, 0.f, 0.f, 0.0002f, 0.1f, true);
	DIAL_D = WidgetManager().CreateDial(hWnd, 39, 221, 19, 19, 0.f, 0.f, 0.0002f, 0.1f, true);
	DIAL_S = WidgetManager().CreateDial(hWnd, 61, 221, 19, 19, 0.03f, 0.f, 0.0002f, 0.1f, true);
	DIAL_SL = WidgetManager().CreateDial(hWnd, 83, 221, 19, 19, 128.f, 255.f, 0.002f, 0.705f, true);
	DIAL_R = WidgetManager().CreateDial(hWnd, 105, 221, 19, 19, 0.f, 0.f, 0.0002f, 0.1f, true);
	BUTTON_SMOOTH = WidgetManager().CreateButton(hWnd, 128, 221, 0, BMP_SMOOTH);
	BUTTON_SHARP = WidgetManager().CreateButton(hWnd, 128, 238, 0, BMP_SHARP);

	DIAL_LOWPASS_CUTOFF = WidgetManager().CreateDial(hWnd, 178, 221, 19, 19, 0.f, 0.f, 0.002f, 1.f, true);
	DIAL_LOWPASS_RESONANCE = WidgetManager().CreateDial(hWnd, 235, 221, 19, 19, 0.40f, 0.f, 0.002f, 1.f, true);

	DIAL_HIGHPASS_CUTOFF = WidgetManager().CreateDial(hWnd, 303, 221, 19, 19, 0.f, 0.f, 0.002f, 1.f, true);
	DIAL_HIGHPASS_RESONANCE = WidgetManager().CreateDial(hWnd, 360, 221, 19, 19, 0.40f, 0.f, 0.002f, 1.f, true);

	DIAL_HIGHBOOSTFREQ = WidgetManager().CreateDial(hWnd, 110, 276, 19, 19, 0.f, 0.f, 0.002f, 1.f, true);
	DIAL_HIGHBOOST = WidgetManager().CreateDial(hWnd, 147, 276, 19, 19, 0.f, 0.f, 0.002f, 1.f, true);

	DIAL_LOWBOOSTFREQ = WidgetManager().CreateDial(hWnd, 20, 276, 19, 19, 150.f, 0.f, 0.002f, 1.f, true);
	DIAL_LOWBOOST = WidgetManager().CreateDial(hWnd, 57, 276, 19, 19, 0.f, 0.f, 0.002f, 1.f, true);

	DIAL_FREQ_OFFSET = WidgetManager().CreateDial(hWnd, 200, 276, 19, 19, 0.f, 0.f, 0.002f, 1.f, false);
	DIAL_AMP_OFFSET = WidgetManager().CreateDial(hWnd, 234, 276, 19, 19, 0.f, 0.f, 0.002f, 1.f, false);

	DIAL_BITCRUSH = WidgetManager().CreateDial(hWnd, 286, 277, 19, 19, 0.f, 0.f, 0.002f, 1.f, true);
	BUTTON_BCSMOOTH = WidgetManager().CreateButton(hWnd, 318, 277, 0, BMP_SMOOTH);
	BUTTON_BCSHARP = WidgetManager().CreateButton(hWnd, 318, 294, 0, BMP_SHARP);

	DIAL_BITREFLECT = WidgetManager().CreateDial(hWnd, 370, 277, 19, 19, 0.f, 0.f, 0.002f, 1.f, true);
	BUTTON_BRSMOOTH = WidgetManager().CreateButton(hWnd, 402, 277, 0, BMP_SMOOTH);
	BUTTON_BRSHARP = WidgetManager().CreateButton(hWnd, 402, 294, 0, BMP_SHARP);

	int gfx[24];
	gfx[0] = 0;
	gfx[1] = BMP_SIN;
	gfx[2] = BMP_SQR;
	gfx[3] = BMP_SAW;
	gfx[4] = BMP_TRI;
	gfx[5] = BMP_SINAA;
	gfx[6] = BMP_SQRAA;
	gfx[7] = BMP_SAWAA;
	gfx[8] = BMP_TRIAA;
	gfx[9] = BMP_LSQR;
	gfx[10] = BMP_LSAW;
	gfx[11] = BMP_LTRI;
	gfx[12] = BMP_LTAP;
	gfx[13] = BMP_LSLI;
	gfx[14] = BMP_LIMP;
	gfx[15] = BMP_LBIP;
	gfx[16] = BMP_LRND1;
	gfx[17] = BMP_LRND2;
	gfx[18] = BMP_LRBEZ;
	gfx[19] = BMP_LRSPL;
	gfx[20] = BMP_LNOI1;
	gfx[21] = BMP_LNOI2;
	gfx[22] = BMP_LNOI3;
	gfx[23] = BMP_LNOI4;

	GFXBOX_OSC1 = WidgetManager().CreateGraphicBox(hWnd, 399, 144, 0, gfx, 24);
	GFXBOX_OSC2 = WidgetManager().CreateGraphicBox(hWnd, 276, 144, 0, gfx, 24);
	GFXBOX_OSC3 = WidgetManager().CreateGraphicBox(hWnd, 153, 144, 0, gfx, 24);
	GFXBOX_OSC4 = WidgetManager().CreateGraphicBox(hWnd, 30, 144, 0, gfx, 24);
	GFXBOX_OSC5 = WidgetManager().CreateGraphicBox(hWnd, 399, 33, 0, gfx, 24);
	GFXBOX_OSC6 = WidgetManager().CreateGraphicBox(hWnd, 276, 33, 0, gfx, 24);
	GFXBOX_OSC7 = WidgetManager().CreateGraphicBox(hWnd, 153, 33, 0, gfx, 24);
	GFXBOX_OSC8 = WidgetManager().CreateGraphicBox(hWnd, 30, 33, 0, gfx, 24);

	//Get pointers
	pSampleScope = ((SampleScope*)WidgetManager().GetWidgetPointer(SAMPLESCOPE));
	pA = ((Dial*)WidgetManager().GetWidgetPointer(DIAL_A));
	pD = ((Dial*)WidgetManager().GetWidgetPointer(DIAL_D));
	pS = ((Dial*)WidgetManager().GetWidgetPointer(DIAL_S));
	pR = ((Dial*)WidgetManager().GetWidgetPointer(DIAL_R));

	//Init the synth engine (must always be done last)
	SynthEngine.Init(44100);

	//Make sure all oscillators share the same default values
	for(int i = 0; i < 8; ++i)
	{
		SynthEngine.m_selected = i;
		SynthEngine.UpdateSelectedOscillator();
	}

	//Set selected oscillator
	SynthEngine.m_selected = 0;
}


//Main Dialog
BOOL CALLBACK MainDlgProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch(Message)
	{

		case WM_INITDIALOG:
		{
			//Set Icon
			SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(IDI_ICON_SMALL)));
			SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(IDI_ICON_BIG)));

			//Load Background
			hBack = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(BMP_BACK));
			GetObject(hBack, sizeof(bmBack), &bmBack);
			hCredit = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(BMP_CREDIT));
			GetObject(hCredit, sizeof(bmCredit), &bmCredit);

			//Load OSC highlight graphics
			hOSC[0] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(BMP_OSC1));
			GetObject(hOSC[0], sizeof(bmOSC[0]), &bmOSC[0]);
			hOSC[1] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(BMP_OSC2));
			GetObject(hOSC[1], sizeof(bmOSC[1]), &bmOSC[1]);
			hOSC[2] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(BMP_OSC3));
			GetObject(hOSC[2], sizeof(bmOSC[2]), &bmOSC[2]);
			hOSC[3] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(BMP_OSC4));
			GetObject(hOSC[3], sizeof(bmOSC[3]), &bmOSC[3]);
			hOSC[4] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(BMP_OSC5));
			GetObject(hOSC[4], sizeof(bmOSC[4]), &bmOSC[4]);
			hOSC[5] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(BMP_OSC6));
			GetObject(hOSC[5], sizeof(bmOSC[5]), &bmOSC[5]);
			hOSC[6] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(BMP_OSC7));
			GetObject(hOSC[6], sizeof(bmOSC[6]), &bmOSC[6]);
			hOSC[7] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(BMP_OSC8));
			GetObject(hOSC[7], sizeof(bmOSC[7]), &bmOSC[7]);

			//Set Timer
			SetTimer(hWnd, TimerGen, 1, 0);

			//Set the status text location
			WidgetManager().SetStatusRect(326, 450, 9, 130);

			//Create Widgets
			CreateWidgets(hWnd);

			//Init Audio
			InitAudio(44100);
		}
		break;

		case WM_TIMER:
        {
			//Prevent mouse press/release flood
			static bool tc1 = false, tc2 = false;

			//Is this window foreground?
			if(GetForegroundWindow() == hWnd)
			{
				//Hotkeys
				if(GetAsyncKeyState(VK_XBUTTON1)&1 || GetAsyncKeyState(VK_RETURN)&1)
				{
					PlaySample(SynthEngine.GetSample(), SynthEngine.GetSampleSize());
					break;
				}

				//Check for left click
				if(GetAsyncKeyState(VK_LBUTTON)&0x8000)
				{
					if(tc1 == false)
					{
						HighlightSelectedOscillator(hWnd, mousex, mousey);
						WidgetManager().MouseState(false, true);
						tc1 = true;
					}
				}
				else
				{
					if(tc1 == true)
					{
						WidgetManager().MouseState(false, false);
						tc1 = false;
					}
				}

				//Check for right click
				if(GetAsyncKeyState(VK_RBUTTON)&0x8000)
				{
					if(tc2 == false)
					{
						WidgetManager().MouseState(true, true);
						tc2 = true;
					}
				}
				else
				{
					if(tc2 == true)
					{
						WidgetManager().MouseState(true, false);
						tc2 = false;
					}
				}
			}

			//Update widgets
			WidgetManager().Update();
		}
		break;

		case WM_MOUSEMOVE:
		{
			mousex = LOWORD(lParam), mousey = HIWORD(lParam);
			WidgetManager().MouseMove(mousex, mousey);
			HoverSelectedOscillator(hWnd, mousex, mousey);
		}
		break;

		//case WM_LBUTTONDOWN:
		//{
			//HighlightSelectedOscillator(hWnd, LOWORD(lParam), HIWORD(lParam));
		//}
		//break;

		//case WM_LBUTTONUP:
		//{
		//	WidgetManager().MouseState(false, false);
		//}
		//break;

		//case WM_RBUTTONUP:
		//{
		//	WidgetManager().MouseState(true, false);
		//}
		//break;

		case WM_PAINT:
		{
			//Begin Paint
			hdcWindow = BeginPaint(hWnd, &ps);
			HDC hdcMemory = CreateCompatibleDC(hdcWindow);

			//Render Background
			SelectObject(hdcMemory, hBack);
			BitBlt(hdcWindow, 0, 0, bmBack.bmWidth, bmBack.bmHeight, hdcMemory, 0, 0, SRCCOPY);

			//Render Credits
			if(CreditEnabled == true)
			{
				SelectObject(hdcMemory, hCredit);
				BitBlt(hdcWindow, 20, 339, bmCredit.bmWidth, bmCredit.bmHeight, hdcMemory, 0, 0, SRCCOPY);
			}

			//Render Buttons
			WidgetManager().Render(hdcMemory, hdcWindow);

			//Highlight Selected Oscillator
			DrawOscSelect(hdcWindow, osch_x, osch_y);

			//If hovering over an oscillator show which one
			if(m_hovering != -1)
			{
				SelectObject(hdcMemory, hOSC[m_hovering]);
				BitBlt(hdcWindow, hover_x+31, hover_y-2, bmOSC[m_hovering].bmWidth, bmOSC[m_hovering].bmHeight, hdcMemory, 0, 0, SRCCOPY);
			}

			//End Paint
			DeleteDC(hdcMemory);
			EndPaint(hWnd, &ps);
		}
		break;

		case WM_COMMAND:
		{
			if(wParam == HOVER_ABOUT)
			{
				CreditEnabled = !CreditEnabled;

				if(CreditEnabled == true)
				{
					pSampleScope->UpdateSample(NULL, 0);
					InvalidateRect(hWnd, 0, FALSE);
					break;
				}
			}

			if(wParam == HOVER_PLAY)
			{
				PlaySample(SynthEngine.GetSample(), SynthEngine.GetSampleSize());
				break;
			}

			if(wParam == HOVER_NEW)
			{
				if(IsAudioPlaying() == true)
					break;

				if(MessageBox(hWnd, "Are you sure you want to start a new project?", "Alert", MB_YESNO | MB_TOPMOST) == IDYES)
				{
					int s = SynthEngine.m_selected;
					WidgetManager().DeleteAllWidgets();
					CreateWidgets(hWnd);
					SynthEngine.m_selected = s;
					InvalidateRect(hWnd, 0, FALSE);
				}
				break;
			}

			if(wParam == HOVER_EXPORT)
			{
				if(IsAudioPlaying() == true)
					break;

				char file[MAX_PATH];
				ZeroMemory(file, sizeof(file));

				OPENFILENAME ofn;
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.hInstance = GetModuleHandle(0);
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = GetForegroundWindow();
				ofn.lpstrFilter = "WAV Files (*.wav*)\0*.wav*\0PCM Files (*.pcm*)\0*.pcm*\0\0";
				ofn.lpstrFile = file;
				ofn.nMaxFile = MAX_PATH;
				ofn.Flags =  OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
				ofn.lpstrDefExt = "";

				if(GetSaveFileName(&ofn) != 0)
				{
					if(ofn.nFilterIndex == 1)
					{
						strcat(file, ".wav");
						SynthEngine.WriteSampleToWAV(file);
					}

					if(ofn.nFilterIndex == 2)
					{
						strcat(file, ".pcm");
						SynthEngine.WriteSampleToPCM(file);
					}
				}
				break;
			}

			if(wParam == HOVER_SAVE)
			{
				if(IsAudioPlaying() == true)
					break;

				char file[MAX_PATH];
				ZeroMemory(file, sizeof(file));

				OPENFILENAME ofn;
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.hInstance = GetModuleHandle(0);
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = GetForegroundWindow();
				ofn.lpstrFilter = "BRG Files (*.brg*)\0*.brg*\0";
				ofn.lpstrFile = file;
				ofn.nMaxFile = MAX_PATH;
				ofn.Flags =  OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
				ofn.lpstrDefExt = "brg";

				if(GetSaveFileName(&ofn) != 0)
				{
					WidgetManager().SaveState(file);

					//Set title to show saved state
					char wintitle[256];
					char filename[64];
					GetFileName(file, filename);
					sprintf(wintitle, "Borg ER-2 - %s", filename);
					SetWindowText(hWnd, wintitle);
				}

				break;
			}

			if(wParam == HOVER_LOAD)
			{
				if(IsAudioPlaying() == true)
					break;

				char file[MAX_PATH];
				ZeroMemory(file, sizeof(file));

				OPENFILENAME ofn;
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.hInstance = GetModuleHandle(0);
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = GetForegroundWindow();
				ofn.lpstrFilter = "BRG Files (*.brg*)\0*.brg*\0";
				ofn.lpstrFile = file;
				ofn.nMaxFile = MAX_PATH;
				ofn.Flags =  OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
				ofn.lpstrDefExt = "brg";

				if(GetOpenFileName(&ofn) != 0)
				{
					//Clear State
					WidgetManager().DeleteAllWidgets();
					CreateWidgets(hWnd);

					//Load State
					WidgetManager().LoadState(file);
					UpdateGFXSelectedOSC(hWnd);

					//Set title to show loaded state
					char wintitle[256];
					char filename[64];
					GetFileName(file, filename);
					sprintf(wintitle, "Borg ER-2 - %s", filename);
					SetWindowText(hWnd, wintitle);
				}

				//Update Oscillator
				SynthEngine.UpdateSelectedOscillator();

				//Update Sample each time a setting is changed
				SynthEngine.CreateSample();
				pSampleScope->UpdateSample(SynthEngine.GetSelectedSample(), SynthEngine.GetSampleSize());

				break;
			}


			if(wParam == BUTTON_OSC5_L)
			{
				((GfxBox*)WidgetManager().GetWidgetPointer(GFXBOX_OSC5))->ScrollLeft();
			}

			if(wParam == BUTTON_OSC5_R)
			{
				((GfxBox*)WidgetManager().GetWidgetPointer(GFXBOX_OSC5))->ScrollRight();
			}

			if(wParam == BUTTON_OSC6_L)
			{
				((GfxBox*)WidgetManager().GetWidgetPointer(GFXBOX_OSC6))->ScrollLeft();
			}

			if(wParam == BUTTON_OSC6_R)
			{
				((GfxBox*)WidgetManager().GetWidgetPointer(GFXBOX_OSC6))->ScrollRight();
			}

			if(wParam == BUTTON_OSC7_L)
			{
				((GfxBox*)WidgetManager().GetWidgetPointer(GFXBOX_OSC7))->ScrollLeft();
			}

			if(wParam == BUTTON_OSC7_R)
			{
				((GfxBox*)WidgetManager().GetWidgetPointer(GFXBOX_OSC7))->ScrollRight();
			}

			if(wParam == BUTTON_OSC8_L)
			{
				((GfxBox*)WidgetManager().GetWidgetPointer(GFXBOX_OSC8))->ScrollLeft();
			}

			if(wParam == BUTTON_OSC8_R)
			{
				((GfxBox*)WidgetManager().GetWidgetPointer(GFXBOX_OSC8))->ScrollRight();
			}

			if(wParam == BUTTON_OSC1_L)
			{
				((GfxBox*)WidgetManager().GetWidgetPointer(GFXBOX_OSC1))->ScrollLeft();
			}

			if(wParam == BUTTON_OSC1_R)
			{
				((GfxBox*)WidgetManager().GetWidgetPointer(GFXBOX_OSC1))->ScrollRight();
			}

			if(wParam == BUTTON_OSC2_L)
			{
				((GfxBox*)WidgetManager().GetWidgetPointer(GFXBOX_OSC2))->ScrollLeft();
			}

			if(wParam == BUTTON_OSC2_R)
			{
				((GfxBox*)WidgetManager().GetWidgetPointer(GFXBOX_OSC2))->ScrollRight();
			}

			if(wParam == BUTTON_OSC3_L)
			{
				((GfxBox*)WidgetManager().GetWidgetPointer(GFXBOX_OSC3))->ScrollLeft();
			}

			if(wParam == BUTTON_OSC3_R)
			{
				((GfxBox*)WidgetManager().GetWidgetPointer(GFXBOX_OSC3))->ScrollRight();
			}

			if(wParam == BUTTON_OSC4_L)
			{
				((GfxBox*)WidgetManager().GetWidgetPointer(GFXBOX_OSC4))->ScrollLeft();
			}

			if(wParam == BUTTON_OSC4_R)
			{
				((GfxBox*)WidgetManager().GetWidgetPointer(GFXBOX_OSC4))->ScrollRight();
			}

			if(wParam == BUTTON_AM1)
			{
				((Button*)WidgetManager().GetWidgetPointer(BUTTON_AM1))->Toggle();
			}

			if(wParam == BUTTON_FM1)
			{
				((Button*)WidgetManager().GetWidgetPointer(BUTTON_FM1))->Toggle();
			}

			if(wParam == BUTTON_AM2)
			{
				((Button*)WidgetManager().GetWidgetPointer(BUTTON_AM2))->Toggle();
			}

			if(wParam == BUTTON_FM2)
			{
				((Button*)WidgetManager().GetWidgetPointer(BUTTON_FM2))->Toggle();
			}

			if(wParam == BUTTON_AM3)
			{
				((Button*)WidgetManager().GetWidgetPointer(BUTTON_AM3))->Toggle();
			}

			if(wParam == BUTTON_FM3)
			{
				((Button*)WidgetManager().GetWidgetPointer(BUTTON_FM3))->Toggle();
			}

			if(wParam == BUTTON_AM4)
			{
				((Button*)WidgetManager().GetWidgetPointer(BUTTON_AM4))->Toggle();
			}

			if(wParam == BUTTON_FM4)
			{
				((Button*)WidgetManager().GetWidgetPointer(BUTTON_FM4))->Toggle();
			}

			if(wParam == BUTTON_AM5)
			{
				((Button*)WidgetManager().GetWidgetPointer(BUTTON_AM5))->Toggle();
			}

			if(wParam == BUTTON_FM5)
			{
				((Button*)WidgetManager().GetWidgetPointer(BUTTON_FM5))->Toggle();
			}

			if(wParam == BUTTON_AM6)
			{
				((Button*)WidgetManager().GetWidgetPointer(BUTTON_AM6))->Toggle();
			}

			if(wParam == BUTTON_FM6)
			{
				((Button*)WidgetManager().GetWidgetPointer(BUTTON_FM6))->Toggle();
			}

			if(wParam == BUTTON_AM7)
			{
				((Button*)WidgetManager().GetWidgetPointer(BUTTON_AM7))->Toggle();
			}

			if(wParam == BUTTON_FM7)
			{
				((Button*)WidgetManager().GetWidgetPointer(BUTTON_FM7))->Toggle();
			}

			if(wParam == BUTTON_AM8)
			{
				((Button*)WidgetManager().GetWidgetPointer(BUTTON_AM8))->Toggle();
			}

			if(wParam == BUTTON_FM8)
			{
				((Button*)WidgetManager().GetWidgetPointer(BUTTON_FM8))->Toggle();
			}

			if(wParam == BUTTON_AM9)
			{
				((Button*)WidgetManager().GetWidgetPointer(BUTTON_AM9))->Toggle();
			}

			if(wParam == BUTTON_FM9)
			{
				((Button*)WidgetManager().GetWidgetPointer(BUTTON_FM9))->Toggle();
			}


			if(wParam == BUTTON_AM10)
			{
				((Button*)WidgetManager().GetWidgetPointer(BUTTON_AM10))->Toggle();
			}

			if(wParam == BUTTON_FM10)
			{
				((Button*)WidgetManager().GetWidgetPointer(BUTTON_FM10))->Toggle();
			}

			if(wParam == BUTTON_ADD1)
			{
				DoAddSubButton(BUTTON_ADD1);
			}

			if(wParam == BUTTON_ADD2)
			{
				DoAddSubButton(BUTTON_ADD2);
			}

			if(wParam == BUTTON_ADD3)
			{
				DoAddSubButton(BUTTON_ADD3);
			}

			if(wParam == BUTTON_ADD4)
			{
				DoAddSubButton(BUTTON_ADD4);
			}

			if(wParam == BUTTON_ADD5)
			{
				DoAddSubButton(BUTTON_ADD5);
			}

			if(wParam == BUTTON_ADD6)
			{
				DoAddSubButton(BUTTON_ADD6);
			}

			if(wParam == BUTTON_ADD7)
			{
				DoAddSubButton(BUTTON_ADD7);
			}

			if(wParam == BUTTON_ADD8)
			{
				DoAddSubButton(BUTTON_ADD8);
			}

			if(wParam == BUTTON_ADD9)
			{
				DoAddSubButton(BUTTON_ADD9);
			}

			if(wParam == BUTTON_ADD10)
			{
				DoAddSubButton(BUTTON_ADD10);
			}

			if(wParam == BUTTON_SMOOTH)
			{
				((Button*)WidgetManager().GetWidgetPointer(BUTTON_SMOOTH))->Toggle();
				((Button*)WidgetManager().GetWidgetPointer(BUTTON_SHARP))->UnSetUnpressedGraphic();
			}

			if(wParam == BUTTON_SHARP)
			{
				((Button*)WidgetManager().GetWidgetPointer(BUTTON_SHARP))->Toggle();
				((Button*)WidgetManager().GetWidgetPointer(BUTTON_SMOOTH))->UnSetUnpressedGraphic();
			}

			if(wParam == BUTTON_BCSMOOTH)
			{
				((Button*)WidgetManager().GetWidgetPointer(BUTTON_BCSMOOTH))->Toggle();
				((Button*)WidgetManager().GetWidgetPointer(BUTTON_BCSHARP))->UnSetUnpressedGraphic();
			}

			if(wParam == BUTTON_BCSHARP)
			{
				((Button*)WidgetManager().GetWidgetPointer(BUTTON_BCSHARP))->Toggle();
				((Button*)WidgetManager().GetWidgetPointer(BUTTON_BCSMOOTH))->UnSetUnpressedGraphic();
			}

			if(wParam == BUTTON_BRSMOOTH)
			{
				((Button*)WidgetManager().GetWidgetPointer(BUTTON_BRSMOOTH))->Toggle();
				((Button*)WidgetManager().GetWidgetPointer(BUTTON_BRSHARP))->UnSetUnpressedGraphic();
			}

			if(wParam == BUTTON_BRSHARP)
			{
				((Button*)WidgetManager().GetWidgetPointer(BUTTON_BRSHARP))->Toggle();
				((Button*)WidgetManager().GetWidgetPointer(BUTTON_BRSMOOTH))->UnSetUnpressedGraphic();
			}

			//Update Oscillator
			SynthEngine.UpdateSelectedOscillator();

			//Update Sample each time a setting is changed
			SynthEngine.CreateSample();
			pSampleScope->UpdateSample(SynthEngine.GetSelectedSample(), SynthEngine.GetSelectedSampleSize());
		}
		break;

		case WM_CLOSE:
		{
			KillAudio();
			DeleteDC(hdcWindow);
			DeleteObject(hBack);
			DeleteDC(hdcWindow);
			EndDialog(hWnd, 0);
			ExitProcess(0);
		}
		break;

	default:
		return FALSE;
	}
	return TRUE;
}


//Main Function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{ 
	HANDLE CheckInstance = CreateMutex(NULL, FALSE, "borger2jc398");
	if(GetLastError() == ERROR_ALREADY_EXISTS){ExitProcess(0);}

	DialogBox(GetModuleHandle(0), MAKEINTRESOURCE(IDD_MAIN), 0, MainDlgProc);

	return 0;
}
