#ifndef MATRIX_H
#define MATRIX_H



// TODO: Upgrade many basic math functions to take 2 different types
//       Create template for conversion-to-higher-type for return values (check Alexandrescu's book)
//       Consider using template metaprogramming to unroll loops in many of Matrix's member functions, might be faster
//       Consider breaking vector & matrix into separate classes. In how many cases is it required to be able to treat the two as the same type?
//       Public/private scope of Add/RemoveDimension methods
//       Determinant? Eigenvalues/vectors? Vector projection?
//       Rename variables in binary calculations to "lhs" and "rhs"
//       Figure out overloaded operators, currently everything is done component wise
//       Make sure all functions that were removed from Matrix can be passed the same parameters for v and vOut
//       Should calls to the access operators be made internally?
//       Check matrix sizes on the Vector & Matrix functions, might be able to generalize some
//       Assure function definitions & declarations match up, compiler won't catch this as templatized function definitions in the header can also serve as declarations
//       Check commenting - Make sure to note matrix size restrictions of functions
//       Test EVERYTHING


// ------------------------------------------------------------------------------------
// ----------------------------------Included headers----------------------------------
// ------------------------------------------------------------------------------------
#include <cassert>
#include <limits>
#include <string>
#include <memory>

// Utilities
#include "Template Utils.h"
#include "TMath.h"

// Undefine max & min macros so numeric_limits<T>::max/min work
#undef max
#undef min


// ------------------------------------------------------------------------------------
// --------------------Templatized class declarations & definitions--------------------
// ------------------------------------------------------------------------------------

// --------------------NxM matrix--------------------
// Data is stored & accessed [row][column], N = rows, M = columns
template<unsigned int N, unsigned int M, typename T = TMath::FLOATTYPE>
class Matrix
{
public:
    static const unsigned int Rows = N;
    static const unsigned int Columns = M;
    static const bool IsVector = M == 1;
    static const bool IsSquare = !IsVector && N == M;


    // ----------------------------------Access grants-----------------------------------

    // Index operators
    inline T &operator [] (const unsigned int i);
    inline const T &operator [] (const unsigned int i) const;

    // Access operators
    inline T &operator () (const unsigned int iRow, const unsigned int iColumn);
    inline const T &operator () (const unsigned int iRow, const unsigned int iColumn) const;

    // Vector access functions
    inline T &x();
    inline T &y();
    inline T &z();
    inline T &w();
    inline const T &x() const;
    inline const T &y() const;
    inline const T &z() const;
    inline const T &w() const;

    // Matrix set/get functions
    inline void GetRow(const unsigned int iRow, Matrix<N, 1, T> &vOut) const;
    inline const Matrix<N, 1, T> &GetRowRef(const unsigned int iRow) const;
    inline void GetColumn(const unsigned int iColumn, Matrix<N, 1, T> &vOut) const;
    inline void GetXVector(Matrix<N, 1, T> &vOut) const;
    inline void GetYVector(Matrix<N, 1, T> &vOut) const;
    inline void GetZVector(Matrix<N, 1, T> &vOut) const;
    void SetRow(const unsigned int iRow, const Matrix<N, 1, T> &v);
    void SetColumn(const unsigned int iColumn, const Matrix<M, 1, T> &v);
    inline void SetXVector(const Matrix<N, 1, T> &v);
    inline void SetYVector(const Matrix<N, 1, T> &v);
    inline void SetZVector(const Matrix<N, 1, T> &v);


    // -----------------------------Constructor declarations-----------------------------

    // Default constructor - Empty
    inline Matrix();
    // Copy constructor
    inline Matrix(const Matrix &m);
    // Constructor - Set to single value
    inline Matrix(const T t);

    // Vector constructors - Set values for 2, 3 and 4 dimensional vectors
    inline Matrix(const T x, const T y);
    inline Matrix(const T x, const T y, const T z);
    inline Matrix(const T x, const T y, const T z, const T w);

    // Matrix constructor - Set columns
    inline Matrix(const Matrix<N, 1, T> &vX, const Matrix<N, 1, T> &vY, const Matrix<N, 1, T> &vZ);
    


    // -------------------------------Overloaded operators-------------------------------

    // Unary math operators
    inline Matrix operator + ();
    inline Matrix operator + () const;
    inline Matrix operator - ();
    inline Matrix operator - () const;

    // Binary math operators - Matrix-Matrix
    inline Matrix operator + (const Matrix &rhs) const;
    inline Matrix operator - (const Matrix &rhs) const;
    inline Matrix operator * (const Matrix &rhs) const;
    inline Matrix operator / (const Matrix &rhs) const;

    // Binary math operators - Matrix-T
    inline Matrix operator + (const T t) const;
    inline Matrix operator - (const T t) const;
    inline Matrix operator * (const T t) const;
    inline Matrix operator / (const T t) const;

    // Assignment operators - Matrix-Matrix
    inline Matrix &operator += (const Matrix &rhs);
    inline Matrix &operator -= (const Matrix &rhs);
    inline Matrix &operator *= (const Matrix &rhs);
    inline Matrix &operator /= (const Matrix &rhs);
        
    // Assignment operators - Matrix-T
    inline Matrix &operator = (const T rhs);
    inline Matrix &operator += (const T rhs);
    inline Matrix &operator -= (const T rhs);
    inline Matrix &operator *= (const T rhs);
    inline Matrix &operator /= (const T rhs);

    // Boolean operators
    bool operator == (const Matrix &rhs) const;
    bool operator != (const Matrix &rhs) const;
    bool operator > (const Matrix &rhs) const;
    bool operator < (const Matrix &rhs) const;
    bool operator >= (const Matrix &rhs) const;
    bool operator <= (const Matrix &rhs) const;


    // ---------------------------------Vector functions---------------------------------

    // Get squared vector magnitude
    T GetMagnitudeSqr() const;

    // Get vector magnitude
    inline T GetMagnitude() const;

    // Set vector magnitude
    void SetMagnitude(const T tMagnitude);

    // Normalize vector
    inline void Normalize();


    // ---------------------------------Matrix functions---------------------------------

    // Elementary row operations
    void SwapRows(const unsigned int iRow1, const unsigned int iRow2);
    void ScaleRow(const unsigned int iRow, const T tScalar);
    void AddScaledRow(const unsigned int iScaledRow, const T tScalar, const unsigned int iResultRow);


    // Note: Matrix must be square for all following matrix functions

    // Set identity matrix
    void SetIdentity();

    // Transpose
    void Transpose();

    // Normalize & make vectors (columns) orthogonal in a 3x3 matrix
    void Orthonormalize();


    // ----------------------------------Misc functions----------------------------------

    // Return total of all entries in matrix
    inline T Sum() const;

    // Abs matrix to this
    inline void Absolute();

    // Floor matrix
    inline void Floor();

    // Return maximum entry in matrix
    T Max() const;

    // Return minimum entry in matrix
    T Min() const;


