#ifndef TEMPLATEUTILS_H
#define TEMPLATEUTILS_H


// Determine inheritance relationships at compile-time
template <typename T, typename U>
class Conversion
{
	// Define two types, Big & Small, which have different sizes
	class Big { char p[2]; };
	typedef char Small;

	// Function to test if T to U conversion is possible, if so Test( const U & ) will be called
	static Small Test( const U & );
	static Big Test( ... );
	// Assures T gets created, since T() might be private
	static T MakeT();

public:
	static const bool Exists = sizeof(Test( MakeT() )) == sizeof(Small);
	static const bool SameType = false;
};
// Partial specialization for T == U
template <typename T>
class Conversion<T, T>
{
public:
	static const bool Exists = true;
	static const bool SameType = true;
};

// Macro to perform test
#define T_INHERITS_U( T, U ) (Conversion<const T *, const U *>::Exists && !Conversion<const T *, const void *>::SameType)


// Select between two types based on a boolean value, if flag is false specialization is used
template <bool flag, typename T, typename U>
struct Select
{ typedef T Result; };
template <typename T, typename U>
struct Select<false, T, U>
{ typedef U Result; };


// Compile time assert class
template <bool>
struct CompileTimeChecker { CompileTimeChecker( ... ); };
// Specialization for false
template <>
struct CompileTimeChecker<false> {};

// Macro to use compile time assert & result in a custom error message
#define STATIC_CHECK( expr, msg ) \
{\
	class ERROR_##msg {};\
	sizeof( new CompileTimeChecker<(expr) != 0>( ERROR_##msg() ) );\
	true;\
}\


#endif