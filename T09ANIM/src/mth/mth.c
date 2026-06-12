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
#include "def.h"

/***  ////////////////
 ***  Vector Functions
 ***/ ////////////////

VEC4 VecSet4( FLT X, FLT Y, FLT Z, FLT W )
{
  VEC4 NewVec = {X, Y, Z, W};
  return NewVec;
}

/*  VecSet3 Creates a vector with provided coordinates.
 *  Parameters:
 *     FLT X, Y, Z - Vector coordinates.
 *  Returns: VEC Vector structure with provided coordinates.
 */
VEC VecSet3( FLT X, FLT Y, FLT Z )
{
  VEC NewVec = {X, Y, Z};
  return NewVec;
} /* End of 'VecSet3' function */


/*  VecAddVec Sums vectors.
 *  Parameters:
 *     VEC A, B - Vectors to be summed.
 *  Returns: VEC Sum of two vectors.
 */
VEC VecAddVec( VEC A, VEC B )
{
  return VecSet3(A.X + B.X, A.Y + B.Y, A.Z + B.Z);
} /* End of 'VecAddVec' function */


/*  VecSubVec Subtracts vectors.
 *  Parameters:
 *     VEC A - Vector to be substracted from.
 *     VEC B - Vector that's being substracted.
 *  Returns: VEC Difference of two vectors.
 */
VEC VecSubVec( VEC A, VEC B )
{
  return VecSet3(A.X - B.X, A.Y - B.Y, A.Z - B.Z);
} /* End of 'VecSubVec' function */


/*  VecLen2 Calculates square length of provided vector.
 *  Parameters:
 *     VEC Vec - vector to find square length of.
 *  Returns: FLT Square length.
 */
FLT VecLen2( VEC Vec )
{
  return Vec.X * Vec.X + Vec.Y * Vec.Y + Vec.Z * Vec.Z;
} /* End of 'VecLen2' function */


/*  VecLen Calculates length of provided vector.
 *  Parameters:
 *     VEC Vec - vector to find length of.
 *  Returns: FLT Length.
 */
FLT VecLen( VEC Vec )
{
  return sqrt(Vec.X * Vec.X + Vec.Y * Vec.Y + Vec.Z * Vec.Z);
} /* End of 'VecLen' function */


/*  VecCross Calculates cross product of two provided vectors.
 *  Parameters:
 *     VEC A, B - Vectors to find cross product of.
 *  Returns: VEC Cross product.
 */
VEC VecCross( VEC Vec1, VEC Vec2 )
{
  return VecSet3(Vec1.Y * Vec2.Z - Vec1.Z * Vec2.Y, Vec1.Z * Vec2.X - Vec1.X * Vec2.Z, Vec1.X * Vec2.Y - Vec1.Y * Vec2.X);
} /* End of 'VecCross' function */


/*  VecDot Calculates dot product of two provided vectors.
 *  Parameters:
 *     VEC A, B - Vectors to find dot product of.
 *  Returns: FLT Dot product.
 */
FLT VecDot( VEC Vec1, VEC Vec2 )
{
  return Vec1.X * Vec2.X + Vec1.Y * Vec2.Y + Vec1.Z * Vec2.Z;
} /* End of 'VecDot' function */


/*  VecScale Multiplies vector with provided coefficent.
 *  Parameters:
 *     VEC A - to be scaled.
 *     FLT a - scale factor.
 *  Returns: VEC Scaled vector.
 */
VEC VecScale( VEC Vec, FLT a )
{
  return VecSet3(Vec.X * a, Vec.Y * a, Vec.Z * a);
} /* End of 'VecScale' function */


/*  VecNormalize resizes vector to length one.
 *  Parameters:
 *     VEC A - to be scaled.
 *     FLT a - scale factor.
 *  Returns: VEC Scaled vector.
 */
VEC VecNormalize( VEC Vec )
{
  return VecScale(Vec, 1 / VecLen(Vec));
} /* End of 'VecNormalize' function */

VEC4 Vec3to4( VEC Vec )
{
  VEC4 NewVec = {Vec.X, Vec.Y, Vec.Z, 1};

  return NewVec;
}

/***  ////////////////
 ***  Matrix Functions
 ***/ ////////////////

MATR MatrSet( FLT A00, FLT A01, FLT A02, FLT A03,
              FLT A10, FLT A11, FLT A12, FLT A13,
              FLT A20, FLT A21, FLT A22, FLT A23,
              FLT A30, FLT A31, FLT A32, FLT A33 )
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