    // -----------------------------Private vector functions-----------------------------

    // Return this vector with an added dimension set to 1
    inline void VectorAddDimention(Matrix<N+1, 1, T> &vOut) const;

    // Return this vector with the last dimension removed
    inline void VectorRemoveDimention(Matrix<N-1, 1, T> &vOut) const;
private:
    T pData[N*M];


    // -----------------------------Private access functions-----------------------------

    inline Matrix<N, 1, T> &GetRowRef(const unsigned int iRow);
    inline Matrix &Data();
    inline const Matrix &Data() const;
    inline T &Data(const unsigned int iRow, const unsigned int iColumn);
    inline const T &Data(const unsigned int iRow, const unsigned int iColumn) const;



    // -----------------------------Private matrix functions-----------------------------

    // Return this matrix with an added dimension set to identity
    void MatrixAddDimention(Matrix<N+1, M+1, T> &mOut) const;

    // Return this matrix with the last dimension removed
    void MatrixRemoveDimention(Matrix<N-1, M-1, T> &mOut) const;
};

// Various typedefs for common matrices
typedef Matrix<2, 2, float> Matrix2f;
typedef Matrix<3, 3, float> Matrix3f;
typedef Matrix<4, 4, float> Matrix4f;
typedef Matrix<2, 1, float> Vector2f;
typedef Matrix<3, 1, float> Vector3f;
typedef Matrix<4, 1, float> Vector4f;
typedef Matrix<3, 1, float> Color3f;
typedef Matrix<4, 1, float> Color4f;
typedef Matrix<2, 1, float> TexCoordf;
typedef Matrix<2, 2, double> Matrix2lf;
typedef Matrix<3, 3, double> Matrix3lf;
typedef Matrix<4, 4, double> Matrix4lf;
typedef Matrix<2, 1, double> Vector2lf;
typedef Matrix<3, 1, double> Vector3lf;
typedef Matrix<4, 1, double> Vector4lf;
typedef Matrix<3, 1, double> Color3lf;
typedef Matrix<4, 1, double> Color4lf;
typedef Matrix<2, 1, double> TexCoordlf;


// ------------------------------------------------------------------------------------
// -------------------Inline & templatized function declarations-----------------------
// ------------------------------------------------------------------------------------

// ---------------------------------Vector functions---------------------------------

// Calculate cross product of v1 & v2 and place result in this. Note matrix must be 3x1
template <typename T>
void VectorCross(const Matrix<3, 1, T> &v1, const Matrix<3, 1, T> &v2, Matrix<3, 1, T> &vOut);

// Calculate dot product of this & m
template <unsigned int N, typename T>
T VectorDot(const Matrix<N, 1, T> &v1, const Matrix<N, 1, T> &v2);

// Calculate squared distance between vectors
template <unsigned int N, typename T>
T VectorDistanceSqr(const Matrix<N, 1, T> &v1, const Matrix<N, 1, T> &v2);

// Calculate distance between vectors
template <unsigned int N, typename T>
inline T VectorDistance(const Matrix<N, 1, T> &v1, const Matrix<N, 1, T> &v2);

// Calculate reflection vector
template <unsigned int N, typename T>
void VectorReflect(const Matrix<N, 1, T> &vIncident, const Matrix<N, 1, T> &vNormal, Matrix<N, 1, T> &vOut);

// Multiply Nx1 vector by NxN matrix
template <unsigned int N, typename T>
void VectorMultiply(const Matrix<N, 1, T> &v, const Matrix<N, N, T> &m, Matrix<N, 1, T> &vOut);

// Multiply Nx1 vector by (N+1)x(N+1) matrix
template <unsigned int N, typename T>
void VectorMultiply(const Matrix<N, 1, T> &v, const Matrix<N+1, N+1, T> &m, Matrix<N, 1, T> &vOut);


// ---------------------------------Matrix functions---------------------------------

// Multiply matrices m1 & m2 in the order m1 x m2
template <unsigned int N, unsigned int M, unsigned int P, typename T>
void MatrixMultiply(const Matrix<N, M, T> &m1, const Matrix<M, P, T> &m2, Matrix<N, P, T> &mOut);

// Invert m and place result in mOut, returns false if matrix is singular
template <unsigned int N, typename T>
bool MatrixInvert(const Matrix<N, N, T> &m, Matrix<N, N, T> &mOut);

// Check if matrix is identity
template <unsigned int N, typename T>
bool MatrixIsIdentity(const Matrix<N, N, T> &m);


// --------------------------------Creation functions--------------------------------

// Generate a translation matrix
template <unsigned int N, typename T>
void CreateTranslationMatrix(const Matrix<N-1, 1, T> &vTranslation, Matrix<N, N, T> &mOut);

// Generate a scaling matrix
template <unsigned int N, typename T>
void CreateScalingMatrix(const Matrix<N-1, 1, T> &vScale, Matrix<N, N, T> &mOut);

// Generate a rotation matrix about the X axis
template <unsigned int N, typename T>
void CreateRotationMatrixX(const T tAngle, Matrix<N, N, T> &mOut);

// Generate a rotation matrix about the Y axis
template <unsigned int N, typename T>
void CreateRotationMatrixY(const T tAngle, Matrix<N, N, T> &mOut);

// Generate a rotation matrix about the Z axis, note can also generate 2x2 rotation matrices for rotating 2D vectors 
template <unsigned int N, typename T>
void CreateRotationMatrixZ(const T tAngle, Matrix<N, N, T> &mOut);

// Generate a rotate matrix about the XYZ axes, note rotations are applied in the order Z-X-Y
template <unsigned int N, typename T>
void CreateRotationMatrixXYZ(const Matrix<3, 1, T> &v, Matrix<N, N, T> &mOut);

// Generate a rotation matrix about an arbitrary axis, note vAxis must be unit length
template <unsigned int N, typename T>
void CreateRotationMatrixAxis(const Matrix<3, 1, T> &vAxis, const T tAngle, Matrix<N, N, T> &mOut);

// Generate view matrix from position, look, & up vectors. Note look & up vectors must be unit length
template <unsigned int N, typename T>
void CreateViewMatrix(const Matrix<3, 1, T> &vPosition, const Matrix<3, 1, T> &vLook, const Matrix<3, 1, T> &vUp, Matrix<N, N, T> &mOut);

// Generate perspective matrix
template <unsigned int N, typename T>
void CreatePerspectiveMatrix(const float fFOVY, const float fAspectRatio, const float fNearClip, const float fFarClip, Matrix<N, N, T> &mOut);


// ------------------------------------------------------------------------------------
// -----------------Inline & templatized member function definitions-------------------
// ------------------------------------------------------------------------------------

// ----------------------------------Access grants-----------------------------------

// Index operators
template <unsigned int N, unsigned  int M, typename T>
T &Matrix<N, M, T>::operator [] (const unsigned int i)
{
    assert(i < N*M && i >= 0);
    return pData[i];
}

