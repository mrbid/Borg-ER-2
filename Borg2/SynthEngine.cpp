/*
	Code written by James Fletcher (~2010).
	https://github.com/mrbid

	I could reduce duplicate branches in CreateSample()
*/

#include "SynthEngine.h"
#include "Borg2.h"
#include "WinAudio.h"
#include "float_cast.h"


//Init Synth Engine
void SynthEngine::Init(int samplerate)
{
	//Set Samplerate
	m_samplerate = samplerate;

	//Set Selected Oscillator
	m_selected = 0;

	//Reset osc data
	ResetOscData();

	//Clear all samples
	for(int i = 0; i < 8; ++i)
		if(m_fsample[i] != NULL)
			for(int i2 = 0; i2 < m_fsamplesize[i]; ++i2)
				m_fsample[i][i2] = 0.f;

	//Init Oscillator
	m_osc.Init(8, m_samplerate, 1000, 1000, 50);

	//Init filters
	for(int i = 0; i < 8; i++)
		m_filters[i].InitFilters(m_samplerate);

	//Get direct pointers to all of the control widgets
	AM1 = ((Button*)WidgetManager().GetWidgetPointer(BUTTON_AM1));
	FM1 = ((Button*)WidgetManager().GetWidgetPointer(BUTTON_FM1));
	AM2 = ((Button*)WidgetManager().GetWidgetPointer(BUTTON_AM2));
	FM2 = ((Button*)WidgetManager().GetWidgetPointer(BUTTON_FM2));
	AM3 = ((Button*)WidgetManager().GetWidgetPointer(BUTTON_AM3));
	FM3 = ((Button*)WidgetManager().GetWidgetPointer(BUTTON_FM3));
	AM4 = ((Button*)WidgetManager().GetWidgetPointer(BUTTON_AM4));
	FM4 = ((Button*)WidgetManager().GetWidgetPointer(BUTTON_FM4));
	AM5 = ((Button*)WidgetManager().GetWidgetPointer(BUTTON_AM5));
	FM5 = ((Button*)WidgetManager().GetWidgetPointer(BUTTON_FM5));
	AM6 = ((Button*)WidgetManager().GetWidgetPointer(BUTTON_AM6));
	FM6 = ((Button*)WidgetManager().GetWidgetPointer(BUTTON_FM6));
	AM7 = ((Button*)WidgetManager().GetWidgetPointer(BUTTON_AM7));
	FM7 = ((Button*)WidgetManager().GetWidgetPointer(BUTTON_FM7));
	AM8 = ((Button*)WidgetManager().GetWidgetPointer(BUTTON_AM8));
	FM8 = ((Button*)WidgetManager().GetWidgetPointer(BUTTON_FM8));
	AM9 = ((Button*)WidgetManager().GetWidgetPointer(BUTTON_AM9));
	FM9 = ((Button*)WidgetManager().GetWidgetPointer(BUTTON_FM9));
	AM10 = ((Button*)WidgetManager().GetWidgetPointer(BUTTON_AM10));
	FM10 = ((Button*)WidgetManager().GetWidgetPointer(BUTTON_FM10));

	ADD1 = ((Button*)WidgetManager().GetWidgetPointer(BUTTON_ADD1));
	ADD2 = ((Button*)WidgetManager().GetWidgetPointer(BUTTON_ADD2));
	ADD3 = ((Button*)WidgetManager().GetWidgetPointer(BUTTON_ADD3));
	ADD4 = ((Button*)WidgetManager().GetWidgetPointer(BUTTON_ADD4));
	ADD5 = ((Button*)WidgetManager().GetWidgetPointer(BUTTON_ADD5));
	ADD6 = ((Button*)WidgetManager().GetWidgetPointer(BUTTON_ADD6));
	ADD7 = ((Button*)WidgetManager().GetWidgetPointer(BUTTON_ADD7));
	ADD8 = ((Button*)WidgetManager().GetWidgetPointer(BUTTON_ADD8));
	ADD9 = ((Button*)WidgetManager().GetWidgetPointer(BUTTON_ADD9));
	ADD10 = ((Button*)WidgetManager().GetWidgetPointer(BUTTON_ADD10));

	SMOOTH = ((Button*)WidgetManager().GetWidgetPointer(BUTTON_SMOOTH));
	SHARP = ((Button*)WidgetManager().GetWidgetPointer(BUTTON_SHARP));

	BCSMOOTH = ((Button*)WidgetManager().GetWidgetPointer(BUTTON_BCSMOOTH));
	BCSHARP = ((Button*)WidgetManager().GetWidgetPointer(BUTTON_BCSHARP));

	BRSMOOTH = ((Button*)WidgetManager().GetWidgetPointer(BUTTON_BRSMOOTH));
	BRSHARP = ((Button*)WidgetManager().GetWidgetPointer(BUTTON_BRSHARP));

	DIALP_A = ((Dial*)WidgetManager().GetWidgetPointer(DIAL_A));
	DIALP_D = ((Dial*)WidgetManager().GetWidgetPointer(DIAL_D));
	DIALP_S = ((Dial*)WidgetManager().GetWidgetPointer(DIAL_S));
	DIALP_SL = ((Dial*)WidgetManager().GetWidgetPointer(DIAL_SL));
	DIALP_R = ((Dial*)WidgetManager().GetWidgetPointer(DIAL_R));
	
	DIAL1_FREQ = ((Dial*)WidgetManager().GetWidgetPointer(DIAL_OSC1_FREQ));
	DIAL1_AMP = ((Dial*)WidgetManager().GetWidgetPointer(DIAL_OSC1_AMP));
	DIAL2_FREQ = ((Dial*)WidgetManager().GetWidgetPointer(DIAL_OSC2_FREQ));
	DIAL2_AMP = ((Dial*)WidgetManager().GetWidgetPointer(DIAL_OSC2_AMP));
	DIAL3_FREQ = ((Dial*)WidgetManager().GetWidgetPointer(DIAL_OSC3_FREQ));
	DIAL3_AMP = ((Dial*)WidgetManager().GetWidgetPointer(DIAL_OSC3_AMP));
	DIAL4_FREQ = ((Dial*)WidgetManager().GetWidgetPointer(DIAL_OSC4_FREQ));
	DIAL4_AMP = ((Dial*)WidgetManager().GetWidgetPointer(DIAL_OSC4_AMP));
	DIAL5_FREQ = ((Dial*)WidgetManager().GetWidgetPointer(DIAL_OSC5_FREQ));
	DIAL5_AMP = ((Dial*)WidgetManager().GetWidgetPointer(DIAL_OSC5_AMP));
	DIAL6_FREQ = ((Dial*)WidgetManager().GetWidgetPointer(DIAL_OSC6_FREQ));
	DIAL6_AMP = ((Dial*)WidgetManager().GetWidgetPointer(DIAL_OSC6_AMP));
	DIAL7_FREQ = ((Dial*)WidgetManager().GetWidgetPointer(DIAL_OSC7_FREQ));
	DIAL7_AMP = ((Dial*)WidgetManager().GetWidgetPointer(DIAL_OSC7_AMP));
	DIAL8_FREQ = ((Dial*)WidgetManager().GetWidgetPointer(DIAL_OSC8_FREQ));
	DIAL8_AMP = ((Dial*)WidgetManager().GetWidgetPointer(DIAL_OSC8_AMP));

	DIALP_LOWBOOST = ((Dial*)WidgetManager().GetWidgetPointer(DIAL_LOWBOOST));
	DIALP_LOWBOOSTFREQ = ((Dial*)WidgetManager().GetWidgetPointer(DIAL_LOWBOOSTFREQ));

	DIALP_HIGHBOOST = ((Dial*)WidgetManager().GetWidgetPointer(DIAL_HIGHBOOST));
	DIALP_HIGHBOOSTFREQ = ((Dial*)WidgetManager().GetWidgetPointer(DIAL_HIGHBOOSTFREQ));

	DIALP_BITCRUSH = ((Dial*)WidgetManager().GetWidgetPointer(DIAL_BITCRUSH));
	DIALP_BITREFLECT = ((Dial*)WidgetManager().GetWidgetPointer(DIAL_BITREFLECT));

	DIALP_LOWPASS_CUTOFF = ((Dial*)WidgetManager().GetWidgetPointer(DIAL_LOWPASS_CUTOFF));
	DIALP_LOWPASS_RESONANCE = ((Dial*)WidgetManager().GetWidgetPointer(DIAL_LOWPASS_RESONANCE));

	DIALP_HIGHPASS_CUTOFF = ((Dial*)WidgetManager().GetWidgetPointer(DIAL_HIGHPASS_CUTOFF));
	DIALP_HIGHPASS_RESONANCE = ((Dial*)WidgetManager().GetWidgetPointer(DIAL_HIGHPASS_RESONANCE));

	DIALP_FREQ_OFFSET = ((Dial*)WidgetManager().GetWidgetPointer(DIAL_FREQ_OFFSET));
	DIALP_AMP_OFFSET = ((Dial*)WidgetManager().GetWidgetPointer(DIAL_AMP_OFFSET));

	GFXBOX1 = ((GfxBox*)WidgetManager().GetWidgetPointer(GFXBOX_OSC1));
	GFXBOX2 = ((GfxBox*)WidgetManager().GetWidgetPointer(GFXBOX_OSC2));
	GFXBOX3 = ((GfxBox*)WidgetManager().GetWidgetPointer(GFXBOX_OSC3));
	GFXBOX4 = ((GfxBox*)WidgetManager().GetWidgetPointer(GFXBOX_OSC4));
	GFXBOX5 = ((GfxBox*)WidgetManager().GetWidgetPointer(GFXBOX_OSC5));
	GFXBOX6 = ((GfxBox*)WidgetManager().GetWidgetPointer(GFXBOX_OSC6));
	GFXBOX7 = ((GfxBox*)WidgetManager().GetWidgetPointer(GFXBOX_OSC7));
	GFXBOX8 = ((GfxBox*)WidgetManager().GetWidgetPointer(GFXBOX_OSC8));
}

