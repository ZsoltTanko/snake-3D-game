#ifndef SINGLETON_H
#define SINGLETON_H



// ------------------------------------------------------------------------------------
// ----------------------------------Included headers----------------------------------
// ------------------------------------------------------------------------------------
#include <cstddef>
#include <stdexcept>


// ------------------------------------------------------------------------------------
// --------------------Templatized class declarations & definitions--------------------
// ------------------------------------------------------------------------------------

// ------------------Creation policies for Singleton-------------------

// Create with new and destroy with delete
template <typename T>
class NewOperatorCreation
{
public:
	static T *Create()
	{ return new T; }

	static void Destroy(T *tObject)
	{
		delete tObject;
		tObject = NULL;
	}
};

// Create a static data member
template <typename T>
class StaticCreation
{
public:
	// Creates a local static variable and returns a pointer to it
	static T *Create()
	{
		static T tInstance;
		return &tInstance;
	}

	static void Destroy(T *tObject) {}
};


// ------------------Lifetime policies for Singleton-------------------

// Register an exit function with atexit(). Throws exception on dead reference
template <typename T>
class DefaultLifetime
{
public:
	typedef void (*DestructionFunction)();

	static void ScheduleDestruction(DestructionFunction ExitFunction)
	{
		std::atexit(ExitFunction);
	}

	static void OnDeadReference()
	{
		std::runtime_error("Dead reference in singleton");
	}
};

// Register an exit function with atexit(), allows the singleton to recreate itself upon dead reference detection
template <typename T>
class PhoenixLifetime
{
public:
	typedef void (*DestructionFunction)();

	static void ScheduleDestruction(DestructionFunction ExitFunction)
	{
		std::atexit(ExitFunction);
	}

	static void OnDeadReference() {}
};

// Allows the singleton to leak, will be cleaned up by OS. No dead reference handling required
template <typename T>
class InfiniteLifetime
{
public:
	typedef void (*DestructionFunction)();

	static void ScheduleDestruction(DestructionFunction ExitFunction) {}

	static void OnDeadReference() {}
};


// -----------------------------Singleton------------------------------

template <typename T,
		  template <class> class CreationPolicy = NewOperatorCreation,
		  template <class> class LifetimePolicy = DefaultLifetime>
class Singleton
{
public:
	// Retrieve instance of singleton, uses lazy initialization
	static T &Instance()
	{
		if (!pInstance)
		{
			if (bDestroyed)
			{
				LifetimePolicy<T>::OnDeadReference();
				bDestroyed = false;
			}
			pInstance = CreationPolicy<T>::Create();
			LifetimePolicy<T>::ScheduleDestruction(&DestroySingleton);
		}

		return *pInstance;
	}

private:
	static T *pInstance;
	static bool bDestroyed;

	// Destroy singleton, called by lifetime policy
	static void DestroySingleton()
	{
		CreationPolicy<T>::Destroy(pInstance);
		bDestroyed = true;
	}

	// Disable ctors, dtor, & equal operator
	Singleton() {}
	Singleton(Singleton &in) {}
	~Singleton() {}
	Singleton &operator =(Singleton &in) {}
};

// ------------------------------------------------------------------------------------
// ---------------------------Static data member definitions---------------------------
// ------------------------------------------------------------------------------------

template <typename T,
		  template <class> class CreationPolicy,
		  template <class> class LifetimePolicy>
T *Singleton<T, CreationPolicy, LifetimePolicy>::pInstance = NULL;

template <typename T,
		  template <class> class CreationPolicy,
		  template <class> class LifetimePolicy>
bool Singleton<T, CreationPolicy, LifetimePolicy>::bDestroyed = false;



#endif