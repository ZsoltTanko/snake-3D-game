// ------------------------------------------------------------------------------------
// ----------------------------------Included headers----------------------------------
// ------------------------------------------------------------------------------------
#include "Snake3DGameStates.h"

// STL
#include <queue>
#include <set>
using namespace std;

// Windows/OpenGL
#include <Windows.h>
#include <gl/GL.h>
#include <gl/glut.h>

// Utilities
#include "Utilities\Factory.h"
#include "Utilities\Singleton.h"
#include "Utilities\Matrix.h"
#include "Utilities\Rand Utilities.h"

#include "Application\GLUTApp.h"
#include "IGameState.h"
#include "Snake3DObjects.h"
#include "Camera.h"


// ------------------------------------------------------------------------------------
// ------------------------------Snake3DGameWorld Members------------------------------
// ------------------------------------------------------------------------------------

// Set type ID for IGameState factory
FactoryRegistrar<IGameState, Snake3DGameWorld, std::string> Snake3DGameWorld::Registrar( "Snake3DGameWorld" );


Snake3DGameWorld::Snake3DGameWorld()
{
	Initialized = false;
}

Snake3DGameWorld::~Snake3DGameWorld()
{
	DeInit();
}

void Snake3DGameWorld::Init()
{
    // Create game objects
    EnvSphereSize = 60;
    snake = new Snake( Vector3f(0, 0, 0), Vector3f(1, 0, 0), 0.01f, 40, 1.0f );
    snakeFood = new SnakeSegment( RandomMatrix<3, 1, float>(-EnvSphereSize * 0.5f, EnvSphereSize * 0.5f), 5, Color3f(1, 0, 0) );

    // Create camera
    camera = new Camera( snake->GetPosition(), snake->GetHeading(), 80, 1, 200 );

    // Set the global current camera
    Singleton<Camera *>::Instance() = camera;

    // Set line width for environment sphere render
    glLineWidth( 5 );

    Initialized = true;
    Finished = false;
    Paused = false;
}

void Snake3DGameWorld::DeInit()
{
    if (Initialized)
    {
        delete snake;
        delete snakeFood;
        delete camera;

        Initialized = false;
    }
}

void Snake3DGameWorld::Update( float ElapsedTime )
{
    // Process user input
    ProcessKeys( Singleton<GLUTApp>::Instance().GetPressedKeys() );
    ProcessMouseMotion( Singleton<GLUTApp>::Instance().GetMouseMotion() );

    snake->Update( ElapsedTime );

    // Move camera with snake
    camera->SetPosition( snake->GetPosition() - camera->GetLook() * 50 + Vector3f(0, 5, 0) );

    // Check for snake-environment sphere collision
    if (snake->GetPosition().GetMagnitudeSqr() > TMath::Sqr(EnvSphereSize))
        GameOver();

    // Check for snake head-snake body collision
    if (snake->IsSelfColliding())
        GameOver();

    // Check for snake-food collision
    if ((snake->GetPosition() - snakeFood->GetPosition()).GetMagnitudeSqr() <= TMath::Sqr(snake->GetSegmentSize() + snakeFood->GetSize()))
    {
        // Add a segment to the snake
		snake->IncreaseLength();

        // Reposition food
		snakeFood->SetPosition( RandomMatrix<3, 1, float>(-EnvSphereSize * 0.5f, EnvSphereSize * 0.5f) );
    }
    
    // Set view matrix
    Singleton<Camera *>::Instance()->ApplyGLViewMatrix();
}

void Snake3DGameWorld::Render() const
{
    // Render the environment sphere. Depth buffer is disabled since the sphere should always be in the background.
    glDepthMask( false );
    glDisable( GL_DEPTH_TEST );

    glColor3f( 0, 0, 0 );
    glutWireSphere( EnvSphereSize, 20, 20 );

    glDepthMask( true );
    glEnable( GL_DEPTH_TEST );

    snake->Render();
    snakeFood->Render();
}

bool Snake3DGameWorld::IsFinished()
{
    return Finished;
}

