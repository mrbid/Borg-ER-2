/*
	Code written by James Fletcher (~2010).
	https://github.com/mrbid
*/

#include "oscillator.h"
#include <windows.h>


//Returns specified point in Square Wave
float osc::GetSquarePoint(int point, float freq, float amplitude, int iterations)
{
    float yr = GetSinePoint(point, freq, amplitude);
     
    for(int h = 3; h < iterations; h+=2)
    {
         yr += GetSinePoint(point, freq*h, amplitude/h);
    }
     
    return yr;
}

//Returns specified point in Sawtooth Wave
float osc::GetSawtoothPoint(int point, float freq, float amplitude, int iterations)
{
	float yr = GetSinePoint(point, freq, amplitude);

	for(int h = 2; h < iterations; h++)
	{
		yr += GetSinePoint(point, freq*h, amplitude/h);
	}

	return yr;
}

//Returns specified point in Triangle Wave
float osc::GetTrianglePoint(int point, float freq, float amplitude, int iterations)
{
	float yr = GetSinePoint(point, freq, amplitude);
	bool eo = 0; //everyother

	for(int h = 3; h < iterations; h+=2)
	{
		if(eo == 1){yr += GetSinePoint(point, freq*h, amplitude/(h*h));} 
		if(eo == 0){yr -= GetSinePoint(point, freq*h, amplitude/(h*h));} 
		eo = !eo;
	}

	return yr;
}