FLT MatrDeterm3x3( FLT A11, FLT A12, FLT A13,
                   FLT A21, FLT A22, FLT A23,
                   FLT A31, FLT A32, FLT A33 )
{
  return A11 * A22 * A33 + A12 * A23 * A31 + A13 * A21 * A32 -
         A11 * A23 * A32 - A12 * A21 * A33 - A13 * A22 * A31;
}

FLT MatrDeterm( MATR Matrix )
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
  FLT Determinant = MatrDeterm(Matrix);
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

VEC VecMulMatr( VEC Vec, MATR Matrix )
{
  FLT w = Vec.X * Matrix.Values[0][3] + Vec.Y * Matrix.Values[1][3] + Vec.Z * Matrix.Values[2][3] + Matrix.Values[3][3];
 
  return VecSet3((Vec.X * Matrix.Values[0][0] + Vec.Y * Matrix.Values[1][0] + Vec.Z * Matrix.Values[2][0] + Matrix.Values[3][0]) / w,
                (Vec.X * Matrix.Values[0][1] + Vec.Y * Matrix.Values[1][1] + Vec.Z * Matrix.Values[2][1] + Matrix.Values[3][1]) / w,
                (Vec.X * Matrix.Values[0][2] + Vec.Y * Matrix.Values[1][2] + Vec.Z * Matrix.Values[2][2] + Matrix.Values[3][2]) / w);
}

VEC VecRotateVec( VEC Axis, VEC Vec, FLT Degrees )
{
  FLT Sin = sin(D2R(Degrees));
  FLT Cos = cos(D2R(Degrees));
  MATR Matrix = MatrIdentity();
  
  Axis = VecNormalize(Axis);

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

MATR MatrRotateX( FLT Degrees )
{
  MATR NewMatrix = MatrIdentity();
  FLT Cos, Sin;

  Degrees = D2R(Degrees);
  Cos = cos(Degrees);
  Sin = sin(Degrees);
  NewMatrix.Values[1][1] =  Cos;
  NewMatrix.Values[1][2] =  Sin;
  NewMatrix.Values[2][1] = -Sin;
  NewMatrix.Values[2][2] =  Cos;
  return NewMatrix;
}

MATR MatrRotateY( FLT Degrees )
{
  MATR NewMatrix = MatrIdentity();
  FLT Cos, Sin;

  Degrees = D2R(Degrees);
  Cos = cos(Degrees);
  Sin = sin(Degrees);
  NewMatrix.Values[0][0] =  Cos;
  NewMatrix.Values[0][2] = -Sin;
  NewMatrix.Values[2][0] =  Sin;
  NewMatrix.Values[2][2] =  Cos;
  return NewMatrix;
}

MATR MatrRotateZ( FLT Degrees )
{
  MATR NewMatrix = MatrIdentity();
  FLT Cos, Sin;

  Degrees = D2R(Degrees);
  Cos = cos(Degrees);
  Sin = sin(Degrees);
  NewMatrix.Values[0][0] =  Cos;
  NewMatrix.Values[0][1] =  Sin;
  NewMatrix.Values[1][0] = -Sin;
  NewMatrix.Values[1][1] =  Cos;
  return NewMatrix;
}

MATR MatrTranslate( VEC Vec )
{
  MATR NewMatrix = MatrIdentity();

  NewMatrix.Values[3][0] = Vec.X;
  NewMatrix.Values[3][1] = Vec.Y;
  NewMatrix.Values[3][2] = Vec.Z;
  return NewMatrix;
}

VOID Swap( FLT *A, FLT *B )
{
  FLT Tmp = *A;
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

MATR MatrView( VEC Loc, VEC At, VEC Up1 )
{
  VEC Dir = VecNormalize(VecSubVec(At, Loc));
  VEC Right = VecNormalize(VecCross(Dir, Up1));
  VEC Up = VecNormalize(VecCross(Right, Dir));
  MATR NewMatrix =
  {
    {
      {Right.X, Up.X, -Dir.X, 0},
      {Right.Y, Up.Y, -Dir.Y, 0},
      {Right.Z, Up.Z, -Dir.Z, 0},
      {-VecDot(Loc, Right), -VecDot(Loc, Up), VecDot(Loc, Dir), 1}
    }
  };

  return NewMatrix;
};

MATR MatrFrustum( FLT Left, FLT Right, FLT Bottom, FLT Top, FLT Near, FLT Far )
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

MATR MatrScale1( FLT X )
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

FLT Sign( FLT X )
{
  return (X > 0) - (X < 0);
}

/* End of 'mth.c' file */
