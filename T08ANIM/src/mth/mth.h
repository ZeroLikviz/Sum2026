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

/* Vector Strucutre */
typedef struct tagVEC
{
  DBL X, Y, Z; /* Vector coordinates */
} VEC;

/* Matrix Structure */
typedef struct tagMATRIX
{
  DBL Values[4][4]; /* Matrix table */
} MATR;

/***  ////////////////
 ***  Vector Functions
 ***/ ////////////////
VEC VecSet3( DBL X, DBL Y, DBL Z );
VEC VecAddVec( VEC A, VEC B );
VEC VecSubVec( VEC A, VEC B );
DBL VecLen2( VEC Vec );
DBL VecLen( VEC Vec );
VEC VecCross( VEC Vec1, VEC Vec2 );
DOUBLE VecDot( VEC Vec1, VEC Vec2 );
VEC VecScale( VEC Vec, DOUBLE a );
VEC VecMulMatr( VEC Vec, MATR Matrix );
VEC VecRotateVec( VEC Axis, VEC Vec, DBL Degrees );
VEC VecNormalize( VEC Vec );


/***  ////////////////
 ***  Matrix Functions
 ***/ ////////////////
MATR MatrSet( DBL A00, DBL A01, DBL A02, DBL A03,
              DBL A10, DBL A11, DBL A12, DBL A13,
              DBL A20, DBL A21, DBL A22, DBL A23,
              DBL A30, DBL A31, DBL A32, DBL A33 );
MATR MatrIdentity( VOID );
MATR MatrMulMatr( MATR M1, MATR M2 );
DBL MatrDeterm3x3( DBL A11, DBL A12, DBL A13,
                   DBL A21, DBL A22, DBL A23,
                   DBL A31, DBL A32, DBL A33 );
DBL MatrDeterm( MATR Matrix );
MATR MatrInverse( MATR Matrix );
MATR MatrRotateX( DBL Degrees );
MATR MatrRotateY( DBL Degrees );
MATR MatrRotateZ( DBL Degrees );
MATR MatrTranslate( VEC Vec );
MATR MatrTranspose( MATR Matrix );
MATR MatrView( VEC Loc, VEC At, VEC Up1 );
MATR MatrFrustum( DBL Left, DBL Right, DBL Top, DBL Bottom, DBL Near, DBL Far );
MATR MatrScale1( DBL X );
#endif
/* End of 'mth.h' file */