//Generate Loopup Tables
void Oscillator::Init(int numosc, int sr, int i_sqr, int i_saw, int i_tri)
{
	//Check if already initialized
	if(m_phase != NULL)
	{
		#ifdef _DEBUG
		MessageBox(0, "Oscillator is already initialized.", "Error", MB_OK | MB_TOPMOST);
		#endif

		return;
	}

	//Random seed for white noise
	srand(GetTickCount());

	//Set Sample Rate
	m_rsamplerate = 1.0f/sr;

	//Setup phases for each oscillator
	m_phase = new float[numosc];

	//Set all to 0.f
	for(int i = 0; i < numosc; i++)
		m_phase[i] = 0.f;


	// ************************
	// Create Normal Lookup Tables
	// ************************
    
	//Create Sine Lookup-Table
	for(int i = 0; i < TABLE_SIZE; i++){m_sintable[i] = sin(i*x2PI/TABLE_SIZE_FLOAT);}
    
	//Create Square Lookup-Table
	for(int i = 0; i < TABLE_SIZE; i++){m_sqrtable[i] = osc::GetSquarePoint(i, x2PI/TABLE_SIZE_FLOAT, 1.0f, 10);}
    
	//Create Sawtooth Lookup-Table
	for(int i = 0; i < TABLE_SIZE; i++){m_sawtable[i] = osc::GetSawtoothPoint(i, x2PI/TABLE_SIZE_FLOAT, 1.0f, 10);}
    
	//Create Triangle Lookup-Table
	for(int i = 0; i < TABLE_SIZE; i++){m_tritable[i] = osc::GetTrianglePoint(i, x2PI/TABLE_SIZE_FLOAT, 1.0f, 5);}


	// ************************
	// Create AA Lookup Tables
	// ************************

	float tt = 0.f;
        
	//Create Sine Lookup-Table
	for(int i = 0; i < TABLE_SIZEAA; i++)
	{
		m_aa_sintable[i].amplitude = sin(i*x2PI/TABLE_SIZEAA_FLOAT);
		m_aa_sintable[i].time = tt;
		tt += TABLE_SIZEAA_FLOAT_RECP;
	}
		
	tt = 0.f;
        
	//Create Square Lookup-Table
	for(int i = 0; i < TABLE_SIZEAA; i++)
	{
		m_aa_sqrtable[i].amplitude = osc::GetSquarePoint(i, x2PI/TABLE_SIZEAA_FLOAT, 1.0f, i_sqr);
		m_aa_sqrtable[i].time = tt;
		tt += TABLE_SIZEAA_FLOAT_RECP;
	}
		
	tt = 0.f;
        
	//Create Sawtooth Lookup-Table
	for(int i = 0; i < TABLE_SIZEAA; i++)
	{
		m_aa_sawtable[i].amplitude = osc::GetSawtoothPoint(i, x2PI/TABLE_SIZEAA_FLOAT, 1.0f, i_saw);
		m_aa_sawtable[i].time = tt;
		tt += TABLE_SIZEAA_FLOAT_RECP;
	}
		
	tt = 0.f;
        
	//Create Triangle Lookup-Table
	for(int i = 0; i < TABLE_SIZEAA; i++)
	{
		m_aa_tritable[i].amplitude = osc::GetTrianglePoint(i, x2PI/TABLE_SIZEAA_FLOAT, 1.0f, i_tri);
		m_aa_tritable[i].time = tt;
		tt += TABLE_SIZEAA_FLOAT_RECP;
	}


	// ************************
	// Create LFO Lookup Tables
	// ************************
        
	m_lfo_sqrtable[0].amplitude = -1.0f;
	m_lfo_sqrtable[0].time = 0.0f;
	m_lfo_sqrtable[1].amplitude = -1.0f;
	m_lfo_sqrtable[1].time = 0.5f;
	m_lfo_sqrtable[2].amplitude = 1.0f;
	m_lfo_sqrtable[2].time = 0.5f;
	m_lfo_sqrtable[3].amplitude = 1.0f;
	m_lfo_sqrtable[3].time = 1.0f;
        
	m_lfo_sawtable[0].amplitude = 1.0f;
	m_lfo_sawtable[0].time = 0.0f;
	m_lfo_sawtable[1].amplitude = -1.0f;
	m_lfo_sawtable[1].time = 1.0f;

	m_lfo_imptable[0].amplitude = 0.0f;
	m_lfo_imptable[0].time = 0.0f;
	m_lfo_imptable[1].amplitude = 0.0f;
	m_lfo_imptable[1].time = 0.75f;
	m_lfo_imptable[2].amplitude = -1.0f;
	m_lfo_imptable[2].time = 0.75f;
	m_lfo_imptable[3].amplitude = -1.0f;
	m_lfo_imptable[3].time = 0.76f;
	m_lfo_imptable[4].amplitude = 0.0f;
	m_lfo_imptable[4].time = 0.76f;
	m_lfo_imptable[5].amplitude = 0.0f;
	m_lfo_imptable[5].time = 1.0f;
        
	m_lfo_tritable[0].amplitude = -1.0f;
	m_lfo_tritable[0].time = 0.0f;
	m_lfo_tritable[1].amplitude = 1.0f;
	m_lfo_tritable[1].time = 0.5f;
	m_lfo_tritable[2].amplitude = -1.0f;
	m_lfo_tritable[2].time = 1.0f;
        
	m_lfo_slitable[0].amplitude = 0.0f;
	m_lfo_slitable[0].time = 0.0f;
	m_lfo_slitable[1].amplitude = 4.0f;
	m_lfo_slitable[1].time = 1.0f;
        
	m_lfo_rndtable[0].amplitude = -1.0f;
	m_lfo_rndtable[0].time = 0.0f;
	m_lfo_rndtable[1].amplitude = -1.0f;
	m_lfo_rndtable[1].time = 0.5f;
	m_lfo_rndtable[2].amplitude = 1.0f;
	m_lfo_rndtable[2].time = 0.5f;
	m_lfo_rndtable[3].amplitude = 1.0f;
	m_lfo_rndtable[3].time = 1.0f;
    
	m_lfo_taptable[0].amplitude = -0.1f;
	m_lfo_taptable[0].time = 0.0f;
	m_lfo_taptable[1].amplitude = 0.1f;
	m_lfo_taptable[1].time = 0.125f;
	m_lfo_taptable[2].amplitude = -0.7f;
	m_lfo_taptable[2].time = 0.250f;
	m_lfo_taptable[3].amplitude = 0.7f;
	m_lfo_taptable[3].time = 0.375f;
	m_lfo_taptable[4].amplitude = -0.5f;
	m_lfo_taptable[4].time = 0.500f;
	m_lfo_taptable[5].amplitude = 0.5f;
	m_lfo_taptable[5].time = 0.625f;
	m_lfo_taptable[6].amplitude = -0.3f;
	m_lfo_taptable[6].time = 0.750f;
	m_lfo_taptable[7].amplitude = 0.3f;
	m_lfo_taptable[7].time = 0.875f;
	m_lfo_taptable[8].amplitude = -0.1f;
	m_lfo_taptable[8].time = 1.0f;

	m_lfo_biplstable[0].amplitude = 0.0f;
	m_lfo_biplstable[0].time = 0.0f;
	m_lfo_biplstable[1].amplitude = 0.0f;
	m_lfo_biplstable[1].time = 0.34f;
	m_lfo_biplstable[2].amplitude = -1.0f;
	m_lfo_biplstable[2].time = 0.34f;
	m_lfo_biplstable[3].amplitude = -1.0f;
	m_lfo_biplstable[3].time = 0.51f;
	m_lfo_biplstable[4].amplitude = 1.0f;
	m_lfo_biplstable[4].time = 0.51f;
	m_lfo_biplstable[5].amplitude = 1.0f;
	m_lfo_biplstable[5].time = 0.68f;
	m_lfo_biplstable[6].amplitude = 0.0f;
	m_lfo_biplstable[6].time = 0.68f;
	m_lfo_biplstable[7].amplitude = 0.0f;
	m_lfo_biplstable[7].time = 1.0f;
    
	m_lfo_sntable[0].amplitude = osc::Noise(rand());
	m_lfo_sntable[0].time = 0.0f;
	m_lfo_sntable[1].amplitude = osc::Noise(rand());
	m_lfo_sntable[1].time = 1.0f;
    
	m_lfo_slide2.Set(0.f, 0.3f, 0.5f, 1.0f);
	m_bez.Set(0.f, osc::Noise(0), osc::Noise(0), 0.f);
}

