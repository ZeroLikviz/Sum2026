/* FILE NAME: mth.c
 * PROGRAMMER: TM5
 * DATE: 09.06.2026
 * PURPOSE: Implementing matrix math.
 */
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "mth.h"

VEC3 VecSet3( DBL X, DBL Y, DBL Z )
{
  VEC3 NewVec = {X, Y, Z};
  return NewVec;
}

VEC3 VecAddVec3( VEC3 A, VEC3 B )
{
  return VecSet3(A.X + B.X, A.Y + B.Y, A.Z + B.Z);
}

VEC3 VecSubVec3( VEC3 A, VEC3 B )
{
  return VecSet3(A.X - B.X, A.Y - B.Y, A.Z - B.Z);
}

DBL VecLen23( VEC3 Vec )
{
  return Vec.X * Vec.X + Vec.Y * Vec.Y + Vec.Z * Vec.Z;
}

DBL VecLen3( VEC3 Vec )
{
  return sqrt(Vec.X * Vec.X + Vec.Y * Vec.Y + Vec.Z * Vec.Z);
}

VEC3 VecCross3( VEC3 Vec1, VEC3 Vec2 )
{
  return VecSet3(Vec1.Y * Vec2.Z - Vec1.Z * Vec2.Y, Vec1.Z * Vec2.X - Vec1.X * Vec2.Z, Vec1.X * Vec2.Y - Vec1.Y * Vec2.X);
}

DOUBLE VecDot3( VEC3 Vec1, VEC3 Vec2 )
{
  return Vec1.X * Vec2.X + Vec1.Y * Vec2.Y + Vec1.Z * Vec2.Z;
}

VEC3 VecMulNum3( VEC3 Vec, DOUBLE a )
{
  return VecSet3(Vec.X * a, Vec.Y * a, Vec.Z * a);
}

VEC3 VecNormalize3( VEC3 Vec )
{
  return VecMulNum3(Vec, 1 / VecLen3(Vec));
}

MATR MatrSet( DBL A00, DBL A01, DBL A02, DBL A03,
              DBL A10, DBL A11, DBL A12, DBL A13,
              DBL A20, DBL A21, DBL A22, DBL A23,
              DBL A30, DBL A31, DBL A32, DBL A33 )
{
  MATR NewMatrix =
  {
    {
      {A00, A01, A02, A03},
      {A10, A11, A12, A13},
      {A20, A21, A22, A23},
      {A30, A31, A32, A33},
    }
  };
 
  return NewMatrix;
}

MATR MatrIdentity( VOID )
{
  MATR NewMatrix =
  {
    {
      {1, 0, 0, 0},
      {0, 1, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1},
    }
  };
 
  return NewMatrix;
}

MATR MatrMulMatr( MATR M1, MATR M2 )
{
  INT i, k, j;
  MATR NewMatrix = {{{0}}};
 
  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (k = 0; k < 4; k++)
        NewMatrix.Values[i][j] += M1.Values[i][k] * M2.Values[k][j];
  return NewMatrix;
}

DBL MatrDeterm3x3( DBL A11, DBL A12, DBL A13,
                   DBL A21, DBL A22, DBL A23,
                   DBL A31, DBL A32, DBL A33 )
{
  return A11 * A22 * A33 + A12 * A23 * A31 + A13 * A21 * A32 -
         A11 * A23 * A32 - A12 * A21 * A33 - A13 * A22 * A31;
}

DBL MatrDeterm( MATR Matrix )
{
  return
     Matrix.Values[0][0] * MatrDeterm3x3(Matrix.Values[1][1], Matrix.Values[1][2], Matrix.Values[1][3],
                               Matrix.Values[2][1], Matrix.Values[2][2], Matrix.Values[2][3],
                               Matrix.Values[3][1], Matrix.Values[3][2], Matrix.Values[3][3]) +
    -Matrix.Values[0][1] * MatrDeterm3x3(Matrix.Values[1][0], Matrix.Values[1][2], Matrix.Values[1][3],
                               Matrix.Values[2][0], Matrix.Values[2][2], Matrix.Values[2][3],
                               Matrix.Values[3][0], Matrix.Values[3][2], Matrix.Values[3][3]) +
     Matrix.Values[0][2] * MatrDeterm3x3(Matrix.Values[1][0], Matrix.Values[1][1], Matrix.Values[1][3],
                               Matrix.Values[2][0], Matrix.Values[2][1], Matrix.Values[2][3],
                               Matrix.Values[3][0], Matrix.Values[3][1], Matrix.Values[3][3]) +
    -Matrix.Values[0][3] * MatrDeterm3x3(Matrix.Values[1][0], Matrix.Values[1][1], Matrix.Values[1][2],
                               Matrix.Values[2][0], Matrix.Values[2][1], Matrix.Values[2][2],
                               Matrix.Values[3][0], Matrix.Values[3][1], Matrix.Values[3][2]);
}