//Synth an oscillator
void SynthEngine::SynthOsc(int oscid, int shape, float freq, float amp, //this oscillator
							float* input1, bool fm1, bool am1, char math1, //horisontal oscillator
							float* input2, bool fm2, bool am2, char math2, //vertical oscillator
							float* out) 
{
	//Frequency Offset
	m_osc.SetPhase(oscid, m_oscdata[oscid].FREQ_OFFSET);

	//Synth Sample
	for(int i = 0; i < m_fsamplesize[oscid]; i++)
	{
		//Shape: None
		if(shape == 0)
		{
			if(input1 == 0)
				out[i] = 0.f;
			else
				out[i] = input1[i];
		}
		else
		{
			float frequency = freq, amplitude = amp;

			//Calculate FM & AM modulation of input 1
			if(input1 != 0)
			{
				if(fm1 == true)
					frequency += input1[i];

				if(am1 == true)
					amplitude += input1[i];
			}

			//Calculate FM & AM modulation of input 2
			if(input2 != 0)
			{
				if(fm2 == true)
					frequency += input2[i];

				if(am2 == true)
					amplitude += input2[i];
			}

			//Calculate Output
			float output = 0.f;

			if(shape == 1)
				output = m_osc.GetSine(oscid, frequency, amplitude);
			else
			if(shape == 2)
				output = m_osc.GetSquare(oscid, frequency, amplitude);
			else
			if(shape == 3)
				output = m_osc.GetSawtooth(oscid, frequency, amplitude);
			else
			if(shape == 4)
				output = m_osc.GetTriangle(oscid, frequency, amplitude);
			else
			if(shape == 5)
				output = m_osc.GetAASine(oscid, frequency, amplitude);
			else
			if(shape == 6)
				output = m_osc.GetAASquare(oscid, frequency, amplitude);
			else
			if(shape == 7)
				output = m_osc.GetAASawtooth(oscid, frequency, amplitude);
			else
			if(shape == 8)
				output = m_osc.GetAATriangle(oscid, frequency, amplitude);
			else
			if(shape == 9)
				output = m_osc.GetLFOSquare(oscid, frequency, amplitude);
			else
			if(shape == 10)
				output = m_osc.GetLFOSawtooth(oscid, frequency, amplitude);
			else
			if(shape == 11)
				output = m_osc.GetLFOTriangle(oscid, frequency, amplitude);
			else
			if(shape == 12)
				output = m_osc.GetLFOTap(oscid, frequency, amplitude);
			else
			if(shape == 13)
				output = m_osc.GetLFOSlide(oscid, frequency, amplitude);
			else
			if(shape == 14)
				output = m_osc.GetLFOImpulse(oscid, frequency, amplitude);
			else
			if(shape == 15)
				output = m_osc.GetLFOBipulse(oscid, frequency, amplitude);
			else
			if(shape == 16)
				output = m_osc.GetLFORandom1(oscid, frequency, amplitude);
			else
			if(shape == 17)
				output = m_osc.GetLFORandom2(oscid, frequency, amplitude);
			else
			if(shape == 18)
				output = m_osc.GetLFORandom3(oscid, frequency, amplitude);
			else
			if(shape == 19)
				output = m_osc.GetLFORandom4(oscid, frequency, amplitude);
			else
			if(shape == 20)
				output = m_osc.GetLFOWhiteNoise(oscid, amplitude);
			else
			if(shape == 21)
				output = m_osc.GetLFOCubicNoise(oscid, frequency, amplitude);
			else
			if(shape == 22)
				output = m_osc.GetLFOLinearNoise(oscid, frequency, amplitude);
			else
			if(shape == 23)
				output = m_osc.GetLFOCosineNoise(oscid, frequency, amplitude);

			//Input1
			if(math1 == 1) //Add
				output += input1[i];
			else
			if(math1 == 2) //Subtract
				output -= input1[i];
			else
			if(math1 == 3) //Times
				output *= input1[i];

			//Input2
			if(math2 == 1) //Add
				output += input2[i];
			else
			if(math2 == 2) //Subtract
				output -= input2[i];
			else
			if(math2 == 3) //Times
				output *= input1[i];

			//Amplitude Offset
			output -= m_oscdata[oscid].AMP_OFFSET;
			
			//Apply Lowpass Filter
			if(m_oscdata[oscid].LOWPASS_CUTOFF != 0.f && m_oscdata[oscid].LOWPASS_RESONANCE != 0)
				output = m_filters[oscid].DoLowPass(output);

			//Apply Highpass Filter
			if(m_oscdata[oscid].HIGHPASS_CUTOFF != 0.f && m_oscdata[oscid].HIGHPASS_RESONANCE != 0)
				output = m_filters[oscid].DoHighPass(output);

			//Apply LowBoost Filter
			if(m_oscdata[oscid].LOWBOOST != 0.f && m_oscdata[oscid].LOWBOOST_FREQ != 0.f)
				output = m_filters[oscid].DoLowBoost(output);

			//Apply High Boost
			if(m_oscdata[oscid].HIGHBOOST != 0.f && m_oscdata[oscid].HIGHBOOST_FREQ != 0.f)
				output = m_filters[oscid].DoHighBoost(output);

			//Apply ADSR
			if(m_oscdata[oscid].ADSR_TYPE == 1)
			{
				output = m_adsr[oscid].CalcADSRSmooth(output, i);
			}
			else if(m_oscdata[oscid].ADSR_TYPE == 2)
			{
					output = m_adsr[oscid].CalcADSRLinear(output, i);
			}

			/*
				Bit Crush has to be applied last for interpolation to
				work with filters unless filters are calculated on a second loop
				after sample is produced.
			*/

			//Apply BitCrusher
			if(m_oscdata[oscid].BITCRUSH != 0)
				output = m_bitcrusher[oscid].DoBitCrush(output, out, i);

			//Return output
			out[i] = output;
		}
	}

	//Finally apply bit reflector
	if(m_oscdata[oscid].BITREFLECT != 0)
		m_bitreflecter[oscid].DoBitReflect(m_fsample[oscid], m_fsamplesize[oscid]);
}