template <unsigned int N, unsigned  int M, typename T>
const T &Matrix<N, M, T>::operator [] (const unsigned int i) const
{
    assert(i < N*M && i >= 0);
    return pData[i];
}

// Access operators
template <unsigned int N, unsigned  int M, typename T>
T &Matrix<N, M, T>::operator () (const unsigned int iRow, const unsigned int iColumn)
{
    STATIC_CHECK(!IsVector, MUST_BE_MATRIX);
    assert(iRow < N && iRow >= 0 && iColumn < M && iColumn >= 0);
    return pData[iRow*M+iColumn];
}
template <unsigned int N, unsigned  int M, typename T>
const T &Matrix<N, M, T>::operator () (const unsigned int iRow, const unsigned int iColumn) const
{
    STATIC_CHECK(!IsVector, MUST_BE_MATRIX);
    assert(iRow < N && iRow >= 0 && iColumn < M && iColumn >= 0);
    return pData[iRow*M+iColumn];
}

// Vector access functions
template <unsigned int N, unsigned  int M, typename T>
inline T &Matrix<N, M, T>::x()
{
    STATIC_CHECK(IsVector, MUST_BE_VECTOR);
    return pData[0]; 
}
template <unsigned int N, unsigned  int M, typename T>
inline T &Matrix<N, M, T>::y()
{
    STATIC_CHECK(IsVector, MUST_BE_VECTOR);
    STATIC_CHECK(N > 1, VECTOR_MUST_HAVE_MORE_THAN_1_DIMENSION);
    return pData[1];
}
template <unsigned int N, unsigned  int M, typename T>
inline T &Matrix<N, M, T>::z()
{
    STATIC_CHECK(IsVector, MUST_BE_VECTOR);
    STATIC_CHECK(N > 2, VECTOR_MUST_HAVE_MORE_THAN_2_DIMENSIONS);
    return pData[2];
}
template <unsigned int N, unsigned  int M, typename T>
inline T &Matrix<N, M, T>::w()
{
    STATIC_CHECK(IsVector, MUST_BE_VECTOR);
    STATIC_CHECK(N > 3, VECTOR_MUST_HAVE_MORE_THAN_3_DIMENSIONS);
    return pData[3];
}
template <unsigned int N, unsigned  int M, typename T>
inline const T &Matrix<N, M, T>::x() const
{
    STATIC_CHECK(IsVector, MUST_BE_VECTOR);
    return pData[0];
}
template <unsigned int N, unsigned  int M, typename T>
inline const T &Matrix<N, M, T>::y() const
{
    STATIC_CHECK(IsVector, MUST_BE_VECTOR);
    STATIC_CHECK(N > 1, VECTOR_MUST_HAVE_MORE_THAN_1_DIMENSION);
    return pData[1];
}
template <unsigned int N, unsigned  int M, typename T>
inline const T &Matrix<N, M, T>::z() const
{
    STATIC_CHECK(IsVector, MUST_BE_VECTOR);
    STATIC_CHECK(N > 2, VECTOR_MUST_HAVE_MORE_THAN_2_DIMENSIONS);
    return pData[2];
}
template <unsigned int N, unsigned  int M, typename T>
inline const T &Matrix<N, M, T>::w() const
{
    STATIC_CHECK(IsVector, MUST_BE_VECTOR);
    STATIC_CHECK(N > 3, VECTOR_MUST_HAVE_MORE_THAN_3_DIMENSIONS);
    return pData[3];
}


// Matrix access/set functions
template <unsigned int N, unsigned  int M, typename T>
inline void Matrix<N, M, T>::GetRow(const unsigned int iRow, Matrix<N, 1, T> &vOut) const
{
    assert(iRow >= 0 && iRow < N);
    for (unsigned int i = 0; i < N; i++)
        vOut[i] = Data(iRow, i);
}

template <unsigned int N, unsigned  int M, typename T>
inline Matrix<N, 1, T> &Matrix<N, M, T>::GetRowRef(const unsigned int iRow)
{
    assert(iRow >= 0 && iRow < N);
    return *(reinterpret_cast<Matrix<N, 1, T> *>(this)+iRow);
}

template <unsigned int N, unsigned  int M, typename T>
inline void Matrix<N, M, T>::GetColumn(const unsigned int iColumn, Matrix<N, 1, T> &vOut) const
{
    assert(iColumn >= 0 && iColumn < M);
    for (unsigned int i = 0; i < M; i++)
        vOut[i] = Data(i, iColumn);
}

template <unsigned int N, unsigned  int M, typename T>
inline void Matrix<N, M, T>::GetXVector(Matrix<N, 1, T> &vOut) const
{
    for (unsigned int i = 0; i < M; i++)
        vOut[i] = Data(i, 0);
}

template <unsigned int N, unsigned  int M, typename T>
inline void Matrix<N, M, T>::GetYVector(Matrix<N, 1, T> &vOut) const
{
    STATIC_CHECK(!IsVector, CANNOT_BE_VECTOR);
    STATIC_CHECK(M > 1, NUMBER_OF_ROWS_MUST_BE_GREATER_THAN_1);
    for (unsigned int i = 0; i < M; i++)
        vOut[i] = Data(i, 1);
}

template <unsigned int N, unsigned  int M, typename T>
inline void Matrix<N, M, T>::GetZVector(Matrix<N, 1, T> &vOut) const
{
    STATIC_CHECK(!IsVector, CANNOT_BE_VECTOR);
    STATIC_CHECK(M > 2, NUMBER_OF_ROWS_MUST_BE_GREATER_THAN_2);
    for (unsigned int i = 0; i < M; i++)
        vOut[i] = Data(i, 2);
}

template <unsigned int N, unsigned  int M, typename T>
void Matrix<N, M, T>::SetRow(const unsigned int iRow, const Matrix<N, 1, T> &v)
{
    assert(iRow >= 0 && iRow < N);
    for (unsigned int i = 0; i < N; i++)
        Data(iRow, i) = v[i];
}

template <unsigned int N, unsigned  int M, typename T>
void Matrix<N, M, T>::SetColumn(const unsigned int iColumn, const Matrix<M, 1, T> &v)
{
    assert(iColumn >= 0 && iColumn < M);
    for (unsigned int i = 0; i < M; i++)
        Data(i, iColumn) = v[i];
}

template <unsigned int N, unsigned  int M, typename T>
inline void Matrix<N, M, T>::SetXVector(const Matrix<N, 1, T> &v)
{
    SetColumn(0, v);
}

template <unsigned int N, unsigned  int M, typename T>
inline void Matrix<N, M, T>::SetYVector(const Matrix<N, 1, T> &v)
{
    STATIC_CHECK(!IsVector, CANNOT_BE_VECTOR);
    STATIC_CHECK(M > 1, NUMBER_OF_ROWS_MUST_BE_GREATER_THAN_1);
    SetColumn(1, v);
}

