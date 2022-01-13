#ifndef TMATH_H
#define TMATH_H



// ------------------------------------------------------------------------------------
// -------------------------------------Includes---------------------------------------
// ------------------------------------------------------------------------------------
#include <cmath>
#include <limits>


namespace TMath
{
    // ------------------------------------------------------------------------------------
    // -------------------------------------Constants--------------------------------------
    // ------------------------------------------------------------------------------------

    // If debugging convert arguments to standard math functions into doubles for extra precision
#ifdef DEBUG
#define MATHDEBUG
#endif

#ifdef MATHDEBUG
    typedef double FLOATTYPE;
#else
    typedef float FLOATTYPE;
#endif

    const FLOATTYPE PI = static_cast<FLOATTYPE>(3.1415926535897932);
    const FLOATTYPE E = static_cast<FLOATTYPE>(2.7182818284590452);
    const FLOATTYPE EPSILON = static_cast<FLOATTYPE>(0.005);
    const FLOATTYPE RAD_IN_DEG = static_cast<FLOATTYPE>(180.0/PI);
    const FLOATTYPE DEG_IN_RAD = static_cast<FLOATTYPE>(PI/180.0);


    // ------------------------------------------------------------------------------------
    // -------------Inline & templatized function declarations & definitions---------------
    // ------------------------------------------------------------------------------------

    // Return absolute value of t
    template<typename T>
    inline T Abs(T t)
    {
        if (t < static_cast<T>(0))
            return -t;

        return t;
    }

    // Return maximum of a & b
    template<typename T>
    inline T Max(T a, T b)
    {
        if (a > b)
            return a;

        return b;
    }

    // Return minimum of a & b
    template<typename T>
    inline T Min(T a, T b)
    {
        if (a < b)
            return a;

        return b;
    }

    // Square t
    template<typename T>
    inline T Sqr(T t)
    {
        return t*t;
    }

    // Cube t
    template<typename T>
    inline T Cube(T t)
    {
        return t*t*t;
    }

    // Convert degrees to radians
    template<typename T>
    inline T DegToRad(T t)
    {
        return t*static_cast<T>(DEG_IN_RAD);
    }

    // Convert radians to degrees
    template<typename T>
    inline T RadToDeg(T t)
    {
        return t*static_cast<T>(RAD_IN_DEG);
    }

    // Return t clamped to the range [min,max]
    template<typename T>
    inline T Clamp(T t, T min, T max)
    {
        if (t > max)
            return max;
        else if (t < min)
            return min;

        return t;
    }

    // Clamp t to the range [min,max] and place result in t
    template<typename T>
    inline void ClampTo(T &t, T min, T max)
    {
        if (t > max)
            t = max;
        else if (t < min)
            t = min;
    }

    // Check for equality with optional epsilon value
    template<typename T>
    inline bool EqualBy(T a, T b, T epsilon = EPSILON)
    {
        if (Abs(a-b) <= epsilon)
            return true;

        return false;
    }

    // Round t up to the next integral value
    template<typename U, typename T>
    inline U Ceil(T t)
    {
        return static_cast<U>(ceil(static_cast<FLOATTYPE)(t));
    }

    // Return integral portion of t
    template<typename U, typename T>
    inline U Floor(T t)
    {
        return static_cast<U>(floor(static_cast<FLOATTYPE>(t)));
    }

    // Return fractional portion of t
    template<typename T>
    inline T Frac(T t)
    {
        return t-Floor<T>(t);
    }

    // Return remained for x/y
    template <typename T>
    inline T Mod(T x, T y)
    {
        return x%y;
    }
    // Specializations for floating point types
    template <>
    inline float Mod(float x, float y)
    {
        return fmod(x, y);
    }
    template <>
    inline double Mod(double x, double y)
    {
        return fmod(x, y);
    }
    template <>
    inline long double Mod(long double x, long double y)
    {
        return fmod(x, y);
    }

    // Return square root of t
    template<typename T>
    inline T Sqrt(T t)
    {
        return static_cast<T>(sqrt(static_cast<FLOATTYPE>(t)));
    }

    // Linear interpolation
    template <typename T, typename U>
    inline U LinearInterpolate(const T tPercent, const U uSample1, const U uSample2)
    {
        return uSample1*(1-tPercent)+uSample2*tPercent;
    }

    // Cosine interpolation
    template <typename T, typename U>
    inline U CosineInterpolate(const T tPercent, const U uSample1, const U uSample2)
    {
        T tTemp = (1-cos(tPercent*static_cast<T>(PI)))*static_cast<T>(0.5);
        return uSample1*(1-tTemp)+uSample2*tTemp;
    }

    // Cubic interpolation
    template <typename T, typename U>
    inline U CubicInterpolate(const T tPercent, const U uSample1, const U uSample2, const U uSample3, const U uSample4)
    {
        U uTemp = (uSample4-uSample3)-(uSample1-uSample2);
        return tPercent*(tPercent*(tPercent*uTemp + ((uSample1-uSample2)-uTemp)) + (uSample3-uSample1)) + uSample2;
    }


    // Trig functions
    template<typename T>
    inline T Sin(T t)
    {
        return static_cast<T>(sin(static_cast<FLOATTYPE>(t)));
    }
    template<typename T>
    inline T Cos(T t)
    {
        return static_cast<T>(cos(static_cast<FLOATTYPE>(t)));
    }
    template<typename T>
    inline T Tan(T t)
    {
        return static_cast<T>(tan(static_cast<FLOATTYPE>(t)));
    }
    template<typename T>
    inline T ArcSin(T t)
    {
        return static_cast<T>(asin(static_cast<FLOATTYPE>(t)));
    }
    template<typename T>
    inline T ArcCos(T t)
    {
        return static_cast<T>(acos(static_cast<FLOATTYPE>(t)));
    }
    template<typename T>
    inline T ArcTan(T t)
    {
        return static_cast<T>(atan(static_cast<FLOATTYPE>(t)));
    }


    // Convert string to fundamental type
    template <typename T>
    inline T StrToT(const char *pString)
    {
        return static_cast<T>(atoi(pString));
    }
    template <>
    inline bool StrToT(const char *pString)
    {
        return static_cast<bool>(atoi(pString) != 0);
    }
    template <>
    inline unsigned int StrToT(const char *pString)
    {
        char *pEnd;
        return static_cast<unsigned int>(strtoul(pString, &pEnd, 0));
    }
    template <>
    inline unsigned long StrToT(const char *pString)
    {
        char *pEnd;
        return static_cast<unsigned long>(strtoul(pString, &pEnd, 0));
    }
    template <>
    inline float StrToT(const char *pString)
    {
        return static_cast<float>(atof(pString));
    }
    template <>
    inline double StrToT(const char *pString)
    {
        return static_cast<double>(atof(pString));
    }
    template <>
    inline long double StrToT(const char *pString)
    {
        return static_cast<long double>(atof(pString));
    }
}



#endif