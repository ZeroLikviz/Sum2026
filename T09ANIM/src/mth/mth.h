/* FILE NAME: mth.h
 * PROGRAMMER: TM5
 * DATE: 09.06.2026
 * PURPOSE: Implement matrix math.
 */
#ifndef __mth_h_
#define __mth_h_
#include <windows.h>

#define PI 3.141592653589793238462643
#define DPI (2 * PI)
#define D2R(X) ((X) / 180.0 * PI)
#define R2D(X) ((X) / PI * 180.0)

#define MatrMulMatr3(M1, M2, M3) (MatrMulMatr(M1, MatrMulMatr(M2, M3)))
#define MatrMulMatr4(M1, M2, M3, M4) (MatrMulMatr(M1, MatrMulMatr3(M2, M3, M4)))
#define MatrMulMatr5(M1, M2, M3, M4, M5) (MatrMulMatr(M1, MatrMulMatr4(M2, M3, M4, M5)))
#define MatrMulMatr6(M1, M2, M3, M4, M5, M6) (MatrMulMatr(M1, MatrMulMatr5(M2, M3, M4, M5, M6)))

typedef DOUBLE DBL;
typedef FLOAT FLT;

/* Vector Strucutres */
typedef struct tagVEC2
{
  FLT X, Y; /* Vector coordinates */
} VEC2;

typedef struct tagVEC
{
  FLT X, Y, Z;
} VEC;

typedef struct tagVEC4
{
  FLT X, Y, Z, W; /* Vector coordinates */
} VEC4;

/* Matrix Structure */
typedef struct tagMATRIX
{
  FLT Values[4][4]; /* Matrix table */
} MATR;

/***  ////////////////
 ***  Vector Functions
 ***/ ////////////////
VEC4 VecSet4( FLT X, FLT Y, FLT Z, FLT W );
VEC VecSet3( FLT X, FLT Y, FLT Z );
VEC VecAddVec( VEC A, VEC B );
VEC VecSubVec( VEC A, VEC B );
FLT VecLen2( VEC Vec );
FLT VecLen( VEC Vec );
VEC VecCross( VEC Vec1, VEC Vec2 );
FLT VecDot( VEC Vec1, VEC Vec2 );
VEC VecScale( VEC Vec, FLT a );
VEC VecMulMatr( VEC Vec, MATR Matrix );
VEC VecRotateVec( VEC Axis, VEC Vec, FLT Degrees );
VEC VecNormalize( VEC Vec );
VEC4 Vec3to4( VEC Vec );
VEC Vec4to3( VEC4 Vec );


/***  ////////////////
 ***  Matrix Functions
 ***/ ////////////////
MATR MatrSet( FLT A00, FLT A01, FLT A02, FLT A03,
              FLT A10, FLT A11, FLT A12, FLT A13,
              FLT A20, FLT A21, FLT A22, FLT A23,
              FLT A30, FLT A31, FLT A32, FLT A33 );
MATR MatrIdentity( VOID );
MATR MatrMulMatr( MATR M1, MATR M2 );
FLT MatrDeterm3x3( FLT A11, FLT A12, FLT A13,
                   FLT A21, FLT A22, FLT A23,
                   FLT A31, FLT A32, FLT A33 );
FLT MatrDeterm( MATR Matrix );
MATR MatrInverse( MATR Matrix );
MATR MatrRotateX( FLT Degrees );
MATR MatrRotateY( FLT Degrees );
MATR MatrRotateZ( FLT Degrees );
MATR MatrTranslate( VEC Vec );
MATR MatrTranspose( MATR Matrix );
MATR MatrView( VEC Loc, VEC At, VEC Up1 );
MATR MatrFrustum( FLT Left, FLT Right, FLT Top, FLT Bottom, FLT Near, FLT Far );
MATR MatrScale1( FLT X );

FLT Sign( FLT X );
#endif
/* End of 'mth.h' file */
