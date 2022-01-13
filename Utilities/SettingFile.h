#ifndef SETTINGFILE_H
#define SETTINGFILE_H



// ------------------------------------------------------------------------------------
// ----------------------------------Included headers----------------------------------
// ------------------------------------------------------------------------------------

// C++ standard library & STL
#include <string>
#include <map>

// Utilities
#include "..\Utilities\TMath.h"


// ------------------------------------------------------------------------------------
// ---------------------------------------Classes--------------------------------------
// ------------------------------------------------------------------------------------

/// Manages a text settings file.
/// Manages a collection of name and value pairs that represent settings. Settings are
/// saved to file one per line, with a single space between the setting name and value.
class SettingFile
{
public:
    /// Default constructor, empty.
    SettingFile();

    /// Constructor, load settings from text file.
    /// - May throw: FileIOException, FileNotFoundException
    SettingFile( const char *FilePath );

    /// Load settings from text file.
    /// - May throw: FileIOException, FileNotFoundException
    void Load( const char *FilePath );

    /// Save settings to text file.
    /// - May throw: FileIOException
    void Save( const char *FilePath ) const;

    /// Get value of setting with name Name.
    /// - May throw: LogicException
    const std::string &GetValue( const char *Name ) const;

    template<typename T>
    /// Get value of setting with name Name as type T.
    /// - May throw: LogicException
    inline T GetValueAs( const char *Name ) const;

    /// Set value of setting with name Name.
    /// - May throw: LogicException
    void SetValue( const char *Name, const char *Value );

    /// Add a new setting.
    /// - Throws a LogicException if a setting with name Name already exists.
    /// - May throw: LogicException
    void AddSetting( const char *Name, const char *Value );

    /// Remove an existing setting.
    /// - Throws a LogicException if a setting with name Name does not exists.
    /// - May throw: LogicException
    void RemoveSetting( const char *Name );

private:
    std::map<std::string, std::string> SettingMap;
};


// ------------------------------------------------------------------------------------
// ---------------------------Templatized member definitions---------------------------
// ------------------------------------------------------------------------------------

template<typename T>
T SettingFile::GetValueAs( const char *Name ) const
{
    return TMath::StrToT<T>( GetValue( Name ).c_str() );
}



#endif