template <unsigned int N, unsigned  int M, typename T>
inline void Matrix<N, M, T>::SetZVector(const Matrix<N, 1, T> &v)
{
    STATIC_CHECK(!IsVector, CANNOT_BE_VECTOR);
    STATIC_CHECK(M > 2, NUMBER_OF_ROWS_MUST_BE_GREATER_THAN_2);
    SetColumn(2, v);
}


// ------------------------------Constructor definitions-----------------------------

// General constructors

// Default constructor - empty
template <unsigned int N, unsigned  int M, typename T>
inline Matrix<N, M, T>::Matrix() {}

// Copy constructor
template <unsigned int N, unsigned  int M, typename T>
inline Matrix<N, M, T>::Matrix(const Matrix &m)
{
    memcpy(this, &m, N*M*sizeof(T));
}

// Constructor - set to single value
template <unsigned int N, unsigned  int M, typename T>
inline Matrix<N, M, T>::Matrix(const T t)
{
    for (unsigned int i = 0; i < N*M; i++)
        pData[i] = t;
}


// Vector constructors

// Set 2D vector
template <unsigned int N, unsigned  int M, typename T>
inline Matrix<N, M, T>::Matrix(const T x, const T y)
{
    STATIC_CHECK(IsVector && N == 2, MUST_BE_2_DIMENSIONAL_VECTOR);

    pData[0] = x;
    pData[1] = y;
}
// Set 3D vector
template <unsigned int N, unsigned  int M, typename T>
inline Matrix<N, M, T>::Matrix(const T x, const T y, const T z)
{
    STATIC_CHECK(IsVector && N == 3, MUST_BE_3_DIMENSIONAL_VECTOR);

    pData[0] = x;
    pData[1] = y;
    pData[2] = z;
}
// Set 4D vector
template <unsigned int N, unsigned  int M, typename T>
inline Matrix<N, M, T>::Matrix(const T x, const T y, const T z, const T w)
{
    STATIC_CHECK(IsVector && N == 4, MUST_BE_4_DIMENSIONAL_VECTOR);

    pData[0] = x;
    pData[1] = y;
    pData[2] = z;
    pData[3] = w;
}

// Matrix constructors
template <unsigned int N, unsigned  int M, typename T>
inline Matrix<N, M, T>::Matrix(const Matrix<N, 1, T> &vX, const Matrix<N, 1, T> &vY, const Matrix<N, 1, T> &vZ)
{
    STATIC_CHECK(N == 3 && M == 3, MUST_BE_3_BY_3_MATRIX);

    SetXVector(vX);
    SetYVector(vY);
    SetZVector(vZ);
}


// -------------------------------Overloaded operators-------------------------------

// Unary math operators
template <unsigned int N, unsigned  int M, typename T>
inline Matrix<N, M, T> Matrix<N, M, T>::operator + ()
{
    return Data();
}
template <unsigned int N, unsigned  int M, typename T>
inline Matrix<N, M, T> Matrix<N, M, T>::operator + () const
{
    return Data();
}
template <unsigned int N, unsigned  int M, typename T>
inline Matrix<N, M, T> Matrix<N, M, T>::operator - ()
{
    Matrix mTemp;
    for (unsigned int i = 0; i < N*M; i++)
        mTemp[i] = -pData[i];

    return mTemp;
}
template <unsigned int N, unsigned  int M, typename T>
inline Matrix<N, M, T> Matrix<N, M, T>::operator - () const
{
    Matrix mTemp;
    for (unsigned int i = 0; i < N*M; i++)
        mTemp[i] = -pData[i];

    return mTemp;
}


// Binary math operators - Matrix-Matrix
template <unsigned int N, unsigned  int M, typename T>
inline Matrix<N, M, T> Matrix<N, M, T>::operator + (const Matrix &rhs) const
{
    Matrix mTemp;
    for (unsigned int i = 0; i < N*M; i++)
        mTemp[i] = pData[i]+rhs[i];

    return mTemp;
}
template <unsigned int N, unsigned  int M, typename T>
inline Matrix<N, M, T> Matrix<N, M, T>::operator - (const Matrix &rhs) const
{
    Matrix mTemp;
    for (unsigned int i = 0; i < N*M; i++)
        mTemp[i] = pData[i]-rhs[i];

    return mTemp;
}
template <unsigned int N, unsigned  int M, typename T>
inline Matrix<N, M, T> Matrix<N, M, T>::operator * (const Matrix &rhs) const
{
    Matrix mTemp;
    for (unsigned int i = 0; i < N*M; i++)
        mTemp[i] = pData[i]*rhs[i];

    return mTemp;
}
template <unsigned int N, unsigned  int M, typename T>
inline Matrix<N, M, T> Matrix<N, M, T>::operator / (const Matrix &rhs) const
{
    Matrix mTemp;
    for (unsigned int i = 0; i < N*M; i++)
        mTemp[i] = pData[i]/rhs[i];

    return mTemp;
}

// Binary math operators - Matrix-T
template <unsigned int N, unsigned  int M, typename T>
inline Matrix<N, M, T> Matrix<N, M, T>::operator + (const T rhs) const
{
    Matrix mTemp;
    for (unsigned int i = 0; i < N*M; i++)
        mTemp[i] = pData[i]+rhs;

    return mTemp;
}
template <unsigned int N, unsigned  int M, typename T>
inline Matrix<N, M, T> Matrix<N, M, T>::operator - (const T rhs) const
{
    Matrix mTemp;
    for (unsigned int i = 0; i < N*M; i++)
        mTemp[i] = pData[i]-rhs;

    return mTemp;
}
template <unsigned int N, unsigned  int M, typename T>
inline Matrix<N, M, T> Matrix<N, M, T>::operator * (const T rhs) const
{
    Matrix mTemp;
    for (unsigned int i = 0; i < N*M; i++)
        mTemp[i] = pData[i]*rhs;

    return mTemp;
}
template <unsigned int N, unsigned  int M, typename T>
inline Matrix<N, M, T> Matrix<N, M, T>::operator / (const T rhs) const
{
    T rhsInv = static_cast<T>(1.0)/rhs;

    Matrix mTemp;
    for (unsigned int i = 0; i < N*M; i++)
        mTemp[i] = pData[i]*rhsInv;

    return mTemp;
}

// Assignment operators - Matrix4x4-Matrix4x4
template <unsigned int N, unsigned  int M, typename T>
inline Matrix<N, M, T> &Matrix<N, M, T>::operator += (const Matrix &rhs)
{
    for (unsigned int i = 0; i < N*M; i++)
        pData[i] = pData[i]+rhs[i];

    return Data();
}
template <unsigned int N, unsigned  int M, typename T>
inline Matrix<N, M, T> &Matrix<N, M, T>::operator -= (const Matrix &rhs)
{
    for (unsigned int i = 0; i < N*M; i++)
        pData[i] = pData[i]-rhs[i];

    return Data();
}
template <unsigned int N, unsigned  int M, typename T>
inline Matrix<N, M, T> &Matrix<N, M, T>::operator *= (const Matrix &rhs)
{
    for (unsigned int i = 0; i < N*M; i++)
        pData[i] = pData[i]*rhs[i];

    return Data();
}
template <unsigned int N, unsigned  int M, typename T>
inline Matrix<N, M, T> &Matrix<N, M, T>::operator /= (const Matrix &rhs)
{
    for (unsigned int i = 0; i < N*M; i++)
        pData[i] = pData[i]/rhs[i];

    return Data();
}

