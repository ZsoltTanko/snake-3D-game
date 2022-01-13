
/// \file Exceptions.cpp
/// Contains the definition of several classes that implement IException.
/// Contains definitions for: VariableException, IndexException, LogicException,
/// ArgumentException, FileIOException, FileNotFoundException


// ------------------------------------------------------------------------------------
// ------------------------------------- Includes -------------------------------------
// ------------------------------------------------------------------------------------
#include "Exceptions.h"

// C++ standard library
#include <string>
using namespace std;


// ------------------------------------------------------------------------------------
// ----------------------------- CausedException methods -------------------------------
// ------------------------------------------------------------------------------------

CausedException::CausedException( const string &Cause ) : Cause(Cause)
{
}

const string &CausedException::GetDescription()
{
    static string Description( "An exception with a specified cause has occurred." );
    return Description;
}
const std::string &CausedException::GetCause()
{
    return Cause;
}

const string &CausedException::GetError()
{
    // Set message via lazy initialization
    if (Message.empty())
        BuildErrorString();

    return Message;
}

void CausedException::BuildErrorString()
{
    Message += "Error: ";
    Message += GetDescription();
    Message += "\n";

    if (!Cause.empty())
    {
        Message += "Cause: ";
        Message += Cause;
        Message += "\n";
    }
}


// ------------------------------------------------------------------------------------
// ---------------------------- VariableException methods -----------------------------
// ------------------------------------------------------------------------------------

VariableException::VariableException( const string &VariableName ) : VariableName(VariableName)
{
}

VariableException::VariableException( const string &VariableName, const string &Cause ) : CausedException(Cause), VariableName(VariableName)
{
}

const string &VariableException::GetDescription()
{
    static string Description( "Invalid variable exception has occurred." );
    return Description;
}

const string &VariableException::GetVariableName()
{
    return VariableName;
}

void VariableException::BuildErrorString()
{
    CausedException::BuildErrorString();

    Message += "Variable: ";
    Message += VariableName;
    Message += "\n";
}


// ------------------------------------------------------------------------------------
// ----------------------------- LogicException methods -------------------------------
// ------------------------------------------------------------------------------------

LogicException::LogicException( const string &Cause ) : CausedException(Cause)
{
}

const string &LogicException::GetDescription()
{
    static string Description( "Logic exception has occurred." );
    return Description;
}


// ------------------------------------------------------------------------------------
// ----------------------------- IndexException methods -------------------------------
// ------------------------------------------------------------------------------------

IndexException::IndexException( const string &VariableName ) : VariableException(VariableName)
{
}

IndexException::IndexException( const string &VariableName, const string &Cause ) : VariableException(VariableName, Cause)
{
}

const string &IndexException::GetDescription()
{
    static string Description( "Index out of bounds exception has occurred." );
    return Description;
}


// ------------------------------------------------------------------------------------
// ---------------------------- ArgumentException methods -----------------------------
// ------------------------------------------------------------------------------------

ArgumentException::ArgumentException( const string &ArgumentName ) : VariableException(ArgumentName)
{
}

ArgumentException::ArgumentException( const string &ArgumentName, const string &Cause ) : VariableException(ArgumentName, Cause)
{
}

const string &ArgumentException::GetDescription()
{
    static string Description( "Invalid argument exception has occurred." );
    return Description;
}

const string &ArgumentException::GetArgumentName()
{
    return GetVariableName();
}

void ArgumentException::BuildErrorString()
{
    CausedException::BuildErrorString();

    Message += "Argument name: ";
    Message += VariableName;
    Message += "\n";
}


// ------------------------------------------------------------------------------------
// ----------------------------- FileIOException methods ------------------------------
// ------------------------------------------------------------------------------------

FileIOException::FileIOException( const string &Filename ) : Filename(Filename)
{
}

FileIOException::FileIOException( const string &Filename, const string &Cause ) : CausedException(Cause), Filename(Filename)
{
}

const string &FileIOException::GetDescription()
{
    static string Description( "File input/output exception has occurred." );
    return Description;
}

const string &FileIOException::GetFilename()
{
    return Filename;
}

void FileIOException::BuildErrorString()
{
    CausedException::BuildErrorString();

    Message += "File name: ";
    Message += Filename;
    Message += "\n";
}


// ------------------------------------------------------------------------------------
// -------------------------- FileNotFoundException methods ---------------------------
// ------------------------------------------------------------------------------------

FileNotFoundException::FileNotFoundException( const string &Filename ) : FileIOException(Filename)
{
}

FileNotFoundException::FileNotFoundException( const string &Filename, const string &Cause ) : FileIOException(Filename, Cause)
{
}

const string &FileNotFoundException::GetDescription()
{
    static string Description( "File not found exception has occurred." );
    return Description;
}