__forceinline int GetMathType(Button* b)
{
	return b->GetUnpressedGraphic() == BMP_ADD ? MATH_ADD : b->GetUnpressedGraphic() == BMP_SUB ? MATH_SUB : b->GetUnpressedGraphic() == BMP_TIMES ? MATH_TIMES : MATH_NONE;
}

//Generate the combined amp of all oscillators
void SynthEngine::Synth()
{
	//Vertical 1
	SynthOsc(OSC8, GFXBOX5->GetValue(), DIAL5_FREQ->GetValue(), DIAL5_AMP->GetValue(),
		0,
		false,
		false,
		MATH_NONE,
		0,
		false,
		false,
		MATH_NONE,
		m_fsample[OSC8]
		);

	//Horizontal 1
	SynthOsc(OSC4, GFXBOX1->GetValue(), DIAL1_FREQ->GetValue(), DIAL1_AMP->GetValue(),
		0,
		false,
		false,
		MATH_NONE,
		m_fsample[OSC8],
		FM4->GetUnpressedGraphic() == 0 ? false : true,
		AM4->GetUnpressedGraphic() == 0 ? false : true,
		GetMathType(ADD4),
		m_fsample[OSC4]
		);

	//Vertical 2
	SynthOsc(OSC7, GFXBOX6->GetValue(), DIAL6_FREQ->GetValue(), DIAL6_AMP->GetValue(),
		m_fsample[OSC8],
		FM10->GetUnpressedGraphic() == 0 ? false : true,
		AM10->GetUnpressedGraphic() == 0 ? false : true,
		GetMathType(ADD10),
		0,
		false,
		false,
		MATH_NONE,
		m_fsample[OSC7]
		);

	//Horizontal 2
	SynthOsc(OSC3, GFXBOX2->GetValue(), DIAL2_FREQ->GetValue(), DIAL2_AMP->GetValue(),
		m_fsample[OSC4],
		FM1->GetUnpressedGraphic() == 0 ? false : true,
		AM1->GetUnpressedGraphic() == 0 ? false : true,
		GetMathType(ADD1),
		m_fsample[OSC7],
		FM5->GetUnpressedGraphic() == 0 ? false : true,
		AM5->GetUnpressedGraphic() == 0 ? false : true,
		GetMathType(ADD5),
		m_fsample[OSC3]
		);

	//Vertical 3
	SynthOsc(OSC6, GFXBOX7->GetValue(), DIAL7_FREQ->GetValue(), DIAL7_AMP->GetValue(),
		m_fsample[OSC7],
		FM9->GetUnpressedGraphic() == 0 ? false : true,
		AM9->GetUnpressedGraphic() == 0 ? false : true,
		GetMathType(ADD9),
		0,
		false,
		false,
		MATH_NONE,
		m_fsample[OSC6]
		);

	//Horizontal 3
	SynthOsc(OSC2, GFXBOX3->GetValue(), DIAL3_FREQ->GetValue(), DIAL3_AMP->GetValue(),
		m_fsample[OSC3],
		FM2->GetUnpressedGraphic() == 0 ? false : true,
		AM2->GetUnpressedGraphic() == 0 ? false : true,
		GetMathType(ADD2),
		m_fsample[OSC6],
		FM6->GetUnpressedGraphic() == 0 ? false : true,
		AM6->GetUnpressedGraphic() == 0 ? false : true,
		GetMathType(ADD6),
		m_fsample[OSC2]
		);

	//Vertical 4
	SynthOsc(OSC5, GFXBOX8->GetValue(), DIAL8_FREQ->GetValue(), DIAL8_AMP->GetValue(),
		m_fsample[OSC6],
		FM8->GetUnpressedGraphic() == 0 ? false : true,
		AM8->GetUnpressedGraphic() == 0 ? false : true,
		GetMathType(ADD8),
		0,
		false,
		false,
		MATH_NONE,
		m_fsample[OSC5]
		);

	//Horizontal 4
	SynthOsc(OSC1, GFXBOX4->GetValue(), DIAL4_FREQ->GetValue(), DIAL4_AMP->GetValue(),
		m_fsample[OSC2],
		FM3->GetUnpressedGraphic() == 0 ? false : true,
		AM3->GetUnpressedGraphic() == 0 ? false : true,
		GetMathType(ADD3),
		m_fsample[OSC5],
		FM7->GetUnpressedGraphic() == 0 ? false : true,
		AM7->GetUnpressedGraphic() == 0 ? false : true,
		GetMathType(ADD7),
		m_fsample[OSC1]
		);
}