// Assignment operators - Matrix4x4-T
template <unsigned int N, unsigned  int M, typename T>
inline Matrix<N, M, T> &Matrix<N, M, T>::operator = (const T rhs)
{
    for (unsigned int i = 0; i < N*M; i++)
        pData[i] = rhs;

    return Data();
}
template <unsigned int N, unsigned  int M, typename T>
inline Matrix<N, M, T> &Matrix<N, M, T>::operator += (const T rhs)
{
    for (unsigned int i = 0; i < N*M; i++)
        pData[i] = pData[i]+rhs;

    return Data();
}
template <unsigned int N, unsigned  int M, typename T>
inline Matrix<N, M, T> &Matrix<N, M, T>::operator -= (const T rhs)
{
    for (unsigned int i = 0; i < N*M; i++)
        pData[i] = pData[i]-rhs;

    return Data();
}
template <unsigned int N, unsigned  int M, typename T>
inline Matrix<N, M, T> &Matrix<N, M, T>::operator *= (const T rhs)
{
    for (unsigned int i = 0; i < N*M; i++)
        pData[i] = pData[i]*rhs;

    return Data();
}
template <unsigned int N, unsigned  int M, typename T>
inline Matrix<N, M, T> &Matrix<N, M, T>::operator /= (const T rhs)
{
    T rhsInv = static_cast<T>(1.0)/rhs;

    for (unsigned int i = 0; i < N*M; i++)
        pData[i] = pData[i]*rhsInv;

    return Data();
}

// Boolean operators
template <unsigned int N, unsigned  int M, typename T>
bool Matrix<N, M, T>::operator == (const Matrix &rhs) const
{
    for (unsigned int i = 0; i < N*M; i++)
        if (pData[i] != rhs[i])
            return false;

    return true;
}
template <unsigned int N, unsigned  int M, typename T>
bool Matrix<N, M, T>::operator != (const Matrix &rhs) const
{
    for (unsigned int i = 0; i < N*M; i++)
    {
        if (pData[i] == rhs[i])
            return false;
    }
    return true;
}
template <unsigned int N, unsigned  int M, typename T>
bool Matrix<N, M, T>::operator > (const Matrix &rhs) const
{
    for (unsigned int i = 0; i < N*M; i++)
        if (pData[i] <= rhs[i])
            return false;

    return true;
}
template <unsigned int N, unsigned  int M, typename T>
bool Matrix<N, M, T>::operator < (const Matrix &rhs) const
{
    for (unsigned int i = 0; i < N*M; i++)
        if (pData[i] >= rhs[i])
            return false;

    return true;
}
template <unsigned int N, unsigned  int M, typename T>
bool Matrix<N, M, T>::operator >= (const Matrix &rhs) const
{
    for (unsigned int i = 0; i < N*M; i++)
        if (pData[i] < rhs[i])
            return false;

    return true;
}
template <unsigned int N, unsigned  int M, typename T>
bool Matrix<N, M, T>::operator <= (const Matrix &rhs) const
{
    for (unsigned int i = 0; i < N*M; i++)
        if (pData[i] > rhs[i])
            return false;

    return true;
}


// ---------------------------------Vector functions---------------------------------

// Get squared vector magnitude
template <unsigned int N, unsigned  int M, typename T>
T Matrix<N, M, T>::GetMagnitudeSqr() const
{
    STATIC_CHECK(IsVector, MUST_BE_VECTOR);

    T tMagnitude = 0;
    for (unsigned int i = 0; i < N; i++)
        tMagnitude += TMath::Sqr(pData[i]);

    return tMagnitude;
}

// Get vector magnitude
template <unsigned int N, unsigned  int M, typename T>
inline T Matrix<N, M, T>::GetMagnitude() const
{
    return TMath::Sqrt(GetMagnitudeSqr());
}

// Set vector magnitude
template <unsigned int N, unsigned  int M, typename T>
void Matrix<N, M, T>::SetMagnitude(const T tMagnitude)
{
    STATIC_CHECK(IsVector, MUST_BE_VECTOR);

    T tTemp = tMagnitude/GetMagnitude();

    for (unsigned int i = 0; i < N; i++)
        pData[i] *= tTemp;
}

// Normalize vector to unit length
template <unsigned int N, unsigned  int M, typename T>
inline void Matrix<N, M, T>::Normalize()
{
    STATIC_CHECK(IsVector, MUST_BE_VECTOR);

    SetMagnitude(1);
}


// ---------------------------------Matrix functions---------------------------------

// Elementary row operations

template <unsigned int N, unsigned  int M, typename T>
void Matrix<N, M, T>::SwapRows(const unsigned int iRow1, const unsigned int iRow2)
{
    assert(iRow1 < N && iRow1 >= 0 && iRow2 < N && iRow2 >= 0);

    Matrix<N, 1, T> vTemp(GetRowRef(iRow1));
    SetRow(iRow1, GetRowRef(iRow2));
    SetRow(iRow2, vTemp);
}

template <unsigned int N, unsigned  int M, typename T>
void Matrix<N, M, T>::ScaleRow(const unsigned int iRow, const T tScalar)
{
    assert(iRow < N && iRow >= 0);

    Matrix<N, 1, T> vTemp(GetRowRef(iRow));
    vTemp *= tScalar;
    SetRow(iRow, vTemp);
}

template <unsigned int N, unsigned  int M, typename T>
void Matrix<N, M, T>::AddScaledRow(const unsigned int iScaledRow, const T tScalar, const unsigned int iResultRow)
{
    assert(iScaledRow < N && iScaledRow >= 0 && iResultRow < N && iResultRow >= 0);

    Matrix<N, 1, T> vTemp = GetRowRef(iScaledRow);

    vTemp *= tScalar;
    vTemp += GetRowRef(iResultRow);

    SetRow(iResultRow, vTemp);
}

// Set identity matrix
template <unsigned int N, unsigned  int M, typename T>
void Matrix<N, M, T>::SetIdentity()
{
    STATIC_CHECK(IsSquare, MATRIX_MUST_BE_SQUARE);

    for (unsigned int i = 0; i < N; i++)
    {
        for (unsigned int j = 0; j < M; j++)
        {
            if (i == j)
                pData[i*M+j] = 1;
            else
                pData[i*M+j] = 0;
        }
    }
}

