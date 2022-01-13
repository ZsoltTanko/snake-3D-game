#ifndef GLUTAPP_H
#define GLUTAPP_H



// ------------------------------------------------------------------------------------
// ----------------------------------Included headers----------------------------------
// ------------------------------------------------------------------------------------

// C++ standard library & STL
#include <string>
#include <stack>
#include <queue>
#include <set>

// Windows
#include <Windows.h>

// Utilities
#include "..\Utilities\Matrix.h"


// ------------------------------------------------------------------------------------
// --------------------------------------Structures------------------------------------
// ------------------------------------------------------------------------------------
struct RenderTextData
{
    // Constructors
    RenderTextData();
    RenderTextData( const std::string &Text, const Vector2f &Position, const Color3f &Color, void *GLUTFont );
    RenderTextData( const std::string &Text, const Vector2f &Position, const Color3f &Color );
    RenderTextData( const std::string &Text, const Vector2f &Position );

    std::string Text;
    Color3f Color;
    Vector2f Position;
    void *GLUTFont;
};


// ------------------------------------------------------------------------------------
// ---------------------------------------Classes--------------------------------------
// ------------------------------------------------------------------------------------

// Design notes:
// - Does not use destructor since glut's main loop never exits. Uses Destroy() instead.
// - Fields allocated on the stack are leaked. The OS handles deallocation, but object
//   destructors are never called. Therefore, all data is dynamically allocated and
//   manually freed in Destroy().
class IGameState;
class PerformanceTimer;
class GLUTApp
{
public:
    int Run( const std::string &SettingsPath, HINSTANCE hInstance, char *CommandLine, int ShowCommand );
	void Exit();

    // Accessors
    inline std::queue<Vector2f> &GetMouseMotion();
    inline std::queue<int> &GetPressedMouseButtons();
    inline std::set<unsigned char> GetPressedKeys();
	inline int GetWindowWidth() const;
	inline int GetWindowHeight() const;

    // Change game state
	void PushState( const std::string &StateID );
    void PopState();
    
    // Text rendering
    void RenderText( RenderTextData *TextData );

private:
    std::stack<IGameState *> *StateStack;

    PerformanceTimer *UpdateTimer;
    int WindowWidth, WindowHeight;

    std::queue<RenderTextData *> *TextToRender;

    // User input queues
    std::queue<Vector2f> *MotionQueue;
    std::queue<int> *ButtonQueue;
	std::set<unsigned char> *PressedSet;


    // Frees all allocated memory
    void Destroy();

    // Rendering methods
    void RenderTextQueue();
    void ApplyGLPerspectiveMatrix();

    // Event handlers
    void OnUpdate();
    void OnKeyPress( unsigned char Key );
    void OnKeyRelease( unsigned char KeyReleased );
    void OnMouseMotion( int MouseX, int MouseY );
    void OnMousePress( int Button );
    void OnChangeSize( int Width, int Height );

    // GLUT callbacks - These methods invoke the event handlers on a singleton instance of GLUTApp.
    static void OnIdleCallback();
    static void OnKeyPressCallback( unsigned char Key, int MouseX, int MouseY );
    static void OnKeyReleaseCallback( unsigned char Key, int MouseX, int MouseY );
    static void OnMouseMotionCallback( int MouseX, int MouseY );
    static void OnMousePressCallback( int Button, int State, int MouseX, int MouseY );
    static void OnChangeSizeCallback( int Width, int Height );
};


// ------------------------------------------------------------------------------------
// -----------------------------Inline function definitions----------------------------
// ------------------------------------------------------------------------------------

std::queue<Vector2f> &GLUTApp::GetMouseMotion()
{
    return *MotionQueue;
}

std::queue<int> &GLUTApp::GetPressedMouseButtons()
{
    return *ButtonQueue;
}

std::set<unsigned char> GLUTApp::GetPressedKeys()
{
    return *PressedSet;
}

int GLUTApp::GetWindowWidth() const
{
	return WindowWidth;
}

int GLUTApp::GetWindowHeight() const
{
	return WindowHeight;
}



#endif