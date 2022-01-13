#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

/// \file Exceptions.h
/// Contains the declaration of the IException interface and several subclasses that represent typical exceptions thrown by a program.


// ------------------------------------------------------------------------------------
// ------------------------------------- Includes -------------------------------------
// ------------------------------------------------------------------------------------

// C++ standard library
#include <string>


// ------------------------------------------------------------------------------------
// ------------------------------------ Interfaces ------------------------------------
// ------------------------------------------------------------------------------------

/// Interface which all exceptions implement.
/// Declares an empty virtual destructor so that subclasses are destroyed properly.
class IException
{
public:
    /// Empty virtual destructor.
    /// Declared only to ensure subclass destructors are invoked.
    virtual ~IException() = 0 {};

    /// Get a description of the exception.
    virtual const std::string &GetDescription() = 0;

    /// Get a description of the error.
    /// Returns a description of the exception, along with any other information about the error.
    virtual const std::string &GetError() = 0;
};


// ------------------------------------------------------------------------------------
// -------------------------------------- Classes -------------------------------------
// ------------------------------------------------------------------------------------

/// An exception caused by an index out of bounds error.
class CausedException : public IException
{
public:
    /// Default constructor, empty.
    CausedException() {}
    /// Set constructor, set the cause of the exception.
    CausedException( const std::string &Cause );

    /// Returns only the description of the exception.
    virtual const std::string &GetDescription();
    /// Returns only the cause of the exception, if one was provided at construction.
    virtual const std::string &GetCause();
    /// Returns a description of the exception, along with the cause, if one was provided at construction.
    virtual const std::string &GetError();

protected:
    std::string Message, Cause;

    /// Build the error string with all provided information.
    virtual void BuildErrorString();
};

/// An exception involving a variable.
/// The default constructor is private to ensure that all instances of VariableException
/// and its subclasses have at least the variable name set at construction.
class VariableException : public CausedException
{
public:
    /// Set constructor, sets the variable name.
    VariableException( const std::string &VariableName );
    /// Set constructor, sets the variable name and cause.
    VariableException( const std::string &VariableName, const std::string &Cause );

    /// Returns only the description of the exception.
    virtual const std::string &GetDescription();
    /// Get the name of the variable that caused the exception.
    const std::string &GetVariableName();

protected:
	std::string VariableName;

    /// Build the error string with all provided information.
    virtual void BuildErrorString();

private:
    // Private default constructor, empty.
    VariableException() {};
};

/// An exception caused by a logic error.
class LogicException : public CausedException
{
public:
    /// Set constructor, sets the cause.
    LogicException( const std::string &Cause );

    /// Returns only the description of the exception.
    virtual const std::string &GetDescription();

private:
    // Private default constructor, empty.
    LogicException() {};
};

/// An exception caused by an index out of bounds error.
/// The default constructor is private to ensure that all instances of IndexException
/// and its subclasses have at least the out of bounds variable name set at construction.
class IndexException : public VariableException
{
public:
    /// Set constructor, sets the out of bounds variable name.
    IndexException( const std::string &VariableName );
    /// Set constructor, sets the out of bounds variable name and cause.
    IndexException( const std::string &VariableName, const std::string &Cause );

    /// Returns only the description of the exception.
    virtual const std::string &GetDescription();
};

/// An exception caused by an invalid argument error.
class ArgumentException : public VariableException
{
public:
    /// Set constructor, sets the argument name.
    ArgumentException( const std::string &ArgumentName );
    /// Set constructor, sets the argument name and cause.
    ArgumentException( const std::string &ArgumentName, const std::string &Cause );

    /// Returns only the description of the exception.
    virtual const std::string &GetDescription();
    /// Get the name of the argument that caused the exception.
    const std::string &GetArgumentName();

protected:
    /// Build the error string with all provided information.
    virtual void BuildErrorString();
};

/// An exception caused by a file input or output error.
class FileIOException : public CausedException
{
public:
    /// Set constructor, set the filename of the file that caused the exception.
    FileIOException( const std::string &Filename );
    /// Set constructor, set the filename and cause of the exception.
    FileIOException( const std::string &Filename, const std::string &Cause );

    /// Returns only the description of the exception.
    virtual const std::string &GetDescription();
    /// Get the filename of the file that caused the exception.
    const std::string &GetFilename();

protected:
    std::string Filename;

    /// Build the error string with all provided information.
    virtual void BuildErrorString();

private:
    // Private default constructor, empty.
    FileIOException() {};
};

/// An exception caused by failure to find and open a file.
class FileNotFoundException : public FileIOException
{
public:
    /// Set constructor, set the filename of the file that caused the exception.
    FileNotFoundException( const std::string &Filename );
    /// Set constructor, set the filename and cause of the exception.
    FileNotFoundException( const std::string &Filename, const std::string &Cause );

    /// Returns only the description of the exception.
    virtual const std::string &GetDescription();
};



#endif