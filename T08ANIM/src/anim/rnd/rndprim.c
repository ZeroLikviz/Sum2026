/* FILE NAME: rndprim.c
 * PROGRAMMER: TM5
 * DATE: 09.06.2026
 * PURPOSE: Implement renderer.
 */

#include <math.h>

#include "anim/rnd/rnd.h"

BOOL TM5_RndPrimCreateSphere( tm5PRIM *Primitive, DBL R, INT W, INT H )
{
  INT i, j, k;
  DBL theta, phi;
 
  if (!TM5_RndPrimCreate(Primitive, W * H, (H - 1) * (W - 1) * 2 * 3))
    return FALSE;
 
  /* Fill vertex array */
  for (k = 0, i = 0, theta = 0; i < H; i++, theta += PI / (H - 1))
    for (j = 0, phi = 0; j < W; j++, phi += 2 * PI / (W - 1))
      Primitive->Vertices[k++].Vec = VecSet3(R * sin(theta) * sin(phi),
                                          R * cos(theta),
                                          R * sin(theta) * cos(phi));
 
  /* Fill index array */
  for (k = 0, i = 0; i < H - 1; i++)
    for (j = 0; j < W - 1; j++)
    {
      /* bottom-left */
      Primitive->Indexes[k++] = i * W + j;
      Primitive->Indexes[k++] = i * W + j + 1;
      Primitive->Indexes[k++] = (i + 1) * W + j;
      /* top-right */
      Primitive->Indexes[k++] = (i + 1) * W + j;
      Primitive->Indexes[k++] = i * W + j + 1;
      Primitive->Indexes[k++] = (i + 1) * W + j + 1;
    }
  return TRUE;
}

BOOL TM5_RndPrimCreateCylinder( tm5PRIM *Primitive, DBL R, DBL H, INT W)
{
  INT i, sign, k;
  DBL alpha;
 
  if (!TM5_RndPrimCreate(Primitive, W * 2 + 2, (W - 1) * 4 * 3 ))
    return FALSE;
 
  /* Fill vertex array */
  Primitive->Vertices[0].Vec = VecSet3(0, -H / 2, 0);
  Primitive->Vertices[1].Vec = VecSet3(0, H / 2, 0);
  for (k = 2, sign = -1; sign < 2; sign += 2)
    for (i = 0, alpha = 0; i < W; i++, alpha += DPI / (W - 1))
      Primitive->Vertices[k++].Vec = VecSet3(R * cos(alpha),
                                             sign * H / 2,
                                             R * sin(alpha));
  /* Fill index array */
  for (k = 0, i = 0; i < W - 1; i++)
  {
    Primitive->Indexes[k++] = 0;
    Primitive->Indexes[k++] = i + 1 + 2;
    Primitive->Indexes[k++] = i + 2;
  }
  for (i = 0; i < W - 1; i++)
  {
    Primitive->Indexes[k++] = 1;
    Primitive->Indexes[k++] = i + W + 1 + 2;
    Primitive->Indexes[k++] = i + W + 2;
  }
  for (i = 0; i < W - 1; i++)
  {
    Primitive->Indexes[k++] = i + 2;
    Primitive->Indexes[k++] = i + W + 2;
    Primitive->Indexes[k++] = i + W + 1 + 2;

    Primitive->Indexes[k++] = i + W + 1 + 2;
    Primitive->Indexes[k++] = i + 1 + 2;
    Primitive->Indexes[k++] = i + 2;
  }
  return TRUE;
}

