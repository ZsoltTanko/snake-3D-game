// ------------------------------------------------------------------------------------
// ----------------------------------Included headers----------------------------------
// ------------------------------------------------------------------------------------
#include "SettingFile.h"

// C++ standard library & STL
#include <string>
#include <iostream>
#include <fstream>
#include <map>
using namespace std;

// Utilities
#include "Exceptions.h"


// ------------------------------------------------------------------------------------
// ---------------------------------SettingFile Members--------------------------------
// ------------------------------------------------------------------------------------

SettingFile::SettingFile()
{
}

SettingFile::SettingFile( const char *FilePath )
{
    Load( FilePath );
}

void SettingFile::Load( const char *FilePath )
{
    // Empty map
    if (SettingMap.size() > 0)
        SettingMap.clear();

    // Open file
    ifstream fin( FilePath );
    if (!fin.good())
        throw FileNotFoundException( FilePath );

    // Iterate through lines on file
    char NameBuffer[100], ValueBuffer[100];
    while (!fin.eof())
    {
        // Read setting name
        fin.getline( NameBuffer, 100, ' ' );
        if (!fin.good())
            throw FileIOException( FilePath, "Settings data is corrupt." );

        // Read setting value
        fin.getline( ValueBuffer, 100, '\n' );
        if (!fin.good())
            throw FileIOException( FilePath, "Settings data is corrupt." );

        // Insert setting into map
        SettingMap.insert( make_pair<string, string>(NameBuffer, ValueBuffer) );

        // Peek to set eofbit if end of file is met
        fin.peek();
    }

    fin.close();
}

void SettingFile::Save( const char *FilePath ) const
{
    // Open/create file
    ofstream fout( FilePath, ios::out | ios::trunc );
    if (!fout.good())
        throw FileIOException( FilePath, "Failed to create settings file." );

    // Iterate through setting map
    for (map<string, string>::const_iterator it = SettingMap.begin(); it != SettingMap.end(); ++it)
    {
        // Write setting name & value
        fout << (*it).first;
        fout << ' ';
        fout << (*it).second;
        fout << '\n';

        if (!fout.good())
            throw FileIOException( FilePath, "Failed to write setting to file." );
    }

    fout.close();
}

const string &SettingFile::GetValue( const char *Name ) const
{
    // Find setting
    map<string, string>::const_iterator it = SettingMap.find( Name );

    // Assure it was found
    if (it == SettingMap.end())
        throw LogicException( string() + "Setting name \"" + Name + "\" not found." );

    return (*it).second;
}

void SettingFile::SetValue( const char *Name, const char *Value )
{
    // Find setting
    map<string, string>::iterator it = SettingMap.find( Name );

    // Assure it was found
    if (it == SettingMap.end())
        throw LogicException( string() + "Setting name \"" + Name + "\" not found." );

    (*it).second = Value;
}

void SettingFile::AddSetting( const char *Name, const char *Value )
{
    if (SettingMap.insert( make_pair<string, string>(Name, Value) ).second == false)
        throw LogicException( string() + "Setting name \"" + Name + "\" already in use." );
}

void SettingFile::RemoveSetting( const char *Name )
{
    if (SettingMap.erase( Name ) == 0)
        throw LogicException( string() + "Setting name \"" + Name + "\" not found." );
}