// Calculate matrix transpose
template <unsigned int N, unsigned  int M, typename T>
void Matrix<N, M, T>::Transpose()
{
    STATIC_CHECK(IsSquare, MATRIX_MUST_BE_SQUARE);

    for (unsigned int i = 0; i < N; i++)
    {
        for (unsigned int j = i+1; j < N; j++)
        {
            unsigned int i1 = i*M+j, i2 = j*M+i;
            T temp = pData[i1];
            pData[i1] = pData[i2];
            pData[i2] = temp;
        }
    }
}

// Normalize & make vectors (columns) orthogonal in a 3x3 matrix
template <unsigned int N, unsigned  int M, typename T>
void Matrix<N, M, T>::Orthonormalize()
{
    STATIC_CHECK(N == 3 && M == 3, MATRIX_MUST_BE_3_BY_3);

    Matrix<N, 1, T> vX, vY, vZ;
    GetXVector(vX);
    GetYVector(vY);

    vX.Normalize();
    vZ.Cross(vX, vY);
    vY.Cross(vZ, vX);
    vY.Normalize();
    vZ.Normalize();

    SetXVector(vX);
    SetYVector(vY);
    SetZVector(vZ);
}


// ----------------------------------Misc functions----------------------------------

// Return total of all entries in matrix
template <unsigned int N, unsigned  int M, typename T>
inline T Matrix<N, M, T>::Sum() const
{
    T tSum = 0;
    for (unsigned int i = 0; i < N*M; i++)
        tSum += pData[i];

    return tSum;
}

// Abs matrix to this
template <unsigned int N, unsigned  int M, typename T>
inline void Matrix<N, M, T>::Absolute()
{
    for (unsigned int i = 0; i < N*M; i++)
        pData[i] = TMath::Abs(pData[i]);
}

// Floor matrix
template <unsigned int N, unsigned  int M, typename T>
inline void Matrix<N, M, T>::Floor()
{
    for (unsigned int i = 0; i < N*M; i++)
        pData[i] = TMath::Floor<T>(pData[i]);
}

// Return maximum entry in matrix
template <unsigned int N, unsigned  int M, typename T>
T Matrix<N, M, T>::Max() const
{
    T tMax = -std::numeric_limits<T>::max();
    for (unsigned int i = 0; i < N*M; i++)
        if (pData[i] > tMax)
            tMax = pData[i];

    return tMax;
} 

// Return minimum entry in matrix
template <unsigned int N, unsigned  int M, typename T>
T Matrix<N, M, T>::Min() const
{
    T tMin = std::numeric_limits<T>::max();
    for (unsigned int i = 0; i < N*M; i++)
        if (pData[i] < tMin)
            tMin = pData[i];

    return tMin;
}


// -----------------------------Private access functions-----------------------------

template <unsigned int N, unsigned  int M, typename T>
inline const Matrix<N, 1, T> &Matrix<N, M, T>::GetRowRef(const unsigned int iRow) const
{
    assert(iRow >= 0 && iRow < N);
    return *(reinterpret_cast<const Matrix<N, 1, T> *>(this)+iRow);
}

template <unsigned int N, unsigned  int M, typename T>
inline Matrix<N, M, T> &Matrix<N, M, T>::Data()
{
    return *this;
}

template <unsigned int N, unsigned  int M, typename T>
inline const Matrix<N, M, T> &Matrix<N, M, T>::Data() const
{
    return *this;
}

template <unsigned int N, unsigned  int M, typename T>
inline T &Matrix<N, M, T>::Data(const unsigned int iRow, const unsigned int iColumn)
{
    return (*this)(iRow, iColumn);
}

template <unsigned int N, unsigned  int M, typename T>
inline const T &Matrix<N, M, T>::Data(const unsigned int iRow, const unsigned int iColumn) const
{
    return (*this)(iRow, iColumn);
}


// -----------------------------Private vector functions-----------------------------

// Return this vector with an added dimension set to 1
template <unsigned int N, unsigned  int M, typename T>
inline void Matrix<N, M, T>::VectorAddDimention(Matrix<N+1, 1, T> &vOut) const
{
    STATIC_CHECK(IsVector, MUST_BE_VECTOR);
    memcpy(&vOut, this, sizeof(T)*N);
    vOut[N] = 1;
}

// Return this vector with the last dimension removed
template <unsigned int N, unsigned  int M, typename T>
inline void Matrix<N, M, T>::VectorRemoveDimention(Matrix<N-1, 1, T>  &vOut) const
{
    STATIC_CHECK(IsVector, MUST_BE_VECTOR);
    memcpy(&vOut, this, sizeof(T)*(N-1));
}


// -----------------------------Private matrix functions-----------------------------

// Return this matrix with an added dimension set to identity
template <unsigned int N, unsigned  int M, typename T>
void Matrix<N, M, T>::MatrixAddDimention(Matrix<N+1, M+1, T> &mOut) const
{
    STATIC_CHECK(IsSquare, MATRIX_MUST_BE_SQUARE);

    mOut.SetIdentity();
    for (unsigned int i = 0; i < N; i++)
        memcpy(&mOut[i*(M+1)], &pData[i*M], sizeof(T)*M);
}

// Return this matrix with the last dimension removed
template <unsigned int N, unsigned  int M, typename T>
void Matrix<N, M, T>::MatrixRemoveDimention(Matrix<N-1, M-1, T> &mOut) const
{
    STATIC_CHECK(IsSquare, MATRIX_MUST_BE_SQUARE);
    STATIC_CHECK(N > 2, NUMBER_OF_ROWS_MUST_BE_GREATER_THAN_2);

    for (unsigned int i = 0; i < N-1; i++)
        memcpy(&mOut[i*(M-1)], &pData[i*M], sizeof(T)*(M-1));
}


// ----------------------------------------------------------------------------------
// -------------------Inline & templatized function definitions----------------------
// ----------------------------------------------------------------------------------

// ---------------------------------Vector functions---------------------------------

// Calculate cross product of v1 & v2 and place result in this. Note matrix must be 3x1
template <typename T>
void VectorCross(const Matrix<3, 1, T> &v1, const Matrix<3, 1, T> &v2, Matrix<3, 1, T> &vOut)
{
    Matrix<3, 1, T> vTemp;
    vTemp.x() = v1.y()*v2.z()-v1.z()*v2.y();
    vTemp.y() = v1.z()*v2.x()-v1.x()*v2.z();
    vTemp.z() = v1.x()*v2.y()-v1.y()*v2.x();

    vOut = vTemp;
}

// Calculate dot product of this & m
template <unsigned int N, typename T>
T VectorDot(const Matrix<N, 1, T> &v1, const Matrix<N, 1, T> &v2)
{
    T tDot = 0;
    for (unsigned int i = 0; i < N; i++)
        tDot += v1[i]*v2[i];

    return tDot;
}

// Calculate squared distance between vectors
template <unsigned int N, typename T>
T VectorDistanceSqr(const Matrix<N, 1, T> &v1, const Matrix<N, 1, T> &v2)
{
    T tDistance = 0;
    for (unsigned int i = 0; i < N; i++)
        tDistance += TMath::Sqr(v1[i]-v2[i]);

    return tDistance;
}

