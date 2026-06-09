/* FILE NAME: mth.h
 * PROGRAMMER: TM5
 * DATE: 09.06.2026
 * PURPOSE: Implement matrix math.
 */
#ifndef __mth_h_
#define __mth_h_
#include <windows.h>
#ifndef PI
#define PI 3.141592653589793238462643
#endif

typedef DOUBLE DBL;
typedef FLOAT FLT;

typedef struct tagVEC2
{
  DBL X, Y;
} VEC2;

typedef struct tagVEC3
{
  DBL X, Y, Z;
} VEC3;

typedef struct tagVEC4
{
  DBL X, Y, Z, W;
} VEC4;

typedef struct tagMATRIX
{
  DBL Values[4][4];
} MATR;

VEC3 VecSet3( DBL X, DBL Y, DBL Z );
VEC3 VecAddVec3( VEC3 A, VEC3 B );
VEC3 VecSubVec3( VEC3 A, VEC3 B );
DBL VecLen23( VEC3 Vec );
DBL VecLen3( VEC3 Vec );
VEC3 VecCross3( VEC3 Vec1, VEC3 Vec2 );
DOUBLE VecDot3( VEC3 Vec1, VEC3 Vec2 );
VEC3 VecMulNum3( VEC3 Vec, DOUBLE a );
VEC3 VecMulMatr( VEC3 Vec, MATR Matrix );
VEC3 VecRotateVec3( VEC3 Axis, VEC3 Vec, DBL Degrees );
VEC3 VecNormalize3( VEC3 Vec );

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
MATR MatrTranslate( VEC3 Vec );
MATR MatrSetTranslate( MATR Matrix, VEC3 Vec );
MATR MatrTranspose( MATR Matrix );
MATR MatrView( VEC3 Loc, VEC3 At, VEC3 Up1 );
MATR MatrFrustum( DBL Left, DBL Right, DBL Top, DBL Bottom, DBL Near, DBL Far );

VOID PrintMatrix( HDC hDC, MATR Matrix );
VOID PrintVec3( HDC hDC, VEC3 Vec );
#endif
/* End of 'mth.h' file */