//Show filter info on dialog
void SynthEngine::ShowSelectedOscillator()
{
	DIALP_A->SetValue(m_oscdata[m_selected].A);
	DIALP_D->SetValue(m_oscdata[m_selected].D);
	DIALP_S->SetValue(m_oscdata[m_selected].S);
	DIALP_SL->SetValue(m_oscdata[m_selected].SL);
	DIALP_R->SetValue(m_oscdata[m_selected].R);

	if(m_oscdata[m_selected].ADSR_TYPE == 0)
	{
		SMOOTH->SetUnpressedGraphic(0);
		SHARP->SetUnpressedGraphic(0);
	}

	if(m_oscdata[m_selected].ADSR_TYPE == 1)
	{
		SMOOTH->SetUnpressedGraphic(BMP_SMOOTH);
		SHARP->SetUnpressedGraphic(0);
	}

	if(m_oscdata[m_selected].ADSR_TYPE == 2)
	{
		SMOOTH->SetUnpressedGraphic(0);
		SHARP->SetUnpressedGraphic(BMP_SHARP);
	}

	DIALP_LOWPASS_CUTOFF->SetValue(m_oscdata[m_selected].LOWPASS_CUTOFF);
	DIALP_LOWPASS_RESONANCE->SetValue(m_oscdata[m_selected].LOWPASS_RESONANCE);

	DIALP_HIGHPASS_CUTOFF->SetValue(m_oscdata[m_selected].HIGHPASS_CUTOFF);
	DIALP_HIGHPASS_RESONANCE->SetValue(m_oscdata[m_selected].HIGHPASS_RESONANCE);

	DIALP_LOWBOOSTFREQ->SetValue(m_oscdata[m_selected].LOWBOOST_FREQ);
	DIALP_LOWBOOST->SetValue(m_oscdata[m_selected].LOWBOOST);

	DIALP_HIGHBOOSTFREQ->SetValue(m_oscdata[m_selected].HIGHBOOST_FREQ);
	DIALP_HIGHBOOST->SetValue(m_oscdata[m_selected].HIGHBOOST);

	DIALP_FREQ_OFFSET->SetValue(m_oscdata[m_selected].FREQ_OFFSET);
	DIALP_AMP_OFFSET->SetValue(m_oscdata[m_selected].AMP_OFFSET);

	DIALP_BITCRUSH->SetValue(m_oscdata[m_selected].BITCRUSH);

	if(m_oscdata[m_selected].BITCRUSH_TYPE == 0)
	{
		BCSMOOTH->SetUnpressedGraphic(0);
		BCSHARP->SetUnpressedGraphic(0);
	}

	if(m_oscdata[m_selected].BITCRUSH_TYPE == 1)
	{
		BCSMOOTH->SetUnpressedGraphic(BMP_SMOOTH);
		BCSHARP->SetUnpressedGraphic(0);
	}

	if(m_oscdata[m_selected].BITCRUSH_TYPE == 2)
	{
		BCSMOOTH->SetUnpressedGraphic(0);
		BCSHARP->SetUnpressedGraphic(BMP_SHARP);
	}

	DIALP_BITREFLECT->SetValue(m_oscdata[m_selected].BITREFLECT);

	if(m_oscdata[m_selected].BITREFLECT_TYPE == 0)
	{
		BRSMOOTH->SetUnpressedGraphic(0);
		BRSHARP->SetUnpressedGraphic(0);
	}

	if(m_oscdata[m_selected].BITREFLECT_TYPE == 1)
	{
		BRSMOOTH->SetUnpressedGraphic(BMP_SMOOTH);
		BRSHARP->SetUnpressedGraphic(0);
	}

	if(m_oscdata[m_selected].BITREFLECT_TYPE == 2)
	{
		BRSMOOTH->SetUnpressedGraphic(0);
		BRSHARP->SetUnpressedGraphic(BMP_SHARP);
	}
}

