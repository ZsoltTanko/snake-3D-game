// ------------------------------------------------------------------------------------
// ----------------------------------Included headers----------------------------------
// ------------------------------------------------------------------------------------
#include "IGameState.h"

// C++ standard library
#include <string>
using namespace std;

// Utilities
#include "Utilities\Singleton.h"
#include "Utilities\Factory.h"


// ------------------------------------------------------------------------------------
// -----------------------------IGameState Factory Method------------------------------
// ------------------------------------------------------------------------------------
IGameState *IGameState::New( const string &ID )
{
    return Singleton<Factory<IGameState, string>>::Instance().CreateProduct( ID );
}