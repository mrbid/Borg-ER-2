/*
	Code written by James Fletcher.
	Copyright 2010 - 2011.
	ethocybin@googlemail.com
	http://www.mrpuzzle.org/
*/

#ifndef WINAUDIO_H
#define WINAUDIO_H

void InitAudio(int samplerate);
void PlaySample(char* sample, int size);
void KillAudio();
bool IsAudioPlaying();

#endif





