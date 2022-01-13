#ifndef SNAKE_3D_OBJECTS_H
#define SNAKE_3D_OBJECTS_H



// ------------------------------------------------------------------------------------
// ----------------------------------Included headers----------------------------------
// ------------------------------------------------------------------------------------

// STL
#include <list>

// Windows/OpenGL
#include <Windows.h>
#include <gl/GL.h>

// Utilities
#include "Utilities\Matrix.h"


// ------------------------------------------------------------------------------------
// ---------------------------------------Classes--------------------------------------
// ------------------------------------------------------------------------------------

class SnakeSegment
{
public:
    // Constructors
    SnakeSegment( const Vector3f &Position, float Size, const Color3f &Color );
    SnakeSegment( const Vector3f &Position, float Size );

    // Accessors
    inline const Vector3f &GetPosition() const;
    inline float GetSize() const;
    inline const Color3f &GetColor() const;

    // Modifiers
    inline void SetPosition( const Vector3f &position );
    inline void SetSize( float size);
    inline void SetColor( const Color3f &color );

    // Methods
    static bool Intersect( const SnakeSegment *s1, const SnakeSegment *s2 );
    void Render() const;

private:
    Vector3f Position;
    float Size;
    Color3f Color;
};


class Snake
{
public:
    // Constructors
    Snake( const Vector3f &HeadPosition, const Vector3f &Heading, float MoveInterval, int NumSegments, float SegmentSize );
    ~Snake();

    // Accessors
    inline const Vector3f &GetPosition() const;
    inline const Vector3f &GetHeading() const;
    inline float GetSegmentSize() const;

    // Methods
    void Update( float ElapsedTime );
    void Render() const;
    void RotateHeading( const Vector3f &Rotation );
    void IncreaseLength();
    bool IsSelfColliding() const;

private:
    Vector3f Heading, Up, Right;
    float MoveInterval, SegmentSize;
    std::list<SnakeSegment *> Segments;
    float ElapsedSinceMove;


	float GetInterpolationCoeff( int i );
};


// ------------------------------------------------------------------------------------
// -----------------------------Inline function definitions----------------------------
// ------------------------------------------------------------------------------------

// --------------------------------SnakeSegment members--------------------------------

const Vector3f &SnakeSegment::GetPosition() const
{
    return Position;
}

float SnakeSegment::GetSize() const
{
    return Size;
}

const Color3f &SnakeSegment::GetColor() const
{
    return Color;
}

void SnakeSegment::SetPosition( const Vector3f &position )
{
    Position = position;
}

void SnakeSegment::SetSize( float size)
{
    Size = size;
}

void SnakeSegment::SetColor( const Color3f &color )
{
    Color = color;
}


// ------------------------------------Snake members-----------------------------------

const Vector3f &Snake::GetPosition() const
{
    return Segments.front()->GetPosition();
}

const Vector3f &Snake::GetHeading() const
{
    return Heading;
}

float Snake::GetSegmentSize() const
{
    return SegmentSize;
}



#endif