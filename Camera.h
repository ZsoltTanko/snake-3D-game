#ifndef CAMERA_H
#define CAMERA_H



// ------------------------------------------------------------------------------------
// ----------------------------------Included headers----------------------------------
// ------------------------------------------------------------------------------------

// Utilities
#include "Utilities\Matrix.h"


// ------------------------------------------------------------------------------------
// ---------------------------------------Classes--------------------------------------
// ------------------------------------------------------------------------------------

class Camera
{
public:
    // Constructors
    Camera( const Vector3f &Position, const Vector3f &Look, float FOVY, float NearClip, float FarClip );

    // Accessors
    inline const Vector3f &GetPosition() const;
    inline const Vector3f &GetLook() const;
    inline float GetFOVY() const;
    inline float GetNearClip() const;
    inline float GetFarClip() const;

    // Modifiers
    inline void SetPosition( const Vector3f &position );
    inline void SetLook( const Vector3f &look );
    inline void SetFOVY( float FOVy );
    inline void SetNearClip( float nearClip );
    inline void SetFarClip( float farClip );

    // Methods
    void Rotate( const Vector3f &rotation );
    void Translate( const Vector3f &translation );
    void ApplyGLViewMatrix() const;

private:
    Vector3f Position, Look, Up, Right;;
    float FOVY, NearClip, FarClip;
};


// ------------------------------------------------------------------------------------
// -----------------------------Inline function definitions----------------------------
// ------------------------------------------------------------------------------------

const Vector3f &Camera::GetPosition() const
{
    return Position;
}

const Vector3f &Camera::GetLook() const
{
    return Look;
}

float Camera::GetFOVY() const
{
    return FOVY;
}

float Camera::GetNearClip() const
{
    return NearClip;
}

float Camera::GetFarClip() const
{
    return FarClip;
}

void Camera::SetPosition( const Vector3f &position )
{
    Position = position;
}

void Camera::SetLook( const Vector3f &look )
{
    Look = look;
}

void Camera::SetFOVY( float FOVy )
{
    FOVY = FOVy;
}

void Camera::SetNearClip( float nearClip )
{
    NearClip = nearClip;
}

void Camera::SetFarClip( float farClip )
{
    FarClip = farClip;
}



#endif