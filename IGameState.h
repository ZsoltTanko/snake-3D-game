#ifndef IGAMESTATE_H
#define IGAMESTATE_H


// ------------------------------------------------------------------------------------
// ----------------------------------Included headers----------------------------------
// ------------------------------------------------------------------------------------

// C++ standard library
#include <string>


// ------------------------------------------------------------------------------------
// --------------------------------------Interfaces------------------------------------
// ------------------------------------------------------------------------------------

class IGameState
{
public:
    virtual void Init() = 0;
    virtual void DeInit() = 0;

    virtual void Update( float ElapsedTime ) = 0;
    virtual void Render() const = 0;

    virtual bool IsFinished() = 0;

    // Factory method
    static IGameState *New( const std::string &ID );
};



#endif