//Update Filter info for Selected Oscillator
void SynthEngine::UpdateSelectedOscillator()
{
	//Set Dial Values
	m_oscdata[m_selected].A = DIALP_A->GetValue();
	m_oscdata[m_selected].D = DIALP_D->GetValue();
	m_oscdata[m_selected].SL = DIALP_SL->GetValue();
	m_oscdata[m_selected].S = DIALP_S->GetValue();
	m_oscdata[m_selected].R = DIALP_R->GetValue();
	m_oscdata[m_selected].ADSR_TYPE = SMOOTH->GetUnpressedGraphic() == BMP_SMOOTH ? 1 : SHARP->GetUnpressedGraphic() == BMP_SHARP ? 2 : 0;

	m_oscdata[m_selected].LOWBOOST = DIALP_LOWBOOST->GetValue();
	m_oscdata[m_selected].LOWBOOST_FREQ = DIALP_LOWBOOSTFREQ->GetValue();

	m_oscdata[m_selected].HIGHBOOST = DIALP_HIGHBOOST->GetValue();
	m_oscdata[m_selected].HIGHBOOST_FREQ = DIALP_HIGHBOOSTFREQ->GetValue();

	m_oscdata[m_selected].LOWPASS_CUTOFF = DIALP_LOWPASS_CUTOFF->GetValue();
	m_oscdata[m_selected].LOWPASS_RESONANCE = DIALP_LOWPASS_RESONANCE->GetValue();

	m_oscdata[m_selected].HIGHPASS_CUTOFF = DIALP_HIGHPASS_CUTOFF->GetValue();
	m_oscdata[m_selected].HIGHPASS_RESONANCE = DIALP_HIGHPASS_RESONANCE->GetValue();

	m_oscdata[m_selected].FREQ_OFFSET = DIALP_FREQ_OFFSET->GetValue();
	m_oscdata[m_selected].AMP_OFFSET = DIALP_AMP_OFFSET->GetValue();

	m_oscdata[m_selected].BITCRUSH = DIALP_BITCRUSH->GetValue();
	m_oscdata[m_selected].BITCRUSH_TYPE = BCSMOOTH->GetUnpressedGraphic() == BMP_SMOOTH ? 1 : BCSHARP->GetUnpressedGraphic() == BMP_SHARP ? 2 : 0;

	m_oscdata[m_selected].BITREFLECT = DIALP_BITREFLECT->GetValue();
	m_oscdata[m_selected].BITREFLECT_TYPE = BRSMOOTH->GetUnpressedGraphic() == BMP_SMOOTH ? 1 : BRSHARP->GetUnpressedGraphic() == BMP_SHARP ? 2 : 0;
}