//Destructor
Oscillator::Oscillator()
{
	if(m_phase != NULL)
		delete [] m_phase;
}

//Generate Random Wave
float Oscillator::GetLFORandom1(int oscid, float frequency, float amplitude)
{
    m_phase[oscid] += frequency*m_rsamplerate;
    
    while(m_phase[oscid] < 0.f){m_phase[oscid] = 1.0f;}
	while(m_phase[oscid] >= 1.0f)
    {
        m_lfo_rndtable[0].amplitude = osc::Noise(m_phase[oscid]);
        m_lfo_rndtable[0].time = 0.0f;
        m_lfo_rndtable[1].amplitude = osc::Noise(m_phase[oscid]);
        m_lfo_rndtable[1].time = 0.5f;
        m_lfo_rndtable[2].amplitude = osc::Noise(m_phase[oscid]);
        m_lfo_rndtable[2].time = 0.5f;
        m_lfo_rndtable[3].amplitude = osc::Noise(m_phase[oscid]);
        m_lfo_rndtable[3].time = 1.0f;
        m_phase[oscid] = 0.f;
    }
        
    if(m_phase[oscid] >= m_lfo_rndtable[0].time && m_phase[oscid] < m_lfo_rndtable[1].time)
    {
        return amplitude * osc::LFOLerp(&m_lfo_rndtable[0], &m_lfo_rndtable[1], 2.f, m_phase[oscid]);
    }
    
    if(m_phase[oscid] >= m_lfo_rndtable[2].time && m_phase[oscid] < m_lfo_rndtable[3].time+0.5f)
    {
        return amplitude * osc::LFOLerp(&m_lfo_rndtable[2], &m_lfo_rndtable[3], 2.f, m_phase[oscid]);
    }
}

