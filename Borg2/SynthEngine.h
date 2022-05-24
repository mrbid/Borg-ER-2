/*
	Code written by James Fletcher (~2010).
	https://github.com/mrbid
*/

#ifndef SYNTHENGINE_H
#define SYNTHENGINE_H

#include "WidgetManager.h"
#include "Oscillator.h"
#include "Filters.h"
#include "ADSR.h"
#include "BitCrusher.h"
#include "BitReflecter.h"

//Defines
#define OSC1 0
#define OSC2 1
#define OSC3 2
#define OSC4 3
#define OSC5 4
#define OSC6 5
#define OSC7 6
#define OSC8 7
#define MATH_NONE 0
#define MATH_ADD 1
#define MATH_SUB 2
#define MATH_TIMES 3

//Oscillator Dial Data
struct oscdata
{
	float A,D,S,SL,R;
	char ADSR_TYPE;
	float LOWPASS_CUTOFF;
	float LOWPASS_RESONANCE;
	float HIGHPASS_CUTOFF;
	float HIGHPASS_RESONANCE;
	float LOWBOOST_FREQ;
	float LOWBOOST;
	float HIGHBOOST_FREQ;
	float HIGHBOOST;
	float FREQ_OFFSET;
	float AMP_OFFSET;
	float BITCRUSH;
	char BITCRUSH_TYPE;
	float BITREFLECT;
	char BITREFLECT_TYPE;
};

//Synth Engine Class
class SynthEngine
{
public:
	
	//Init
	void Init(int samplerate);

	//Result of all eight oscillators synthesized
	void Synth();

	//Create a sample
	void CreateSample();

	//Write Sample to WAV
	void WriteSampleToWAV(char* filename);

	//Write Sample to PCM
	void WriteSampleToPCM(char* filename);

	//Save all 8 oscillator states
	void SaveState(FILE* out);

	//Load all 8 oscillator states
	void LoadState(FILE* out);

	//Get pointer to sample
	char* GetSample();

	//Get pointer to selected sample
	float* GetSelectedSample();

	//Get size of sample
	long GetSampleSize();

	//Get size of selected sample
	long GetSelectedSampleSize();

	//Update Filter Info for Selected Oscillator
	void UpdateSelectedOscillator();

	//Show filter info on dialog
	void ShowSelectedOscillator();

	//Selected Oscillator
	int m_selected;

private:

	//Synth an Individual Oscillator
	void SynthOsc( int oscid, int shape, float freq, float amp, //this oscillator
				   float* input1, bool fm1, bool am1, char math1, //horisontal oscillator
				   float* input2, bool fm2, bool am2, char math2, //vertical oscillator
				   float* out); //Output sample from oscillator

	//ADSR Filters
	ADSR m_adsr[8];

	//Filters
	Filters m_filters[8];

	//BitCrusher
	BitCrusher m_bitcrusher[8];

	//BitReflecter
	BitReflecter m_bitreflecter[8];

	//Oscillator Dial Data
	__forceinline void ResetOscData()
	{
		for(int i = 0; i < 8; ++i)
			memset(m_oscdata, 0x00, sizeof(oscdata));
	}
	oscdata m_oscdata[8];

	//Clamp the output to prevent clipping
	float ClampClip(float f);

	//Length of sample in seconds
	float m_seconds;

	//Sample Rate
	int m_samplerate;

	//Audio Sample
	char* m_sample;
	float* m_fsample[8];
	long m_fsamplesize[8];
	long m_samplesize;

	//Oscillator
	Oscillator m_osc;

	//Widget Pointers
	Button *AM1, *FM1, *AM2, *FM2, *AM3, *FM3, *AM4, *FM4, *AM5, *FM5, *AM6,
		*FM6, *AM7, *FM7, *AM8, *FM8, *AM9, *FM9, *AM10, *FM10, *ADD1, *ADD2,
		*ADD3, *ADD4, *ADD5, *ADD6, *ADD7, *ADD8, *ADD9, *ADD10, *SMOOTH, *SHARP,
		*BCSHARP, *BCSMOOTH, *BRSHARP, *BRSMOOTH;
	Dial *DIAL1_FREQ, *DIAL1_AMP, *DIAL2_FREQ, *DIAL2_AMP, *DIAL3_FREQ, *DIAL3_AMP,
		*DIAL4_FREQ, *DIAL4_AMP, *DIAL5_FREQ, *DIAL5_AMP, *DIAL6_FREQ, *DIAL6_AMP,
		*DIAL7_FREQ, *DIAL7_AMP, *DIAL8_FREQ, *DIAL8_AMP, *DIALP_A, *DIALP_D, *DIALP_S,
		*DIALP_SL, *DIALP_R, *DIALP_LOWBOOST, *DIALP_CUTOFF, *DIALP_LOWPASS_RESONANCE,
		*DIALP_LOWPASS_CUTOFF, *DIALP_HIGHPASS_CUTOFF, *DIALP_HIGHPASS_RESONANCE,
		*DIALP_LOWBOOSTFREQ, *DIALP_BITCRUSH, *DIALP_HIGHBOOST, *DIALP_HIGHBOOSTFREQ,
		*DIALP_FREQ_OFFSET, *DIALP_AMP_OFFSET, *DIALP_BITREFLECT;
	GfxBox *GFXBOX1, *GFXBOX2, *GFXBOX3, *GFXBOX4, *GFXBOX5, *GFXBOX6, *GFXBOX7, *GFXBOX8;

};


__forceinline char* SynthEngine::GetSample(){return m_sample;}
__forceinline float* SynthEngine::GetSelectedSample(){return m_fsample[m_selected];}
__forceinline long SynthEngine::GetSelectedSampleSize(){return m_fsamplesize[m_selected];}
__forceinline long SynthEngine::GetSampleSize(){return m_samplesize;}
__forceinline float SynthEngine::ClampClip(float f){return f < -127.0f ? -127.0f : f > 127.0f ? 127.0f : f;}

#endif




