/*
	Code written by James Fletcher (~2010).
	https://github.com/mrbid

	For the bit crusher to do it's job it needs to know the
	current index sample value and it will return the new
	index sample value. However it will also need a pointer
	to the entire array and the current index position.
*/

#ifndef BITREFLECTER_H
#define BITREFLECTER_H

#include <math.h>

//Bit Crusher Class
class BitReflecter
{
public:
	
	//Set Bit Reflect
	void SetBitReflect(float stretch, char type);

	//Do Bit Reflect
	void DoBitReflect(float *m_fsample, long len);

private:
	
	int m_bc; //Bit reflect index
	float m_ls; //last sample
	float m_rBCV; //Recip of stretch amount
	long m_stretch; //stretch amount (how many samples lost)
	char m_type; //Interpolation? 0 = no

};


__forceinline void BitReflecter::SetBitReflect(float stretch, char type)
{
	m_bc = 0;
	m_ls = 0;
	m_type = type;
	m_stretch = stretch;

	if(m_stretch == 0)
		m_stretch = 1;

	m_rBCV = 1.f / m_stretch;
}

#endif