MATR MatrInverse( MATR Matrix )
{
  DBL Determinant = MatrDeterm(Matrix);
  MATR NewMatrix;
 
  if (Determinant == 0)
    return MatrIdentity();

  NewMatrix.Values[0][0] =
     MatrDeterm3x3(Matrix.Values[1][1], Matrix.Values[1][2], Matrix.Values[1][3],
                   Matrix.Values[2][1], Matrix.Values[2][2], Matrix.Values[2][3],
                   Matrix.Values[3][1], Matrix.Values[3][2], Matrix.Values[3][3]) / Determinant;
  NewMatrix.Values[1][0] =
    -MatrDeterm3x3(Matrix.Values[1][0], Matrix.Values[1][2], Matrix.Values[1][3],
                   Matrix.Values[2][0], Matrix.Values[2][2], Matrix.Values[2][3],
                   Matrix.Values[3][0], Matrix.Values[3][2], Matrix.Values[3][3]) / Determinant;
  NewMatrix.Values[2][0] =
     MatrDeterm3x3(Matrix.Values[1][0], Matrix.Values[1][1], Matrix.Values[1][3],
                   Matrix.Values[2][0], Matrix.Values[2][1], Matrix.Values[2][3],
                   Matrix.Values[3][0], Matrix.Values[3][1], Matrix.Values[3][3]) / Determinant;
  NewMatrix.Values[3][0] =
    -MatrDeterm3x3(Matrix.Values[1][0], Matrix.Values[1][1], Matrix.Values[1][2],
                   Matrix.Values[2][0], Matrix.Values[2][1], Matrix.Values[2][2],
                   Matrix.Values[3][0], Matrix.Values[3][1], Matrix.Values[3][2]) / Determinant;
 
  NewMatrix.Values[0][1] =
    -MatrDeterm3x3(Matrix.Values[0][1], Matrix.Values[0][2], Matrix.Values[0][3],
                   Matrix.Values[2][1], Matrix.Values[2][2], Matrix.Values[2][3],
                   Matrix.Values[3][1], Matrix.Values[3][2], Matrix.Values[3][3]) / Determinant;
  NewMatrix.Values[1][1] =
     MatrDeterm3x3(Matrix.Values[0][0], Matrix.Values[0][2], Matrix.Values[0][3],
                   Matrix.Values[2][0], Matrix.Values[2][2], Matrix.Values[2][3],
                   Matrix.Values[3][0], Matrix.Values[3][2], Matrix.Values[3][3]) / Determinant;
  NewMatrix.Values[2][1] =
    -MatrDeterm3x3(Matrix.Values[0][0], Matrix.Values[0][1], Matrix.Values[0][3],
                   Matrix.Values[2][0], Matrix.Values[2][1], Matrix.Values[2][3],
                   Matrix.Values[3][0], Matrix.Values[3][1], Matrix.Values[3][3]) / Determinant;
  NewMatrix.Values[3][1] =
     MatrDeterm3x3(Matrix.Values[0][0], Matrix.Values[0][1], Matrix.Values[0][2],
                   Matrix.Values[2][0], Matrix.Values[2][1], Matrix.Values[2][2],
                   Matrix.Values[3][0], Matrix.Values[3][1], Matrix.Values[3][2]) / Determinant;
 
  NewMatrix.Values[0][2] =
     MatrDeterm3x3(Matrix.Values[0][1], Matrix.Values[0][2], Matrix.Values[0][3],
                   Matrix.Values[1][1], Matrix.Values[1][2], Matrix.Values[1][3],
                   Matrix.Values[3][1], Matrix.Values[3][2], Matrix.Values[3][3]) / Determinant;
  NewMatrix.Values[1][2] =
    -MatrDeterm3x3(Matrix.Values[0][0], Matrix.Values[0][2], Matrix.Values[0][3],
                   Matrix.Values[1][0], Matrix.Values[1][2], Matrix.Values[1][3],
                   Matrix.Values[3][0], Matrix.Values[3][2], Matrix.Values[3][3]) / Determinant;
  NewMatrix.Values[2][2] =
     MatrDeterm3x3(Matrix.Values[0][0], Matrix.Values[0][1], Matrix.Values[0][3],
                   Matrix.Values[1][0], Matrix.Values[1][1], Matrix.Values[1][3],
                   Matrix.Values[3][0], Matrix.Values[3][1], Matrix.Values[3][3]) / Determinant;
  NewMatrix.Values[3][2] =
    -MatrDeterm3x3(Matrix.Values[0][0], Matrix.Values[0][1], Matrix.Values[0][2],
                   Matrix.Values[1][0], Matrix.Values[1][1], Matrix.Values[1][2],
                   Matrix.Values[3][0], Matrix.Values[3][1], Matrix.Values[3][2]) / Determinant;
 
  NewMatrix.Values[0][3] =
    -MatrDeterm3x3(Matrix.Values[0][1], Matrix.Values[0][2], Matrix.Values[0][3],
                   Matrix.Values[1][1], Matrix.Values[1][2], Matrix.Values[1][3],
                   Matrix.Values[2][1], Matrix.Values[2][2], Matrix.Values[2][3]) / Determinant;
  NewMatrix.Values[1][3] =
     MatrDeterm3x3(Matrix.Values[0][0], Matrix.Values[0][2], Matrix.Values[0][3],
                   Matrix.Values[1][0], Matrix.Values[1][2], Matrix.Values[1][3],
                   Matrix.Values[2][0], Matrix.Values[2][2], Matrix.Values[2][3]) / Determinant;
  NewMatrix.Values[2][3] =
    -MatrDeterm3x3(Matrix.Values[0][0], Matrix.Values[0][1], Matrix.Values[0][3],
                   Matrix.Values[1][0], Matrix.Values[1][1], Matrix.Values[1][3],
                   Matrix.Values[2][0], Matrix.Values[2][1], Matrix.Values[2][3]) / Determinant;
  NewMatrix.Values[3][3] =
     MatrDeterm3x3(Matrix.Values[0][0], Matrix.Values[0][1], Matrix.Values[0][2],
                   Matrix.Values[1][0], Matrix.Values[1][1], Matrix.Values[1][2],
                   Matrix.Values[2][0], Matrix.Values[2][1], Matrix.Values[2][2]) / Determinant;
 
  return NewMatrix;
}

