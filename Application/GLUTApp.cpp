// ------------------------------------------------------------------------------------
// ----------------------------------Included headers----------------------------------
// ------------------------------------------------------------------------------------
#include "GLUTApp.h"

// C standard library
#include <cstdlib>

// C++ standard library & STL
#include <string>
#include <stack>
#include <queue>
#include <set>
using namespace std;

// Windows/OpenGL
#include <Windows.h>
#include <gl/GL.h>
#include <gl/glut.h>

// Utilities
#include "..\Utilities\Singleton.h"
#include "..\Utilities\Timer.h"
#include "..\Utilities\SettingFile.h"

#include "..\IGameState.h"
#include "..\Camera.h"


// ------------------------------------------------------------------------------------
// -------------------------------RenderTextData Members-------------------------------
// ------------------------------------------------------------------------------------

RenderTextData::RenderTextData()
: Color(0, 0, 1), GLUTFont(GLUT_BITMAP_TIMES_ROMAN_10)
{
}

RenderTextData::RenderTextData( const std::string &Text, const Vector2f &Position, const Color3f &Color, void *GLUTFont )
: Text(Text), Position(Position), Color(Color), GLUTFont(GLUTFont)
{
}

RenderTextData::RenderTextData( const std::string &Text, const Vector2f &Position, const Color3f &Color )
: Text(Text), Position(Position), Color(Color), GLUTFont(GLUT_BITMAP_TIMES_ROMAN_10)
{
}

RenderTextData::RenderTextData( const std::string &Text, const Vector2f &Position )
: Text(Text), Position(Position), Color(0, 0, 1), GLUTFont(GLUT_BITMAP_TIMES_ROMAN_10)
{
}


// ------------------------------------------------------------------------------------
// -----------------------------------GLUTApp Members----------------------------------
// ------------------------------------------------------------------------------------

int GLUTApp::Run( const string &SettingsPath, HINSTANCE hInstance, char *CommandLine, int ShowCommand )
{
    // Allocate memory
    StateStack = new stack<IGameState *>;
    UpdateTimer = new PerformanceTimer;
    TextToRender = new queue<RenderTextData *>;
	PressedSet = new set<unsigned char>;
    MotionQueue = new queue<Vector2f>;
    ButtonQueue = new queue<int>;

    // Load settings
    SettingFile Settings( SettingsPath.c_str() );
    const string &WindowTitle = Settings.GetValue( "WindowTitle" ),
                 &InitialStateID = Settings.GetValue( "InitialStateID" );
    WindowWidth = Settings.GetValueAs<int>( "WindowWidth" );
    WindowHeight = Settings.GetValueAs<int>( "WindowHeight" );
    bool Fullscreen = Settings.GetValueAs<int>( "Fullscreen" ) == 1;

    // Initialize OpenGL & window
    glutInit( &ShowCommand, &CommandLine );
    glutInitDisplayMode( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA );
    glutInitWindowPosition( 0, 0 );
    glutInitWindowSize( WindowWidth, WindowHeight );
    glutCreateWindow( WindowTitle.c_str() );
    if (Fullscreen)
        glutFullScreen();

    // Ignore key auto-repeat
    glutIgnoreKeyRepeat( 1 );
	glutSetKeyRepeat( GLUT_KEY_REPEAT_OFF );

	// Disable cursor
	glutSetCursor( GLUT_CURSOR_NONE );

    // Set GLUT callbacks
    glutIdleFunc( OnIdleCallback );
    glutDisplayFunc( OnIdleCallback );
    glutKeyboardFunc( OnKeyPressCallback );
    glutKeyboardUpFunc( OnKeyReleaseCallback );
    glutPassiveMotionFunc( OnMouseMotionCallback );
    glutMotionFunc( OnMouseMotionCallback );
    glutMouseFunc( OnMousePressCallback );
    glutReshapeFunc( OnChangeSizeCallback );

    // Enable depth buffer and back face culling
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE );

    // Set buffer clear color to white
    glClearColor( 1, 1, 1, 1 );

    // Initialize first state
	PushState( InitialStateID );

    // Set perspective matrix
    ApplyGLPerspectiveMatrix();

    // Run
    glutMainLoop();

    // Never reached, but the compiler doesn't know that
    return 0;
}

void GLUTApp::Exit()
{
	Destroy();
	exit( 0 );
}

void GLUTApp::Destroy()
{
    for (; !StateStack->empty(); StateStack->pop())
        delete StateStack->top();

    delete StateStack;
    delete UpdateTimer;
    delete TextToRender;
    delete PressedSet;
    delete MotionQueue;
    delete ButtonQueue;
}

void GLUTApp::PushState( const string &StateID )
{
    // Create instance of new state
	IGameState *NewState = IGameState::New( StateID );

    // Initialize it
	NewState->Init();

    // Push it onto stack
	StateStack->push( NewState );
}

void GLUTApp::PopState()
{
    // Get reference to top state
    IGameState *TopState = StateStack->top();

    // Pop it from the stack
    StateStack->pop();

    // Free it
    delete TopState;
}

