#ifndef MERSENNETWISTER_H
#define MERSENNETWISTER_H


// Uses a slightly modified version of the source from Dave Loeser's article "A Mersenne Twister Class" on codeproject.com
// www.codeproject.com/cpp/mersennetwisterclass.asp


// Project includes
#include <limits>

// Undefine max & min macros to allow numeric_limits<>::max() & min() to work
#undef max
#undef min


// Global constants
static const float fMaxInv = 1.0f/static_cast<float>(std::numeric_limits<unsigned long>::max());
static const double lfMaxInv = 1.0/static_cast<double>(std::numeric_limits<unsigned long>::max());

static const unsigned int K = 0x9908B0DFU, DEFAULT_SEED = 4357;

// Macros
#define hiBit(u) ((u) & 0x80000000U)
#define loBit(u) ((u) & 0x00000001U)
#define loBits(u) ((u) & 0x7FFFFFFFU)
#define mixBits(u, v) (hiBit(u)|loBits(v))


// Random number generator that uses the mersenne twister algorithm
class MersenneTwister
{
public:
	// Default ctor - Seeds generator with current time
	MersenneTwister();
	// Set ctor - Seeds generator with seed
	MersenneTwister(unsigned long _seed);

	// Default dtor - Empty
	~MersenneTwister() {};

	// Seed generator
	void Seed(unsigned long seed);

	// Get unaltered random number
	unsigned long Next();

	// Get random number of type T in range
	template <typename T>
	T Next(const T tMax, const T tMin);
	// Specializations for signed integral & real values
	template <>
	char Next<char>(const char tMin, const char tMax);
	template <>
	unsigned char Next<unsigned char>(const unsigned char tMin, const unsigned char tMax);
	template <>
	short Next<short>(const short tMin, const short tMax);
	template <>
	int Next<int>(const int tMin, const int tMax);
	template <>
	long Next<long>(const long tMin, const long tMax);
	template <>
	float Next<float>(const float tMin, const float tMax);
	template <>
	double Next<double>(const double tMin, const double tMax);

private:
	// uliState vector + 1 extra to not violate ANSI C
	unsigned long uliState[625],
	// uliNext random value is computed from here
				  *uliNext;
	// can *uliNext++ this many times before reloading
	int iLeft;
	// Added so that setting a seed actually maintains that seed when a Reload takes place.
	unsigned int uiSeedValue;
	
	// Reload generator
	unsigned long Reload();
};


// Get random number of type T in range
template <typename T>
inline T MersenneTwister::Next(const T tMax, const T tMin)
{
	return static_cast<T>(Random()%(tMax-tMin))+tMin;
}
// Specializations for signed integral & real values
template <>
inline char MersenneTwister::Next<char>(const char tMin, const char tMax)
{
	return static_cast<char>(Next()%std::numeric_limits<char>::max())%(tMax-tMin)+tMin;
}
template <>
inline unsigned char MersenneTwister::Next<unsigned char>(const unsigned char tMin, const unsigned char tMax)
{
	return static_cast<unsigned char>(Next()%std::numeric_limits<unsigned char>::max())%(tMax-tMin)+tMin;
}
template <>
inline short MersenneTwister::Next<short>(const short tMin, const short tMax)
{
	return static_cast<short>(Next()%std::numeric_limits<short>::max())%(tMax-tMin)+tMin;
}
template <>
inline int MersenneTwister::Next<int>(const int tMin, const int tMax)
{
	return static_cast<int>(Next()%std::numeric_limits<int>::max())%(tMax-tMin)+tMin;
}
template <>
inline long MersenneTwister::Next<long>(const long tMin, const long tMax)
{
	return static_cast<long>(Next()%std::numeric_limits<long>::max())%(tMax-tMin)+tMin;
}
template <>
inline float MersenneTwister::Next<float>(const float tMin, const float tMax)
{
	return (Next()*fMaxInv)*(tMax-tMin)+tMin;
}
template <>
inline double MersenneTwister::Next<double>(const double  tMin, const double tMax)
{
	return (Next()*lfMaxInv)*(tMax-tMin)+tMin;
}


#endif