/*
	Code written by James Fletcher (~2010).
	https://github.com/mrbid
*/

#ifndef WINAUDIO_H
#define WINAUDIO_H

void InitAudio(int samplerate);
void PlaySample(char* sample, int size);
void KillAudio();
bool IsAudioPlaying();

#endif