VEC3 VecMulMatr( VEC3 Vec, MATR Matrix )
{
  DBL w = Vec.X * Matrix.Values[0][3] + Vec.Y * Matrix.Values[1][3] + Vec.Z * Matrix.Values[2][3] + Matrix.Values[3][3];
 
  return VecSet3((Vec.X * Matrix.Values[0][0] + Vec.Y * Matrix.Values[1][0] + Vec.Z * Matrix.Values[2][0] + Matrix.Values[3][0]) / w,
                (Vec.X * Matrix.Values[0][1] + Vec.Y * Matrix.Values[1][1] + Vec.Z * Matrix.Values[2][1] + Matrix.Values[3][1]) / w,
                (Vec.X * Matrix.Values[0][2] + Vec.Y * Matrix.Values[1][2] + Vec.Z * Matrix.Values[2][2] + Matrix.Values[3][2]) / w);
}

VEC3 VecRotateVec3( VEC3 Axis, VEC3 Vec, DBL Degrees )
{
  DBL Sin = sin(D2R(Degrees));
  DBL Cos = cos(D2R(Degrees));
  MATR Matrix = MatrIdentity();
  
  Axis = VecNormalize3(Axis);

  Matrix.Values[0][0] = Cos + Axis.X * Axis.X * (1 - Cos);
  Matrix.Values[0][1] = Axis.Y * Axis.X * (1 - Cos) + Axis.Z * Sin;
  Matrix.Values[0][2] = Axis.Z * Axis.X * (1 - Cos) - Axis.Y * Sin;

  Matrix.Values[1][0] = Axis.X * Axis.Y * (1 - Cos) - Axis.Z * Sin;
  Matrix.Values[1][1] = Cos + Axis.Y * Axis.Y * (1 - Cos);
  Matrix.Values[1][2] = Axis.Z * Axis.Y * (1 - Cos) + Axis.X * Sin;

  Matrix.Values[2][0] = Axis.X * Axis.Z * (1 - Cos) + Axis.Y * Sin;
  Matrix.Values[2][1] = Axis.Y * Axis.Z * (1 - Cos) - Axis.X * Sin;
  Matrix.Values[2][2] = Cos + Axis.Z * Axis.Z * (1 - Cos);

  return VecMulMatr(Vec, Matrix);
}

MATR MatrRotateX( DBL Degrees )
{
  MATR NewMatrix = MatrIdentity();
  DBL Cos, Sin;

  Degrees = D2R(Degrees);
  Cos = cos(Degrees);
  Sin = sin(Degrees);
  NewMatrix.Values[1][1] =  Cos;
  NewMatrix.Values[1][2] =  Sin;
  NewMatrix.Values[2][1] = -Sin;
  NewMatrix.Values[2][2] =  Cos;
  return NewMatrix;
}