void GLUTApp::OnUpdate()
{
    // Get elapsed time since last update
    float Elapsed = UpdateTimer->GetElapsed();
    UpdateTimer->Reset();

    // Get reference to current state
	IGameState *CurrentState = StateStack->top();

    // Update current state
    CurrentState->Update( Elapsed );

	// Only Update() should change the current game state, so check for a new top state
	if (CurrentState != StateStack->top())
		return;

    // Check if the current state is finished
    if (CurrentState->IsFinished())
    {
		PopState();

		// If no more states exist then the game must be over
		if (StateStack->empty())
			Singleton<GLUTApp>::Instance().Exit();

		return;
    }

    // Clear screen & depth buffer
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Invoke render for current game state
    CurrentState->Render();

    // Render text
    RenderTextQueue();

    // Swap back buffer to front
    glutSwapBuffers();

    // For reasons unknown to me, calling glutBitmapCharacter() causes GLUT to
    // fire passive mouse motion events much more frequently, greatly reducing
    // an otherwise apparent (and unwanted) skipping effect. Since the call is
    // made after the back buffer swap, the rendered character is never seen.
    glutBitmapCharacter( GLUT_BITMAP_8_BY_13, ' ' );

    // Clear user input buffers
    PressedSet->clear();
    for (; !MotionQueue->empty(); MotionQueue->pop());
    for (; !ButtonQueue->empty(); ButtonQueue->pop());
}

void GLUTApp::OnKeyPress( unsigned char Key )
{
    // Exit on escape key press
    if (Key == 27)
        Exit();

	PressedSet->insert( Key );
}

void GLUTApp::OnKeyRelease( unsigned char KeyReleased )
{
	PressedSet->erase( KeyReleased );
}

void GLUTApp::OnMouseMotion( int MouseX, int MouseY )
{
	// Previous position of mouse
	static Vector2f PrevPos( (float)MouseX/WindowWidth, (float)MouseY/WindowHeight );

    // Current position of mouse
    Vector2f CurPos( (float)MouseX/WindowWidth, (float)MouseY/WindowHeight );

    // Push the difference onto the queue
    MotionQueue->push( CurPos - PrevPos );

    // Save current position
    PrevPos = CurPos;

	// Reset mouse to center of screen when it nears the edge
	if (CurPos.x() <= 0.1f || CurPos.y() <= 0.1f || CurPos.x() >= 0.9f || CurPos.y() >= 0.9f)
	{
		PrevPos.x() = 0.5f;
		PrevPos.y() = 0.5f;
		glutWarpPointer( WindowWidth/2, WindowHeight/2 );
	}
}

void GLUTApp::OnMousePress( int Button )
{
    ButtonQueue->push( Button );
}

void GLUTApp::OnChangeSize( int Width, int Height )
{
    if (Height == 0)
        Height = 1;

    WindowWidth = Width;
    WindowHeight = Height;

    // Reset the perspective matrix, new window size probably means a new aspect ratio
    ApplyGLPerspectiveMatrix();
}

void GLUTApp::RenderText( RenderTextData *TextData )
{
    TextToRender->push( TextData );
}

void GLUTApp::RenderTextQueue()
{
    if (TextToRender->size() == 0)
        return;

    for (RenderTextData *TextData = TextToRender->front(); !TextToRender->empty(); TextToRender->pop())
    {
        // Push identity matrix onto projection matrix stack
        glMatrixMode( GL_PROJECTION );
        glPushMatrix();
        glLoadIdentity();

        // Set a 2D orthographic projection matrix
        gluOrtho2D( 0, WindowWidth, 0, WindowHeight );
        glScalef( 1, -1, 1 );
        glTranslatef( 0, -WindowHeight, 0 );

        // Push identity matrix onto model-view matrix stack
        glMatrixMode( GL_MODELVIEW );
        glPushMatrix();
        glLoadIdentity();

        // Set color & position
        glColor3fv( (float *)&TextData->Color );
        glRasterPos2fv( (float *)&TextData->Position );

        // Render characters via GLUT
        for (unsigned int i = 0; i < TextData->Text.length(); i++)
            glutBitmapCharacter( TextData->GLUTFont, TextData->Text[i] );

        // Pop matrices off stacks
        glPopMatrix();
        glMatrixMode( GL_PROJECTION );
        glPopMatrix();
    }
}

void GLUTApp::ApplyGLPerspectiveMatrix()
{
    // Get pointer to current camera
    const Camera *CurCamera = Singleton<Camera *>::Instance();

    // Set projection matrix to identity
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    // Set the view port to the entire window
    glViewport( 0, 0, WindowWidth, WindowHeight );

    // Set projection matrix
    gluPerspective( CurCamera->GetFOVY(), (double)WindowWidth/WindowHeight, CurCamera->GetNearClip(), CurCamera->GetFarClip() );
}

void GLUTApp::OnIdleCallback()
{
    Singleton<GLUTApp>::Instance().OnUpdate();
}

void GLUTApp::OnKeyPressCallback( unsigned char Key, int MouseX, int MouseY )
{
    Singleton<GLUTApp>::Instance().OnKeyPress( Key );
}

void GLUTApp::OnKeyReleaseCallback( unsigned char Key, int MouseX, int MouseY )
{
    Singleton<GLUTApp>::Instance().OnKeyRelease( Key );
}

void GLUTApp::OnMouseMotionCallback( int MouseX, int MouseY )
{
    Singleton<GLUTApp>::Instance().OnMouseMotion( MouseX, MouseY );
}

void GLUTApp::OnMousePressCallback( int Button, int State, int MouseX, int MouseY )
{
    Singleton<GLUTApp>::Instance().OnMousePress( Button );
}

void GLUTApp::OnChangeSizeCallback( int Width, int Height )
{
    Singleton<GLUTApp>::Instance().OnChangeSize( Width, Height );
}