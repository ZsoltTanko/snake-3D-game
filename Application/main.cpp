// ------------------------------------------------------------------------------------
// ----------------------------------Included headers----------------------------------
// ------------------------------------------------------------------------------------

// C++ standard library
#include <string>
using namespace std;

// Windows
#include <Windows.h>

// Utilities
#include "..\Utilities\Singleton.h"
#include "..\Utilities\SettingFile.h"

#include "GLUTApp.h"


// ------------------------------------------------------------------------------------
// ----------------------------------------Main----------------------------------------
// ------------------------------------------------------------------------------------
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
    // Load config file
    SettingFile ConfigFile( "Config.txt" );

    // Run application
    return Singleton<GLUTApp>::Instance().Run( ConfigFile.GetValue( "SettingsPath" ), hInstance, lpCmdLine, nShowCmd );
}