//Synthesize Sample
void SynthEngine::CreateSample()
{
	//Is audio currently playing?
	if(IsAudioPlaying() == false)
	{
		//Delete current samples
		if(m_sample != NULL)
			delete [] m_sample;

		for(int i = 0; i < 8; ++i)
			if(m_fsample[i] != NULL)
				delete [] m_fsample[i];

		//Sample is the length of oscillator 1's ADSR
		const float seconds = m_oscdata[0].A + m_oscdata[0].D + m_oscdata[0].S + m_oscdata[0].R;

		//Allocate Samples
		m_samplesize = m_samplerate * (seconds * 2.f);

		//Allocate Each Oscillator Sample
		for(int i = 0; i < 8; ++i)
		{
			m_fsample[i] = new float[m_samplesize];
			m_fsamplesize[i] = m_samplesize;
		}

		//Init out sample
		m_sample = new char[m_samplesize];
	}

	//Null each sample
	for(int i = 0; i < 8; ++i)
		for(int i2 = 0; i2 < m_fsamplesize[i]; ++i2)
			m_fsample[i][i2] = 0.f;

	//Reset Oscillator Phases
	for(int i = 0; i < 8; i++)
		m_osc.SetPhase(i, 0.f);

	//Init all oscillator filters
	for(int i = 0; i < 8; ++i)
	{
		//Init ADSR
		if(m_oscdata[i].ADSR_TYPE == 1 || m_oscdata[i].ADSR_TYPE == 2)
			m_adsr[i].SetADSR(m_oscdata[i].A, m_oscdata[i].D, m_oscdata[i].S, m_oscdata[i].SL, m_oscdata[i].R, m_samplerate);

		//Init Low Boost
		if(m_oscdata[i].LOWBOOST != 0.f && m_oscdata[i].LOWBOOST_FREQ != 0.f)
			m_filters[i].SetLowBoost(m_oscdata[i].LOWBOOST_FREQ, m_oscdata[i].LOWBOOST);

		//Init High Boost
		if(m_oscdata[i].HIGHBOOST != 0.f && m_oscdata[i].HIGHBOOST_FREQ != 0.f)
			m_filters[i].SetHighBoost(m_oscdata[i].HIGHBOOST_FREQ, m_oscdata[i].HIGHBOOST);

		//Init Lowpass
		if(m_oscdata[i].LOWPASS_CUTOFF != 0.f && m_oscdata[i].LOWPASS_RESONANCE != 0)
			m_filters[i].SetLowPass(m_oscdata[i].LOWPASS_CUTOFF, m_oscdata[i].LOWPASS_RESONANCE);

		//Init Highpass
		if(m_oscdata[i].HIGHPASS_CUTOFF != 0.f && m_oscdata[i].HIGHPASS_RESONANCE != 0)
			m_filters[i].SetHighPass(m_oscdata[i].HIGHPASS_CUTOFF, m_oscdata[i].HIGHPASS_RESONANCE);

		//Init BitCrush
		if(m_oscdata[i].BITCRUSH != 0)
			m_bitcrusher[i].SetBitCrush(m_oscdata[i].BITCRUSH, m_oscdata[i].BITCRUSH_TYPE);

		//Init bitreflecter
		if(DIALP_BITREFLECT->GetValue() != 0.f)
			m_bitreflecter[i].SetBitReflect(m_oscdata[i].BITREFLECT, m_oscdata[i].BITREFLECT_TYPE);
	}

	//Synth Samples
	Synth();

	//Convert float sample to char sample
	for(int i = 0; i < m_samplesize; i++)
		m_sample[i] = lrintf(ClampClip(m_fsample[0][i]));
}

