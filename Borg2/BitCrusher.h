/*
	Code written by James Fletcher (~2010).
	https://github.com/mrbid

	For the bit crusher to do it's job it needs to know the
	current index sample value and it will return the new
	index sample value. However it will also need a pointer
	to the entire array and the current index position.
*/

#ifndef BITCRUSHER_H
#define BITCRUSHER_H

//Bit Crusher Class
class BitCrusher
{
public:
	
	//Set Bit Crush
	void SetBitCrush(float crush, char type);

	//Do Bitcrush
	float DoBitCrush(float in, float* m_fsample, int i);

private:
	
	int m_bc; //Bit Crush index
	float m_ls; //last sample
	float m_rBCV; //Recip of crush amount
	float m_crush; //crush amount (how many samples lost)
	char m_type; //Interpolation? 0 = no

};

__forceinline void BitCrusher::SetBitCrush(float crush, char type)
{
	m_bc = 0;
	m_ls = 0;
	m_type = type;
	m_crush = crush;
	m_rBCV = 1.f / m_crush;
}

#endif


