#ifndef RANDUTILITIES_H
#define RANDUTILITIES_H



// ------------------------------------------------------------------------------------
// ----------------------------------Included headers----------------------------------
// ------------------------------------------------------------------------------------

// Utilities
#include "..\Utilities\Singleton.h"
#include "..\Utilities\Matrix.h"
#include "..\Utilities\MersenneTwister.h"


// ------------------------------------------------------------------------------------
// ---------------------------------Function prototypes--------------------------------
// ------------------------------------------------------------------------------------

// Wrapper function for access to global random number generator
template<typename T>
inline T Rand( T Min, T Max);

// Generate a random NxM matrix of type T
template<int N, int M, typename T>
Matrix<N, M, T> RandomMatrix( T Min, T Max);


// ------------------------------------------------------------------------------------
// ----------------------Inline & templatized function definitions---------------------
// ------------------------------------------------------------------------------------

template<typename T>
T Rand( T Min, T Max)
{
    return Singleton<MersenneTwister>::Instance().Next( Min, Max );
}

template<int N, int M, typename T>
Matrix<N, M, T> RandomMatrix( T Min, T Max)
{
    Matrix<N, M, T> result;
    for (int i = 0; i < N*M; i++)
        result[i] = Rand( Min, Max );

    return result;
}



#endif