//Write the sample out to a WAV file
void SynthEngine::WriteSampleToWAV(char* filename)
{
	const unsigned int wavedata_size = (37+m_samplesize)+2; //(44+sample_size+1)-8
	const unsigned int subchunk = 16;
	const unsigned short audioformat = 1;
	const unsigned short channels = 1;
	const unsigned int samplerate = 44100;
	const unsigned short bitspersample = 16;
	const unsigned int byterate = samplerate * channels * bitspersample / 8;
	const unsigned short blockalignment = channels * bitspersample / 8;

	FILE* wav = fopen(filename, "wb");
	if(wav != NULL)
	{
		fwrite("RIFF", 4, 1, wav);
		fwrite(&wavedata_size, 1, 4, wav);
		fwrite("WAVE", 4, 1, wav);
		fwrite("fmt ", 4, 1, wav);
		fwrite(&subchunk, 1, 4, wav);
		fwrite(&audioformat, 1, 2, wav);
		fwrite(&channels, 1, 2, wav);
		fwrite(&samplerate, 1, 4, wav);
		fwrite(&byterate, 1, 4, wav);
		fwrite(&blockalignment, 1, 2, wav);
		fwrite(&bitspersample, 1, 2, wav);
		fwrite("data", 4, 1, wav);
		fwrite(&m_samplesize+1, 1, 4, wav);
		fwrite(m_sample, 1, m_samplesize-1, wav);
		fclose(wav);
	}
}

//Write the sample out to a PCM file
void SynthEngine::WriteSampleToPCM(char* filename)
{
	FILE* wav = fopen(filename, "wb");
	if(wav != NULL)
	{
		fwrite(m_sample, 1, m_samplesize-1, wav);
		fclose(wav);
	}
}

//Save all 8 oscillator states
void SynthEngine::SaveState(FILE* out)
{
	for(int i = 0; i < 8; ++i)
		fwrite(&m_oscdata[i], 1, sizeof(m_oscdata[i]), out);

	fwrite(&m_selected, 1, sizeof(m_selected), out);
}

//Load all 8 oscillator states
void SynthEngine::LoadState(FILE* out)
{
	for(int i = 0; i < 8; ++i)
		fread(&m_oscdata[i], 1, sizeof(m_oscdata[i]), out);

	fread(&m_selected, 1, sizeof(m_selected), out);
}






