// ------------------------------------------------------------------------------------
// ----------------------------------Included headers----------------------------------
// ------------------------------------------------------------------------------------
#include "Camera.h"

// Windows/OpenGL
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

// Utilities
#include "Utilities\Matrix.h"


// ------------------------------------------------------------------------------------
// ------------------------------------Camera Members----------------------------------
// ------------------------------------------------------------------------------------

Camera::Camera( const Vector3f &Position, const Vector3f &Look, float FOVY, float NearClip, float FarClip )
        : Position(Position), Look(Look), FOVY(FOVY), NearClip(NearClip), FarClip(FarClip), Up(0, 1, 0)
{
    // Calculate right vector
	VectorCross( Up, Look, Right );
}

void Camera::Rotate( const Vector3f &Rotation )
{
    Matrix3f RotationMatrix;

    // Create rotation matrix about the right vector, and rotate the look & up vectors
	CreateRotationMatrixAxis( Right, Rotation.x(), RotationMatrix );
    VectorMultiply( Look, RotationMatrix, Look );
    VectorMultiply( Up, RotationMatrix, Up );

    // Create rotation matrix about the up vector, and rotate the look & right vectors
	CreateRotationMatrixAxis( Up, Rotation.y(), RotationMatrix );
    VectorMultiply( Look, RotationMatrix, Look );
    VectorMultiply( Right, RotationMatrix, Right );
}

void Camera::Translate( const Vector3f &translation )
{
    Position += translation;
}

void Camera::ApplyGLViewMatrix() const
{
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    gluLookAt( Position.x(), Position.y(), Position.z(),
               Position.x() + Look.x(), Position.y() + Look.y(), Position.z() + Look.z(),
               Up.x(), Up.y(), Up.z() );
}