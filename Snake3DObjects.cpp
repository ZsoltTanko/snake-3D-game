// ------------------------------------------------------------------------------------
// ----------------------------------Included headers----------------------------------
// ------------------------------------------------------------------------------------
#include "Snake3DObjects.h"

// STL
#include <list>
using namespace std;

// Windows/OpenGL
#include <Windows.h>
#include <gl/GL.h>
#include <gl/glut.h>

// Utilities
#include "Utilities\Matrix.h"
#include "Utilities\Rand Utilities.h"


// ------------------------------------------------------------------------------------
// --------------------------------SnakeSegment Members--------------------------------
// ------------------------------------------------------------------------------------

SnakeSegment::SnakeSegment( const Vector3f &Position, float Size, const Color3f &Color )
: Position(Position), Size(Size), Color(Color)
{
}
SnakeSegment::SnakeSegment( const Vector3f &Position, float Size )
: Position(Position), Size(Size), Color(RandomMatrix<3, 1, float>(0, 1))
{
}

bool SnakeSegment::Intersect( const SnakeSegment *s1, const SnakeSegment *s2 )
{
    return (s1->GetPosition() - s2->GetPosition()).GetMagnitudeSqr() <= TMath::Sqr(s1->GetSize() + s2->GetSize());
}

void SnakeSegment::Render() const
{
    glColor3fv( (const float *)&Color );

    glPushMatrix();

    glTranslatef( Position.x(), Position.y(), Position.z() );

    glutSolidSphere( Size, 15, 5 );

    glPopMatrix();
}


// ------------------------------------------------------------------------------------
// ------------------------------------Snake Members-----------------------------------
// ------------------------------------------------------------------------------------

Snake::Snake( const Vector3f &HeadPosition, const Vector3f &Heading, float MoveInterval, int NumSegments, float SegmentSize )
: Heading(Heading), MoveInterval(MoveInterval), SegmentSize(SegmentSize), Up(0, 1, 0)
{
    ElapsedSinceMove = 0;

    // Assure heading is a unit vector and calculate right vector
    this->Heading.Normalize();
	VectorCross( Up, Heading, Right );

    // Create segments
    for (int i = 0; i < NumSegments; i++)
        Segments.push_back( new SnakeSegment( -Heading * i, SegmentSize ) );
}

Snake::~Snake()
{
    for (list<SnakeSegment *>::iterator it = Segments.begin(); it != Segments.end(); ++it)
        delete *it;
}

void Snake::Update( float ElapsedTime )
{
    ElapsedSinceMove += ElapsedTime;

    // If enough time has passed to make a move
    if (ElapsedSinceMove >= MoveInterval)
    {
        // Remove the end segment
        SnakeSegment *tail = Segments.back();
        Segments.pop_back();

        // Reset the tail segment to be the new head
        tail->SetPosition( Segments.front()->GetPosition() + Heading );

        // Add new head
        Segments.push_front( tail );

        ElapsedSinceMove = 0;
    }

    // Colors to interpolate between
    Color3f StartColor(0.463f, 0.282f, 0), // Brown
            EndColor(0, 1, 0); // Green

    // Sizes to interpolate between
    float StartSize = SegmentSize * 1.25f,
          EndSize = StartSize * 2;

    // Interpolate segment color and size based on position within the snake
    int i = 0;
    for (list<SnakeSegment *>::iterator it = Segments.begin(); it != Segments.end(); ++it, i++)
    {
        float x = GetInterpolationCoeff( i );

        (*it)->SetColor( TMath::CosineInterpolate( x, StartColor, EndColor ) );
        (*it)->SetSize( TMath::CosineInterpolate( x, StartSize, EndSize ) );
    }
}

float Snake::GetInterpolationCoeff( int i )
{
    float x = i/20.0f;
    
	x = x - (int)x;
    
	if (x > 0.5f)
        x = 1-x;

    x *= 2;

	return x;
}

void Snake::Render() const
{
    // Render segments
    for (list<SnakeSegment *>::const_iterator it = Segments.begin(); it != Segments.end(); ++it)
        (*it)->Render();
}

void Snake::RotateHeading( const Vector3f &Rotation )
{
    Matrix3f RotationMatrix;

    // Create rotation matrix about the right vector, and rotate the heading & up vectors
	CreateRotationMatrixAxis( Right, Rotation.x(), RotationMatrix );
    VectorMultiply( Heading, RotationMatrix, Heading );
    VectorMultiply( Up, RotationMatrix, Up );

    // Create rotation matrix about the up vector, and rotate the heading & right vectors
	CreateRotationMatrixAxis( Up, Rotation.y(), RotationMatrix );
    VectorMultiply( Heading, RotationMatrix, Heading );
    VectorMultiply( Right, RotationMatrix, Right );
}

void Snake::IncreaseLength()
{
    // Add new segments to the end of the snake
	for (int i = 0; i < 20; i++)
		Segments.push_back( new SnakeSegment( Segments.back()->GetPosition(), 0 ) );
}

bool Snake::IsSelfColliding() const
{
    SnakeSegment * const Head = Segments.front();
    
    // Prevent head collision with self or the next few segments
    list<SnakeSegment *>::const_iterator it = Segments.begin();
    for (int i = 0; i < 4; i++)
        ++it;

    // Iterate through segments, checking for collisions with head
    for (; it != Segments.end(); ++it)
    {
        if (SnakeSegment::Intersect( Head, *it ))
            return true;
    }

    return false;
}