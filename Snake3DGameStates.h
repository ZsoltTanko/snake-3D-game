#ifndef SNAKE3DGAMESTATES_H
#define SNAKE3DGAMESTATES_H



// ------------------------------------------------------------------------------------
// ----------------------------------Included headers----------------------------------
// ------------------------------------------------------------------------------------

// STL
#include <queue>
#include <set>

// Utilities
#include "Utilities\Factory.h"
#include "Utilities\Matrix.h"
#include "Utilities\Timer.h"

#include "IGameState.h"


// ------------------------------------------------------------------------------------
// ---------------------------------------Classes--------------------------------------
// ------------------------------------------------------------------------------------

class Snake;
class SnakeSegment;
class Camera;
class Snake3DGameWorld : public IGameState
{
public:
    Snake3DGameWorld();
    ~Snake3DGameWorld();

    void Init();
    void DeInit();

    void Update( float ElapsedTime );
    void Render() const;

    bool IsFinished();

private:
    // IGameState factory registrar
    static FactoryRegistrar<IGameState, Snake3DGameWorld, std::string> Registrar;

    Snake *snake;
    SnakeSegment *snakeFood;
	Camera *camera;
    float EnvSphereSize;

    bool Initialized, Paused, Finished;
    PerformanceTimer PauseTimer;


    // User input processing
    void ProcessMouseMotion( std::queue<Vector2f> &MotionQueue );
    void ProcessKeys( std::set<unsigned char> &PressedSet );

    // Misc utility methods
    void GameOver();
};

struct RenderTextData;
class Snake3DPaused : public IGameState
{
public:
    Snake3DPaused();
    ~Snake3DPaused();

    void Init();
    void DeInit();

    void Update( float ElapsedTime );
    void Render() const;

    bool IsFinished();

private:
    // IGameState factory registrar
    static FactoryRegistrar<IGameState, Snake3DPaused, std::string> Registrar;

	RenderTextData *PauseText;
    bool Initialized, Finished;
};

class Snake3DGameOver : public IGameState
{
public:
    Snake3DGameOver();
    ~Snake3DGameOver();

    void Init();
    void DeInit();

    void Update( float ElapsedTime );
    void Render() const;

    bool IsFinished();

private:
    // IGameState factory registrar
    static FactoryRegistrar<IGameState, Snake3DGameOver, std::string> Registrar;

	RenderTextData *GameOverText;
    bool Initialized, Finished;
};



#endif