//Generate Random Wave
float Oscillator::GetLFORandom2(int oscid, float frequency, float amplitude)
{
    m_phase[oscid] += frequency*m_rsamplerate;
    
    while(m_phase[oscid] < 0.f){m_phase[oscid] = 1.0f;}
	while(m_phase[oscid] >= 1.0f)
    {
        const float r1 = osc::Noise(m_phase[oscid]);
        const float r2 = osc::Noise(m_phase[oscid]);
        m_lfo_rndtable[0].amplitude = r1;
        m_lfo_rndtable[0].time = 0.0f;
        m_lfo_rndtable[1].amplitude = r1;
        m_lfo_rndtable[1].time = 0.5f;
        m_lfo_rndtable[2].amplitude = r2;
        m_lfo_rndtable[2].time = 0.5f;
        m_lfo_rndtable[3].amplitude = r2;
        m_lfo_rndtable[3].time = 1.0f;
        m_phase[oscid] = 0.f;
    }
        
    if(m_phase[oscid] >= m_lfo_rndtable[0].time && m_phase[oscid] < m_lfo_rndtable[1].time)
    {
        return amplitude * osc::LFOLerp(&m_lfo_rndtable[0], &m_lfo_rndtable[1], 2.f, m_phase[oscid]);
    }
    
    if(m_phase[oscid] >= m_lfo_rndtable[2].time && m_phase[oscid] < m_lfo_rndtable[3].time+0.5f)
    {
        return amplitude * osc::LFOLerp(&m_lfo_rndtable[2], &m_lfo_rndtable[3], 2.f, m_phase[oscid]);
    }
}

//Generate Tap Wave
float Oscillator::GetLFOTap(int oscid, float frequency, float amplitude)
{
    float retval = 0.f;
    m_phase[oscid] += frequency*m_rsamplerate;
        
    if(m_phase[oscid] >= m_lfo_taptable[0].time && m_phase[oscid] < m_lfo_taptable[1].time)
    {
        retval = amplitude * osc::LFOLerp(&m_lfo_taptable[0], &m_lfo_taptable[1], 8.f, m_phase[oscid]);
    }
    
    if(m_phase[oscid] >= m_lfo_taptable[1].time && m_phase[oscid] < m_lfo_taptable[2].time)
    {
        retval = amplitude * osc::LFOLerp(&m_lfo_taptable[1], &m_lfo_taptable[2], 8.f, m_phase[oscid]);
    }
    
    if(m_phase[oscid] >= m_lfo_taptable[2].time && m_phase[oscid] < m_lfo_taptable[3].time)
    {
        retval = amplitude * osc::LFOLerp(&m_lfo_taptable[2], &m_lfo_taptable[3], 8.f, m_phase[oscid]);
    }
    
    if(m_phase[oscid] >= m_lfo_taptable[3].time && m_phase[oscid] < m_lfo_taptable[4].time)
    {
        retval = amplitude * osc::LFOLerp(&m_lfo_taptable[3], &m_lfo_taptable[4], 8.f, m_phase[oscid]);
    }
    
    if(m_phase[oscid] >= m_lfo_taptable[4].time && m_phase[oscid] < m_lfo_taptable[5].time)
    {
        retval = amplitude * osc::LFOLerp(&m_lfo_taptable[4], &m_lfo_taptable[5], 8.f, m_phase[oscid]);
    }
    
    if(m_phase[oscid] >= m_lfo_taptable[5].time && m_phase[oscid] < m_lfo_taptable[6].time)
    {
        retval = amplitude * osc::LFOLerp(&m_lfo_taptable[5], &m_lfo_taptable[6], 8.f, m_phase[oscid]);
    }
    
    if(m_phase[oscid] >= m_lfo_taptable[6].time && m_phase[oscid] < m_lfo_taptable[7].time+0.5f)
    {
        retval = amplitude * osc::LFOLerp(&m_lfo_taptable[6], &m_lfo_taptable[7], 8.f, m_phase[oscid]);
    }
    
    if(m_phase[oscid] >= m_lfo_taptable[7].time && m_phase[oscid] < m_lfo_taptable[8].time+0.5f)
    {
        retval = amplitude * osc::LFOLerp(&m_lfo_taptable[7], &m_lfo_taptable[8], 8.f, m_phase[oscid]);
    }
    
    osc::wrap(m_phase[oscid], 1.0f);
    return retval;
}


















