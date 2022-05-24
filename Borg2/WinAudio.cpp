/*
	Code written by James Fletcher (~2010).
	https://github.com/mrbid
*/

#include "WinAudio.h"

#include <windows.h>
#include <mmsystem.h>

//Global Variables
HWAVEOUT hWaveOut;
WAVEFORMATEX wavfmt;
WAVEHDR header;
HANDLE hAudioThread;
DWORD AudioThreadID;
bool m_playsample;
bool m_killaudiothread;
char* m_sample;
int m_samplesize;

//Audio Thread
DWORD WINAPI AudioThread(void* param)
{
	//Audio Thread Loop
	while(1)
	{
		//Don't go crazy
		Sleep(100);

		//Play Sample
		if(m_playsample == true)
		{
			ZeroMemory(&header, sizeof(WAVEHDR));
			header.dwBufferLength = m_samplesize;
			header.lpData = m_sample;

			waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
			waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));

			while(waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR)) == WAVERR_STILLPLAYING)
					Sleep(10);

			m_playsample = false;
		}

		//Exit Thread
		if(m_killaudiothread == true)
		{
			break;
		}
	}

	//Exit Fine
	return true;
}

//Init Windows Audio
void InitAudio(int samplerate)
{
	m_killaudiothread = false;
	m_playsample = false;

	//Setup Wave Format Info
	wavfmt.nSamplesPerSec = samplerate;
	wavfmt.wBitsPerSample = 16;
	wavfmt.nChannels = 1;
	wavfmt.cbSize = 0;
	wavfmt.wFormatTag = WAVE_FORMAT_PCM;
	wavfmt.nBlockAlign = (wavfmt.wBitsPerSample >> 3) * wavfmt.nChannels;
	wavfmt.nAvgBytesPerSec = wavfmt.nBlockAlign * wavfmt.nSamplesPerSec;

	//Open Wave Out
	if(waveOutOpen(&hWaveOut, WAVE_MAPPER, &wavfmt, 0, 0, CALLBACK_NULL) != MMSYSERR_NOERROR)
	{
		MessageBox(0, "Failed to open audio device.", "Error.", MB_OK | MB_TOPMOST);
		return;
	}

	//Create Audio Thread
	hAudioThread = CreateThread(0, 0, AudioThread, 0, 0, &AudioThreadID);
}

//Kill Windows Audio
void KillAudio()
{
	m_killaudiothread = true;
	waveOutClose(hWaveOut);
}

//Is audio playing
bool IsAudioPlaying()
{
	return m_playsample;
}

//Play Audio Sample
void PlaySample(char* sample, int size)
{
	if(sample == NULL || size == 0)
		return;

	m_sample = sample;
	m_samplesize = size;
	m_playsample = true;
}