MATR MatrRotateY( DBL Degrees )
{
  MATR NewMatrix = MatrIdentity();
  DBL Cos, Sin;

  Degrees = D2R(Degrees);
  Cos = cos(Degrees);
  Sin = sin(Degrees);
  NewMatrix.Values[0][0] =  Cos;
  NewMatrix.Values[0][2] = -Sin;
  NewMatrix.Values[2][0] =  Sin;
  NewMatrix.Values[2][2] =  Cos;
  return NewMatrix;
}

MATR MatrRotateZ( DBL Degrees )
{
  MATR NewMatrix = MatrIdentity();
  DBL Cos, Sin;

  Degrees = D2R(Degrees);
  Cos = cos(Degrees);
  Sin = sin(Degrees);
  NewMatrix.Values[0][0] =  Cos;
  NewMatrix.Values[0][1] =  Sin;
  NewMatrix.Values[1][0] = -Sin;
  NewMatrix.Values[1][1] =  Cos;
  return NewMatrix;
}

MATR MatrTranslate( VEC3 Vec )
{
  MATR NewMatrix = MatrIdentity();

  NewMatrix.Values[3][0] = Vec.X;
  NewMatrix.Values[3][1] = Vec.Y;
  NewMatrix.Values[3][2] = Vec.Z;
  return NewMatrix;
}

MATR MatrSetTranslate( MATR Matrix, VEC3 Vec )
{
  Matrix.Values[3][0] = Vec.X;
  Matrix.Values[3][1] = Vec.Y;
  Matrix.Values[3][2] = Vec.Z;
  return Matrix;
}

VOID Swap( DBL *A, DBL *B )
{
  DBL Tmp = *A;
  *A = *B;
  *B = Tmp;
}

MATR MatrTranspose( MATR Matrix )
{
  INT i, j;

  for (j = 0; j < 4; j++)
    for (i = 0; i < j + 1; i++)
      Swap(&Matrix.Values[i][j], &Matrix.Values[j][i]);

  return Matrix;
}

MATR MatrView( VEC3 Loc, VEC3 At, VEC3 Up1 )
{
  VEC3 Dir = VecNormalize3(VecSubVec3(At, Loc));
  VEC3 Right = VecNormalize3(VecCross3(Dir, Up1));
  VEC3 Up = VecNormalize3(VecCross3(Right, Dir));
  MATR NewMatrix =
  {
    {
      {Right.X, Up.X, -Dir.X, 0},
      {Right.Y, Up.Y, -Dir.Y, 0},
      {Right.Z, Up.Z, -Dir.Z, 0},
      {-VecDot3(Loc, Right), -VecDot3(Loc, Up), VecDot3(Loc, Dir), 1}
    }
  };

  return NewMatrix;
};

MATR MatrFrustum( DBL Left, DBL Right, DBL Bottom, DBL Top, DBL Near, DBL Far )
{
  MATR NewMatrix =
  {
    {
      {2 * Near / (Right - Left), 0, 0, 0},
      {0, 2 * Near / (Top - Bottom), 0, 0},
      {(Right + Left) / (Right - Left), (Top + Bottom) / (Top - Bottom), -(Far + Near) / (Far - Near), -1},
      { 0, 0, -2 * Near * Far / (Far - Near), 0}
    }
  };

  return NewMatrix;
}

MATR MatrScale1( DBL X )
{
  MATR NewMatrix =
  {
    {
      {X, 0, 0, 0},
      {0, X, 0, 0},
      {0, 0, X, 0},
      {0, 0, 0, 1}
    }
  };

  return NewMatrix;
}

MATR MatrScale3( VEC3 Vec )
{
  MATR NewMatrix =
  {
    {
      {Vec.X, 0, 0, 0},
      {0, Vec.Y, 0, 0},
      {0, 0, Vec.Z, 0},
      {0, 0, 0,     1}
    }
  };

  return NewMatrix;
}

VOID PrintMatrix( HDC hDC, MATR Matrix )
{
  static CHAR Text[4096] = "";
  INT i;

  for (i = 0; i < 4; i++)
    TextOut(hDC, 0, i * 20, Text, sprintf(Text, "[%3.1f][%3.1f][%3.1f][%3.1f]\n",
      Matrix.Values[i][0], Matrix.Values[i][1], Matrix.Values[i][2], Matrix.Values[i][3]));
}

VOID PrintVec3( HDC hDC, VEC3 Vec )
{
  static CHAR Text[4096] = "";
  TextOut(hDC, 0, 0, Text, sprintf(Text, "{%3.1f; %3.1f; %3.1f}\n", Vec.X, Vec.Y, Vec.Z));
}

/* End of 'mth.c' file */
