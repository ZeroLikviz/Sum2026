/* FILE NAME: globe.c
 * PROGRAMMER: TM5
 * DATE: 06.06.2026
 * PURPOSE: Trying 3D.
 */
#include <math.h>
#include "globe.h"
#include "mth.h"
#include "timer.h"
#define GLB_GRID_H 30
#define GLB_GRID_W 30
#define WRAP_H(X) ((X) % GLB_GRID_H)
#define WRAP_W(X) ((X) % GLB_GRID_W)
INT GLB_Ws;
INT GLB_Hs;
INT GLB_Wp;
INT GLB_Hp;
DOUBLE GLB_ProjSize = 1;
DOUBLE GLB_ProjDist = 1;

static VEC3 GLB_Globe[GLB_GRID_H][GLB_GRID_W];
static VEC3 GLB_Normals[GLB_GRID_H][GLB_GRID_W];

MATR PointMatrix;
MATR VecMatrix;

DOUBLE Pow( DOUBLE X, DOUBLE Y )
{
  if (X <= 0)
    return -pow(-X, Y);
  return pow(X, Y);
}

DOUBLE Clamp( DOUBLE X, DOUBLE Min, DOUBLE Max )
{
  if (X < Min)
    return Min;
  if (X > Max)
    return Max;
  return X;
}

/* Fills GLB_Globe with sphere points */
VOID GLB_Init( DOUBLE R )
{
  INT i, j;
  DOUBLE Phi, Theta;
  static DOUBLE coeff = 0.35;
  
  coeff += 0.090 * sin(Time * 5);
  coeff = Clamp(coeff, 0.3, 1.4);

  for (i = 0, Theta = 0; i < GLB_GRID_H; i++, Theta += PI / (GLB_GRID_H - 1))
    for (j = 0, Phi = 0; j < GLB_GRID_W; j++, Phi += DPI / (GLB_GRID_W - 2))
    {
      GLB_Globe[i][j].X = R * Pow(sin(Theta), coeff) * Pow(sin(Phi), sin(coeff));
      GLB_Globe[i][j].Y = R * cos(Theta);
      GLB_Globe[i][j].Z = R * Pow(sin(Theta), coeff) * Pow(cos(Phi), cos(coeff));
    }
  
  for (i = 0; i < GLB_GRID_H; i++)
    for (j = 0; j < GLB_GRID_W; j++)
      GLB_Normals[i][j] = VecSet3(0, 0, 0);
    
  for (i = 0; i < GLB_GRID_H - 1; i++)
    for (j = 0; j < GLB_GRID_W - 1; j++)
    {
      VEC3 Vec1 = VecSubVec3(GLB_Globe[i][j], GLB_Globe[i + 1][j]);
      VEC3 Vec2 = VecSubVec3(GLB_Globe[i + 1][j], GLB_Globe[i + 1][j + 1]);
      VEC3 Cross = VecCross3(Vec1, Vec2);
      Cross = VecMulNum3(Cross, 1.0 / VecLen3(Cross));
      GLB_Normals[i][j] = VecAddVec3(GLB_Normals[i][j], Cross);
      GLB_Normals[i][j + 1] = VecAddVec3(GLB_Normals[i][j + 1], Cross);
      GLB_Normals[i + 1][j] = VecAddVec3(GLB_Normals[i + 1][j], Cross);
      GLB_Normals[i + 1][j + 1] = VecAddVec3(GLB_Normals[i + 1][j + 1], Cross);
    }

  for (i = 0; i < GLB_GRID_H; i++)
    for (j = 0; j < GLB_GRID_W; j++)
      GLB_Normals[i][j] = VecMulNum3(GLB_Normals[i][j], 1.0 / VecLen3(GLB_Normals[i][j]));
} /* End of 'GLB_Init' file */


/* Sets GLB_Ws and GLB_Hs */
VOID GLB_Resize( INT W, INT H )
{
  GLB_Ws = W;
  GLB_Hs = H;
  
  if (W <= H)
  {
    GLB_Wp = GLB_Ws;
    GLB_Hp = GLB_Ws;
  }
  else
  {
    GLB_Wp = GLB_Hs;
    GLB_Hp = GLB_Hs;
  }
} /* End of 'GLB_Resize' file */

VOID CalcMatrixes( VOID )
{
  MATR RX = MatrRotateX(270 * sin(Time) + 90);
  MATR RY = MatrRotateY(90 * sin(4 * Time));
  MATR RZ = MatrRotateZ(180 * cos(Time));
  MATR R = MatrMulMatr(MatrMulMatr(RY, RX), RZ);

  PointMatrix = MatrMulMatr(MatrMulMatr(R, MatrView(VecSet3(0, 0, -4 + 3 * sin(GlobalTime)), VecSet3(0, 0, 0), VecSet3(0, 1, 0))), 
    MatrFrustum(-GLB_Ws / 2, GLB_Ws / 2, -GLB_Hs / 2, GLB_Hs / 2, GLB_ProjDist, 10));
  VecMatrix = MatrTranspose(MatrInverse(R));
}

VEC3 Apply( VEC3 P )
{
  return VecMulMatr(P, PointMatrix);
}

VEC3 ApplyNormal( VEC3 P )
{
  return VecMulMatr(P, VecMatrix);
}

/* Draws GLB_Globe */
VOID GLB_Draw( HDC hDC )
{
  static POINT Projections[GLB_GRID_H][GLB_GRID_W];
  static POINT Face[4];
  INT i, j;

  //if (!IsPause)
  //  GLB_Init(0.5);
  
  CalcMatrixes();
  for (i = 0; i < GLB_GRID_H; i++)
    for (j = 0; j < GLB_GRID_W; j++)
    {
      VEC3 Vertex = Apply(GLB_Globe[i][j]);
      Projections[i][j].x = (INT)Vertex.X;
      Projections[i][j].y = (INT)Vertex.Y;
    }

  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SelectObject(hDC, GetStockObject(DC_PEN));
  for (i = 0; i < GLB_GRID_H - 1; i++)
    for (j = 0; j < GLB_GRID_W - 1; j++)
    {
      INT r = 255;
      INT g = 0;
      INT b = 0;
      DBL Factor = VecDot3(ApplyNormal(GLB_Normals[i][j]), VecSet3(0, 0, -1));
      if (Factor < 0.28)
        continue;
      /*
      Factor = pow(fabs(Factor), 20);
      Factor = pow(Factor, 0.3);
      if (Factor > 0.3 && Factor < 0.5)
        Factor = 1 - Factor;
      else if (Factor < 0.3)
        Factor = pow(Factor, Factor);
      */

      SetDCPenColor(hDC, RGB(255 - r * Factor, 255 - g * Factor, 255 - b * Factor));
      SetDCBrushColor(hDC, RGB(r * Factor, g * Factor, b * Factor));
      Face[0] = Projections[i][j];
      Face[1] = Projections[i][j + 1];
      Face[2] = Projections[i + 1][j + 1];
      Face[3] = Projections[i + 1][j];

      Polygon(hDC, Face, 4);
    }
} /* End of 'GLB_Draw' file */

/* END OF 't07globe.c' FILE */