BOOL TM5_RndPrimCreateTorus( tm5PRIM *Primitive, DBL oR, DBL iR, INT tW, INT iW)
{
  INT i, j, k;
  DBL alpha, beta;
 
  if (!TM5_RndPrimCreate(Primitive, tW * iW, (tW - 1) * (iW - 1) * 2 * 3))
    return FALSE;
 
  /* Fill vertex array */ 
  for (i = 0, alpha = 0, k = 0; i < tW; i++, alpha += DPI / (tW - 1))
  {
    MATR MatrixY = MatrRotateY(R2D(alpha));
    for (j = 0, beta = 0; j < iW; j++, beta += DPI / (iW - 1))
      Primitive->Vertices[k++].Vec = VecMulMatr(VecSet3(oR + cos(beta) * iR +  iR,
                                                        sin(beta) * iR,
                                                        0),
                                     MatrixY);
  }

  /* Fill index array */
  for (i = 0, k = 0; i < tW - 1; i++)
    for (j = 0; j < iW - 1; j++)
    {
      Primitive->Indexes[k++] = i * iW + j;
      Primitive->Indexes[k++] = (i + 1) * iW + j;
      Primitive->Indexes[k++] = i * iW + j + 1;

      Primitive->Indexes[k++] = (i + 1) * iW + j;
      Primitive->Indexes[k++] = (i + 1) * iW + j + 1;
      Primitive->Indexes[k++] = i * iW + j + 1;
    }
  
  return TRUE;
}

VOID TM5_RndPrimPermanentApply( tm5PRIM *Primitive, MATR Matrix )
{
  INT i;

  for (i = 0; i < Primitive->NumOfV; i++)
    Primitive->Vertices[i].Vec = VecMulMatr(Primitive->Vertices[i].Vec, Matrix);
}

VOID TM5_RndPrimRelocate( tm5PRIM *Primitive )
{
  INT i;
  VEC3 Min = Primitive->Vertices[0].Vec,
       Max = Primitive->Vertices[0].Vec,
       Relocation;

  for (i = 0; i < Primitive->NumOfV; i++)
  {
    Min.X = min(Primitive->Vertices[i].Vec.X, Min.X);
    Min.Y = min(Primitive->Vertices[i].Vec.Y, Min.Y);
    Min.Z = min(Primitive->Vertices[i].Vec.Z, Min.Z);

    Max.X = max(Primitive->Vertices[i].Vec.X, Max.X);
    Max.Y = max(Primitive->Vertices[i].Vec.Y, Max.Y);
    Max.Z = max(Primitive->Vertices[i].Vec.Z, Max.Z);
  }

  Relocation = VecMulNum3(VecAddVec3(Min, Max), -0.5);
  TM5_RndPrimPermanentApply(Primitive, MatrTranslate(Relocation));
}

VOID TM5_RndPrimRelocateMass( tm5PRIM *Primitive )
{
  INT i;
  VEC3 Relocation = VecSet3(0, 0, 0);

  for (i = 0; i < Primitive->NumOfV; i++)
    Relocation = VecAddVec3(Relocation, Primitive->Vertices[i].Vec);

  Relocation = VecMulNum3(Relocation, -1.0 / Primitive->NumOfV);
  TM5_RndPrimPermanentApply(Primitive, MatrTranslate(Relocation));
}

VOID TM5_RndPrimResizeTo1( tm5PRIM *Primitive )
{
  INT i;
  VEC3 Min = Primitive->Vertices[0].Vec,
       Max = Primitive->Vertices[0].Vec;
  DBL ResizeFactor;

  for (i = 0; i < Primitive->NumOfV; i++)
  {
    Min.X = min(Primitive->Vertices[i].Vec.X, Min.X);
    Min.Y = min(Primitive->Vertices[i].Vec.Y, Min.Y);
    Min.Z = min(Primitive->Vertices[i].Vec.Z, Min.Z);

    Max.X = max(Primitive->Vertices[i].Vec.X, Max.X);
    Max.Y = max(Primitive->Vertices[i].Vec.Y, Max.Y);
    Max.Z = max(Primitive->Vertices[i].Vec.Z, Max.Z);
  }

  ResizeFactor = 1 / max(max(Max.X - Min.X, Max.Y - Min.Y), Max.Z - Min.Z);
  TM5_RndPrimPermanentApply(Primitive, MatrScale1(ResizeFactor));
}

VOID TM5_RndPrimStandartize( tm5PRIM *Primitive )
{
  TM5_RndPrimRelocate(Primitive);
  TM5_RndPrimResizeTo1(Primitive);
}

/* End of 'rndprim.c' file */