// Calculate distance between this * v
template <unsigned int N, typename T>
inline T VectorDistance(const Matrix<N, 1, T> &v1, const Matrix<N, 1, T> &v2)
{
    return Sqrt(VectorDistanceSqr(v1, v2));
}


// Calculate reflection vector
template <unsigned int N, typename T>
void VectorReflect(const Matrix<N, 1, T> &vIncident, const Matrix<N, 1, T> &vNormal, Matrix<N, 1, T> &vOut)
{
    T tDot = VectorDot(vIncident, vNormal)*2;

    for (unsigned int i = 0; i < N; i++)
        vOut[i] = vIncident[i]-vNormal[i]*tDot;
}

// Multiply Nx1 vector by NxN matrix
template <unsigned int N, typename T>
void VectorMultiply(const Matrix<N, 1, T> &v, const Matrix<N, N, T> &m, Matrix<N, 1, T> &vOut)
{
    Matrix<N, 1, T> vResult(0);
    for (unsigned int i = 0; i < N; i++)
        for (unsigned int j = 0; j < N; j++)
            vResult[i] += m(j, i)*v[j];

    memcpy(&vOut, &vResult, sizeof(T)*N);
}

// Multiply Nx1 vector by (N+1)x(N+1) matrix
template <unsigned int N, typename T>
void VectorMultiply(const Matrix<N, 1, T> &v, const Matrix<N+1, N+1, T> &m, Matrix<N, 1, T> &vOut)
{
    Matrix<N+1, 1, T> vTemp, vResult(0);
    v.VectorAddDimention(vTemp);

    for (unsigned int i = 0; i <= N; i++)
        for (unsigned int j = 0; j <= N; j++)
            vResult[i] += m(j, i)*vTemp[j];

    if (vResult[N] == 1)
        memcpy(&vOut, &vResult, sizeof(T)*N);
    else
    {
        T tTemp = static_cast<T>(1.0)/vResult[N];
        for (unsigned int i = 0; i < N; i++)
            vOut[i] = vResult[i]*tTemp;
    }
}


// ---------------------------------Matrix functions---------------------------------

// Multiply matrices m1 & m2 in the order m1 x m2
template <unsigned int N, unsigned int M, unsigned int P, typename T>
void MatrixMultiply(const Matrix<N, M, T> &m1, const Matrix<M, P, T> &m2, Matrix<N, P, T> &mOut)
{
    Matrix<N, P, T> mTemp(0);
    for (unsigned int i = 0; i < N; i++)
        for (unsigned int j = 0; j < P; j++)
            for (unsigned int k = 0; k < M; k++)
                mTemp(i, j) += m1(i, k)*m2(k, j);

    mOut = mTemp;
}


// Note: Matrix must be square for all following matrix functions

// Invert m and place result in mOut, returns false if matrix is singular
template <unsigned int N, typename T>
bool MatrixInvert(const Matrix<N, N, T> &m, Matrix<N, N, T> &mOut)
{
    // Augment an identity matrix onto the right side of matrix m,
    // perform elementary row operations on this augmented matrix until the left side is an identity matrix,
    // this will leave the inverse of matrix m on the right side

    Matrix<N, N, T> mIdent, mResult(m);
    mIdent.SetIdentity();

    // Iterate through matrix columns
    Matrix<N, 1, T> mCurrentColumn;
    for (unsigned int j = 0; j < N; j++)
    {
        // Get copy of column j
        mResult.GetColumn(j, mCurrentColumn);

        // Find row with maximum entry in column j
        int i = j;
        T tMax = mCurrentColumn[j];
        for (unsigned int iRow = j; iRow < N; iRow++)
        {
            T tTemp = TMath::Abs(mCurrentColumn[iRow]);
            if (tTemp > tMax)
            {
                tMax = tTemp;
                i = iRow;
            }
        }

        // If no row exists in column j with a non-zero entry then m is singular
        if (tMax == 0)
            return false;

        // If row with maximum entry in column j is not equal to j swap rows i and j
        if (i != j)
        {
            mResult.SwapRows(i, j);
            mIdent.SwapRows(i, j);
        }

        // Multiply row j by 1/m(j,j)
        T tScalar = static_cast<T>(1.0)/mResult(j, j);
        if (tScalar != 1)
        {
            mResult.ScaleRow(j, tScalar);
            mIdent.ScaleRow(j, tScalar);
        }

        // For each row r thats != j add -m(r, j) multiplied by row j to row r
        for (unsigned int r = 0; r < N; r++)
        {
            if (r == j)
                continue;

            tScalar = -mResult(r, j);
            if (tScalar != 0)
            {
                mResult.AddScaledRow(j, tScalar, r);
                mIdent.AddScaledRow(j, tScalar, r);
            }
        }
    }

    mOut = mIdent;

    return true;
}

// Check if matrix is an identity matrix
template <unsigned int N, typename T>
bool MatrixIsIdentity(const Matrix<N, N, T> &m)
{
    for (unsigned int i = 0; i < N; i++)
    {
        for (unsigned int j = 0; j < N; j++)
        {
            if (i == j)
            {
                if (m[i*M+j] != 1)
                    return false;
            }
            else
            {
                if (m[i*M+j] != 0)
                    return false;
            }
        }
    }

    return true;
}


// ---------------------------------Creation functions---------------------------------

// Generate a translation matrix
template <unsigned int N, typename T>
void CreateTranslationMatrix(const Matrix<N-1, 1, T> &vTranslation, Matrix<N, N, T> &mOut)
{
    STATIC_CHECK(N > 2, MATRIX_INCORRECT_SIZE);

    mOut.SetIdentity();

    Matrix<N, 1, T> vTemp;
    vTranslation.VectorAddDimention(vTemp);
    mOut.SetRow(N-1, vTemp);
}

// Generate a scaling matrix
template <unsigned int N, typename T>
void CreateScalingMatrix(const Matrix<N-1, 1, T> &vScale, Matrix<N, N, T> &mOut)
{
    STATIC_CHECK(N > 2, MATRIX_INCORRECT_SIZE);

    mOut.SetIdentity();

    for (unsigned int i = 0; i < N; i++)
        mOut(i, i) = vScale[i];
}

// Generate a rotation matrix about the X axis
template <unsigned int N, typename T>
void CreateRotationMatrixX(const T tAngle, Matrix<N, N, T> &mOut)
{
    STATIC_CHECK(N > 2, MATRIX_INCORRECT_SIZE);

    mOut.SetIdentity();

    T tSin = TMath::Sin(tAngle),
      tCos = TMath::Cos(tAngle);

    mOut(1, 1) = tCos;
    mOut(2, 1) = tSin;
    mOut(1, 2) = -tSin;
    mOut(2, 2) = tCos;
}

