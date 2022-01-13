// Project includes
#include "MersenneTwister.h"
#include <cassert>
#include <ctime>


// Default ctor - Seeds generator with current time
MersenneTwister::MersenneTwister() : iLeft(-1)
{
    Seed((unsigned long int)time(NULL));
}
// Set ctor - Seeds generator with seed
MersenneTwister::MersenneTwister(unsigned long _seed) : iLeft(-1), uiSeedValue(_seed)
{
	Seed(uiSeedValue);
}

// Seed generator
void MersenneTwister::Seed(unsigned long seed)
{
	unsigned long x = (seed | 1U) & 0xFFFFFFFFU, *s = uliState;
	int j;

	for (iLeft = 0, *s++ = x, j = 624; --j; *s++ = (x*=69069U) & 0xFFFFFFFFU);
	    uiSeedValue = seed;
}

// Get unaltered random number
unsigned long MersenneTwister::Next()
{
	unsigned long y;

	if (--iLeft < 0)
		return Reload();

	y = *uliNext++;
	y ^= (y >> 11);
	y ^= (y << 7) & 0x9D2C5680U;
	y ^= (y << 15) & 0xEFC60000U;

	return (y ^ (y >> 18));
}

// Reload generator
unsigned long MersenneTwister::Reload()
{
	unsigned long *p0 = uliState,
				  *p2 = uliState+2,
				  *pM = uliState+397,
				  s0, s1;
	int j;

	if (iLeft < -1)
		Seed(uiSeedValue);

	iLeft = 624-1, uliNext = uliState+1;

	for (s0 = uliState[0], s1 = uliState[1], j = 624-397+1; --j; s0 = s1, s1 = *p2++)
		*p0++ = *pM++ ^ (mixBits(s0, s1) >> 1) ^ (loBit(s1) ? K : 0U);

	for (pM = uliState, j = 397; --j; s0 = s1, s1 = *p2++)
		*p0++ = *pM++ ^ (mixBits(s0, s1) >> 1) ^ (loBit(s1) ? K : 0U);

	s1 = uliState[0], *p0 = *pM ^ (mixBits(s0, s1) >> 1) ^ (loBit(s1) ? K : 0U);
	s1 ^= (s1 >> 11);
	s1 ^= (s1 << 7) & 0x9D2C5680U;
	s1 ^= (s1 << 15) & 0xEFC60000U;

	return (s1 ^ (s1 >> 18));
}