void Snake3DGameWorld::ProcessMouseMotion( queue<Vector2f> &MotionQueue )
{
    if (MotionQueue.size() == 0)
        return;

    // Sum mouse movements into a single rotation
    Vector3f RotationSum(0);
    for (const Vector2f &Move = MotionQueue.front(); !MotionQueue.empty(); MotionQueue.pop())
    {
        RotationSum.x() += Move.y();
        RotationSum.y() += Move.x();
    }
    RotationSum *= 2*TMath::PI;

    // Rotate snake & camera
    snake->RotateHeading( RotationSum );
    camera->Rotate( RotationSum );
}

void Snake3DGameWorld::ProcessKeys( set<unsigned char> &PressedSet )
{
    // Change to paused state on P key press
	if (PressedSet.count( 'p' ) > 0)
        Singleton<GLUTApp>::Instance().PushState( "Snake3DPaused" );
}

void Snake3DGameWorld::GameOver()
{
    Finished = true;

    // Change to game over state
	Singleton<GLUTApp>::Instance().PushState( "Snake3DGameOver" );
}


// ------------------------------------------------------------------------------------
// ------------------------------Snake3DPaused Members-------------------------------
// ------------------------------------------------------------------------------------

// Set type ID for IGameState factory
FactoryRegistrar<IGameState, Snake3DPaused, std::string> Snake3DPaused::Registrar( "Snake3DPaused" );

Snake3DPaused::Snake3DPaused()
{
	Initialized = false;
}

Snake3DPaused::~Snake3DPaused()
{
	DeInit();
}

void Snake3DPaused::Init()
{
	int WindowWidth = Singleton<GLUTApp>::Instance().GetWindowWidth(),
		WindowHeight = Singleton<GLUTApp>::Instance().GetWindowHeight();
	PauseText = new RenderTextData( "Game Paused", Vector2f(WindowWidth*0.5f - 50, WindowHeight*0.5f), Color3f(1, 0, 0), GLUT_BITMAP_HELVETICA_18 );

    Initialized = true;
    Finished = false;
}

void Snake3DPaused::DeInit()
{
	if (Initialized)
	{
		delete PauseText;

        Initialized = false;
	}
}

void Snake3DPaused::Update( float ElapsedTime )
{
	// Get reference to pressed key set
	set<unsigned char> &PressedSet = Singleton<GLUTApp>::Instance().GetPressedKeys();

    // Unpause on U key press
	if (PressedSet.count( 'u' ) > 0)
		Finished = true;
}

void Snake3DPaused::Render() const
{
	// Render pause text
	Singleton<GLUTApp>::Instance().RenderText( PauseText );
}

bool Snake3DPaused::IsFinished()
{	
    return Finished;
}


// ------------------------------------------------------------------------------------
// ------------------------------Snake3DGameOver Members-------------------------------
// ------------------------------------------------------------------------------------

// Set type ID for IGameState factory
FactoryRegistrar<IGameState, Snake3DGameOver, std::string> Snake3DGameOver::Registrar( "Snake3DGameOver" );

Snake3DGameOver::Snake3DGameOver()
{
	Initialized = false;
}

Snake3DGameOver::~Snake3DGameOver()
{
	DeInit();
}

void Snake3DGameOver::Init()
{
	int WindowWidth = Singleton<GLUTApp>::Instance().GetWindowWidth(),
		WindowHeight = Singleton<GLUTApp>::Instance().GetWindowHeight();
	GameOverText = new RenderTextData( "Game Over! Press space to play again. Press escape to exit.",
                                       Vector2f(WindowWidth*0.5f - 250, WindowHeight*0.5f), Color3f(1, 0, 0),
                                       GLUT_BITMAP_HELVETICA_18 );

    Initialized = true;
    Finished = false;
}

void Snake3DGameOver::DeInit()
{
	if (Initialized)
	{
		delete GameOverText;

        Initialized = false;
	}
}

void Snake3DGameOver::Update( float ElapsedTime )
{
	// Get reference to pressed key set
	set<unsigned char> &PressedSet = Singleton<GLUTApp>::Instance().GetPressedKeys();

    // Restart game on space key press
	if (PressedSet.count( ' ' ) > 0)
	{
		Finished = true;
		Singleton<GLUTApp>::Instance().PushState( "Snake3DGameWorld" );
	}
}

void Snake3DGameOver::Render() const
{
	// Render game over text
	Singleton<GLUTApp>::Instance().RenderText( GameOverText );
}

bool Snake3DGameOver::IsFinished()
{	
    return Finished;
}