// Generate a rotation matrix about the Y axis
template <unsigned int N, typename T>
void CreateRotationMatrixY(const T tAngle, Matrix<N, N, T> &mOut)
{
    STATIC_CHECK(N > 2, MATRIX_INCORRECT_SIZE);

    mOut.SetIdentity();

    T tSin = TMath::Sin(tAngle),
      tCos = TMath::Cos(tAngle);

    mOut(0, 0) = tCos;
    mOut(0, 2) = tSin;
    mOut(2, 0) = -tSin;
    mOut(2, 2) = tCos;
}

// Generate a rotation matrix about the Z axis
// Note: Can also be used to generate rotation matrices for rotating 2D vectors
template <unsigned int N, typename T>
void CreateRotationMatrixZ(const T tAngle, Matrix<N, N, T> &mOut)
{
    STATIC_CHECK(N > 2, MATRIX_INCORRECT_SIZE);

    mOut.SetIdentity();

    T tSin = TMath::Sin(tAngle),
      tCos = TMath::Cos(tAngle);

    mOut(0, 0) = tCos;
    mOut(1, 0) = tSin;
    mOut(0, 1) = -tSin;
    mOut(1, 1) = tCos;
}

// Generate a rotate matrix about the XYZ axes
// Note: Rotations are applied in the order Z-X-Y
template <unsigned int N, typename T>
void CreateRotationMatrixXYZ(const Matrix<3, 1, T> &v, Matrix<N, N, T> &mOut)
{
    STATIC_CHECK(N > 2, MATRIX_INCORRECT_SIZE);

    mOut.SetIdentity();

    T tCosZ = TMath::Cos(-v.z()),
      tSinZ = TMath::Sin(-v.z()),
      tCosX = TMath::Cos(-v.x()),
      tSinX = TMath::Sin(-v.x()),
      tCosY = TMath::Cos(-v.y()),
      tSinY = TMath::Sin(-v.y());

    mOut(0, 0) = tCosZ*tCosY+tSinZ*tSinX*tSinY;
    mOut(0, 1) = tSinZ*tCosX;
    mOut(0, 2) = tCosZ*-tSinY+tSinZ*tSinX*tCosY;

    mOut(1, 0) = -tSinZ*tCosY+tCosZ*tSinX*tSinY;
    mOut(1, 1) = tCosZ*tCosX;
    mOut(1, 2) = tSinZ*tSinY+tCosZ*tSinX*tCosY;

    mOut(2, 0) = tCosX*tSinY;
    mOut(2, 1) = -tSinX;
    mOut(2, 2) = tCosX*tCosY;
}

// Generate a rotation matrix about an arbitrary axis, note vAxis must be unit length
template <unsigned int N, typename T>
void CreateRotationMatrixAxis(const Matrix<3, 1, T> &vAxis, const T tAngle, Matrix<N, N, T> &mOut)
{
    STATIC_CHECK(N > 2, MATRIX_INCORRECT_SIZE);

    mOut.SetIdentity();

    T tSin = TMath::Sin(-tAngle),
      tCos = TMath::Cos(-tAngle),
      tOneMinusCos = static_cast<T>(1.0)-tCos;

    mOut(0, 0) = tCos+tOneMinusCos*TMath::Sqr(vAxis.x());
    mOut(0, 1) = tOneMinusCos*vAxis.x()*vAxis.y()+tSin*vAxis.z();
    mOut(0, 2) = tOneMinusCos*vAxis.x()*vAxis.z()-tSin*vAxis.y();

    mOut(1, 0) = tOneMinusCos*vAxis.x()*vAxis.y()-tSin*vAxis.z();
    mOut(1, 1) = tCos+tOneMinusCos*TMath::Sqr(vAxis.y());
    mOut(1, 2) = tOneMinusCos*vAxis.y()*vAxis.z()+tSin*vAxis.x();

    mOut(2, 0) = tOneMinusCos*vAxis.x()*vAxis.z()+tSin*vAxis.y();
    mOut(2, 1) = tOneMinusCos*vAxis.y()*vAxis.z()-tSin*vAxis.x();
    mOut(2, 2) = tCos+tOneMinusCos*TMath::Sqr(vAxis.z());
}

// Generate view matrix from position, look, & up vectors. Note look & up vectors must be unit length
template <unsigned int N, typename T>
void CreateViewMatrix(const Matrix<3, 1, T> &vPosition, const Matrix<3, 1, T> &vLook, const Matrix<3, 1, T> &vUp, Matrix<N, N, T> &mOut)
{
    STATIC_CHECK(N > 2, MATRIX_INCORRECT_SIZE);

    // Calculate camera's XY axes, note Z axis = look vector in camera space
    Matrix<3, 1, T> vXAxis, vYAxis;
    VectorCross(vUp, vLook, vXAxis);
    vXAxis.Normalize();
    VectorCross(vLook, vXAxis, vYAxis);

    mOut(0, 0) = vXAxis.x();
    mOut(0, 1) = vYAxis.x();
    mOut(0, 2) = vLook.x();
    mOut(0, 3) = 0;

    mOut(1, 0) = vXAxis.y();
    mOut(1, 1) = vYAxis.y();
    mOut(1, 2) = vLook.y();
    mOut(1, 3) = 0;

    mOut(2, 0) = vXAxis.z();
    mOut(2, 1) = vYAxis.z();
    mOut(2, 2) = vLook.z();
    mOut(2, 3) = 0;

    mOut(3, 0) = -VectorDot(vPosition, vXAxis);
    mOut(3, 1) = -VectorDot(vPosition, vYAxis);
    mOut(3, 2) = -VectorDot(vPosition, vLook);
    mOut(3, 3) = 1;
}

// Generate projection matrix
template <unsigned int N, typename T>
void CreatePerspectiveMatrix(const float fFOVY, const float fAspectRatio, const float fNearClip, const float fFarClip, Matrix<N, N, T> &mOut)
{
    // Calculate Y scale of perspective projection & length of clip volume along Z axis
    float fYScale = 1.0f/TMath::Tan(fFOVY*0.5f),
          fInvClipRange = 1.0f/(fFarClip-fNearClip);

    mOut(0, 0) = static_cast<T>(1.0/fAspectRatio*fYScale);
    mOut(0, 1) = 0;
    mOut(0, 2) = 0;
    mOut(0, 3) = 0;

    mOut(1, 0) = 0;
    mOut(1, 1) = static_cast<T>(fYScale);
    mOut(1, 2) = 0;
    mOut(1, 3) = 0;

    mOut(2, 0) = 0;
    mOut(2, 1) = 0;
    mOut(2, 2) = static_cast<T>(fFarClip*fInvClipRange);
    mOut(2, 3) = 1;

    mOut(3, 0) = 0;
    mOut(3, 1) = 0;
    mOut(3, 2) = static_cast<T>(-fNearClip*(fFarClip*fInvClipRange));
    mOut(3, 3) = 0;
}



#endif