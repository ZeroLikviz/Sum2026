/* FILE NAME: globe.c
 * PROGRAMMER: TM5
 * DATE: 06.06.2026
 * PURPOSE: Trying 3D.
 */
#include <time.h>
#include <math.h>
#include "globe.h"
#include "mth.h"
#define HIDE_MARGIN 0.2
#define GLB_GRID_H 40
#define GLB_GRID_W 40
#define WRAP_H(X) ((X) % GLB_GRID_H)
#define WRAP_W(X) ((X) % GLB_GRID_W)
INT GLB_Ws;
INT GLB_Hs;
INT GLB_Wp;
INT GLB_Hp;
DOUBLE GLB_ProjSize = 1;
DOUBLE GLB_ProjDist = 1;
DOUBLE Timer = 0;

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

/* Fills GLB_Globe with sphere points */
VOID GLB_Init( DOUBLE R )
{
  INT i, j;
  DOUBLE Phi, Theta;
  static DOUBLE a = 0.3, b = 0.3 ;
  a += 0.050 * sin(Timer * 5);
  b += 0.050 * sin(Timer * 5);

  for (i = 0, Theta = 0; i < GLB_GRID_H; i++, Theta += PI / (GLB_GRID_H - 1))
    for (j = 0, Phi = 0; j < GLB_GRID_W; j++, Phi += DPI / (GLB_GRID_W - 2))
    {
      GLB_Globe[i][j].X = R * Pow(sin(Theta), a) * Pow(sin(Phi), b);
      GLB_Globe[i][j].Y = R * cos(Theta);
      GLB_Globe[i][j].Z = R * Pow(sin(Theta), b) * Pow(cos(Phi), a);
    }
  
  for (i = 0; i < GLB_GRID_H - 1; i++)
    for (j = 0; j < GLB_GRID_W; j++)
    {
      VEC3 Vec1 = VecSubVec3(GLB_Globe[i][j], GLB_Globe[i + 1][j]);
      VEC3 Vec2 = VecSubVec3(GLB_Globe[i + 1][j], GLB_Globe[i + 1][WRAP_W(j + 1)]);
      VEC3 Cross = VecCross3(Vec1, Vec2);
      Cross = VecMulNum3(Cross, 1.0 / VecLen3(Cross));
      GLB_Normals[i][j] = Cross;
    }
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
  MATR RX = MatrRotateX(270 * sin(Timer));
  MATR RY = MatrRotateY(90 * sin(4 * Timer));
  MATR RZ = MatrRotateZ(180 * cos(Timer));
  MATR R = MatrMulMatr(MatrMulMatr(RY, RX), RZ);
  MATR T = MatrTranslate(VecSet3(0, 0, -3));

  PointMatrix = MatrMulMatr(R, T);
  VecMatrix = MatrTranspone(MatrInverse(R));
  Timer = (DOUBLE)clock() / CLOCKS_PER_SEC;
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

  GLB_Init(0.8);
  CalcMatrixes();
  for (i = 0; i < GLB_GRID_H; i++)
    for (j = 0; j < GLB_GRID_W; j++)
    {
      VEC3 Vertex = Apply(GLB_Globe[i][j]);
      Projections[i][j].x =  (INT)(Vertex.X * GLB_Wp * GLB_ProjDist / Vertex.Z) + GLB_Ws / 2;
      Projections[i][j].y = -(INT)(Vertex.Y * GLB_Hp * GLB_ProjDist / Vertex.Z) + GLB_Hs / 2;
    }

  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SelectObject(hDC, GetStockObject(DC_PEN));
  for (i = 0; i < GLB_GRID_H - 1; i++)
    for (j = 0; j < GLB_GRID_W - 1; j++)
    {
      DBL Factor = VecDot3(ApplyNormal(GLB_Normals[i][j]), VecSet3(0, 0, -1));
      if (Factor > -0.2)
        continue;
      Factor = fabs(Factor);
      SetDCPenColor(hDC, RGB(255 * Factor, 180 * Factor, 0 * Factor));
      SetDCBrushColor(hDC, RGB(255 * Factor, 180 * Factor, 0 * Factor));
      Face[0] = Projections[i][j];
      Face[1] = Projections[i][j + 1];
      Face[2] = Projections[i + 1][j + 1];
      Face[3] = Projections[i + 1][j];

      Polygon(hDC, Face, 4);
    }
} /* End of 'GLB_Draw' file */

/* END OF 't07globe.c' FILE */