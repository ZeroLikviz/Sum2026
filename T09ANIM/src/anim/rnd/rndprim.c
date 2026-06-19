/* FILE NAME: rndprim.c
 * PROGRAMMER: TM5
 * DATE: 09.06.2026
 * PURPOSE: Implement renderer.
 */

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "anim/rnd/rnd.h"

VOID TM5_RndPrimCreateSquare( tm5PRIM *Primitive, DBL Size, VEC Color )
{
  tm5VERTEX Vertices[4] = {0};
  INT Indexes[6] = {0};
  INT i;
  
  Vertices[0].Vec = VecSet3(0, 0, 0);
  Vertices[1].Vec = VecSet3(Size, 0, 0);
  Vertices[2].Vec = VecSet3(Size, Size, 0);
  Vertices[3].Vec = VecSet3(0, Size, 0);

  Vertices[0].Texture = VecSet2(0, 0);
  Vertices[1].Texture = VecSet2(1, 0);
  Vertices[2].Texture = VecSet2(1, 1);
  Vertices[3].Texture = VecSet2(0, 1);

  Indexes[0] = 0;
  Indexes[1] = 1;
  Indexes[2] = 3;
  Indexes[3] = 3;
  Indexes[4] = 1;
  Indexes[5] = 2;

  for (i = 0; i < 4; i++)
    Vertices[i].Color = Vec3to4(Color);

  TM5_RndPrimCreate(Primitive, Vertices, 4, Indexes, 6, TM5_RND_TRIANGLES);
}

VOID TM5_RndPrimPermanentApply( tm5VERTEX *Vertices, INT NumOfV, MATR Matrix )
{
  INT i;

  for (i = 0; i < NumOfV; i++)
    Vertices[i].Vec = VecMulMatr(Vertices[i].Vec, Matrix);
}

VOID TM5_RndPrimRelocate( tm5VERTEX *Vertices, INT NumOfV )
{
  INT i;
  VEC Min = Vertices[0].Vec,
       Max = Vertices[0].Vec,
       Relocation;

  for (i = 0; i < NumOfV; i++)
  {
    Min.X = min(Vertices[i].Vec.X, Min.X);
    Min.Y = min(Vertices[i].Vec.Y, Min.Y);
    Min.Z = min(Vertices[i].Vec.Z, Min.Z);

    Max.X = max(Vertices[i].Vec.X, Max.X);
    Max.Y = max(Vertices[i].Vec.Y, Max.Y);
    Max.Z = max(Vertices[i].Vec.Z, Max.Z);
  }

  Relocation = VecScale(VecAddVec(Min, Max), -0.5);
  TM5_RndPrimPermanentApply(Vertices, NumOfV, MatrTranslate(Relocation));
}

VOID TM5_RndPrimResize( tm5VERTEX *Vertices, INT NumOfV )
{
  INT i;
  VEC Min = Vertices[0].Vec,
       Max = Vertices[0].Vec;
  DBL ResizeFactor;

  for (i = 0; i < NumOfV; i++)
  {
    Min.X = min(Vertices[i].Vec.X, Min.X);
    Min.Y = min(Vertices[i].Vec.Y, Min.Y);
    Min.Z = min(Vertices[i].Vec.Z, Min.Z);

    Max.X = max(Vertices[i].Vec.X, Max.X);
    Max.Y = max(Vertices[i].Vec.Y, Max.Y);
    Max.Z = max(Vertices[i].Vec.Z, Max.Z);
  }

  ResizeFactor = 1 / max(max(Max.X - Min.X, Max.Y - Min.Y), Max.Z - Min.Z);
  TM5_RndPrimPermanentApply(Vertices, NumOfV, MatrScale1(ResizeFactor));
}

VOID TM5_RndPrimStandartize( tm5VERTEX *Vertices, INT NumOfV )
{
  TM5_RndPrimRelocate(Vertices, NumOfV);
  TM5_RndPrimResize(Vertices, NumOfV);
}

VOID TM5_RndPrimCalculateNormals( tm5VERTEX *Vertices, INT NumOfV, INT *Indexes, INT NumOfI )
{
  INT i = 0;

  for (i = 0; i < NumOfV; i++)
    Vertices[i].Normal = VecSet3(0, 0, 0);

  for (i = 0; i < NumOfI; i += 3)
  {
    VEC Vec1 = Vertices[Indexes[i + 0]].Vec;
    VEC Vec2 = Vertices[Indexes[i + 1]].Vec;
    VEC Vec3 = Vertices[Indexes[i + 2]].Vec;
    VEC Cross = VecCross(VecSubVec(Vec1, Vec2), VecSubVec(Vec2, Vec3));
    Cross = VecNormalize(Cross);

    Vertices[Indexes[i + 0]].Normal = VecAddVec(Cross, Vertices[Indexes[i + 0]].Normal);
    Vertices[Indexes[i + 1]].Normal = VecAddVec(Cross, Vertices[Indexes[i + 1]].Normal);
    Vertices[Indexes[i + 2]].Normal = VecAddVec(Cross, Vertices[Indexes[i + 2]].Normal);
  }

  for (i = 0; i < NumOfV; i++)
    Vertices[i].Normal = VecNormalize(Vertices[i].Normal);
}

VOID TM5_RndPrimApplySun( tm5VERTEX *Vertices, INT NumOfV, VEC SunPos )
{
  INT i = 0;

  SunPos = VecNormalize(SunPos);
  for (i = 0; i < NumOfV; i++)
  {
    FLT Factor = (VecDot(VecScale(SunPos, -1), Vertices[i].Normal) + 1) / 2;
    if (Factor < 0.5)
      Factor = 0.5;
    Vertices[i].Color = Vec3to4(VecScale(Vec4to3(Vertices[i].Color), Factor));
  }
}

VOID TM5_RndPrimScale( tm5VERTEX *Vertices, INT NumOfV, FLT Factor )
{
  INT i = 0;

  for (i = 0; i < NumOfV; i++)
    Vertices[i].Vec = VecScale(Vertices[i].Vec, Factor);
}

VOID TM5_RndPrimFree( tm5PRIM *Primitive )
{
  glBindVertexArray(Primitive->VertexArrayId);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &Primitive->VertexBufferId);
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &Primitive->VertexArrayId);
  memset(Primitive, 0, sizeof(tm5PRIM));
}

VOID TM5_RndPrimCreate( tm5PRIM *Primitive, tm5VERTEX *Vertices, INT NofV, INT *Indexes, INT NofI, tm5DRAW_MODE DrawMode )
{
  memset(Primitive, 0, sizeof(tm5PRIM));
  
  Primitive->MinBB = TM5_RndVertexCalculateMinBB(Vertices, NofV);
  Primitive->MaxBB = TM5_RndVertexCalculateMaxBB(Vertices, NofV);
  Primitive->DrawMode = DrawMode;
  Primitive->Transform = MatrIdentity();
  Primitive->NumOfElements = NofV;
  if (NofI != 0)
    Primitive->NumOfElements = NofI;
  glGenBuffers(1, &Primitive->VertexBufferId);
  glGenBuffers(1, &Primitive->IndexBufferId);
  glGenVertexArrays(1, &Primitive->VertexArrayId);

  glBindVertexArray(Primitive->VertexArrayId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Primitive->IndexBufferId);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT) * NofI, Indexes, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, Primitive->VertexBufferId);
  glBufferData(GL_ARRAY_BUFFER, sizeof(tm5VERTEX) * NofV, Vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(tm5VERTEX),
                        (VOID *)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(tm5VERTEX),
                        (VOID *)sizeof(VEC));
  glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(tm5VERTEX),
                        (VOID *)(sizeof(VEC) + sizeof(VEC2)));
  glVertexAttribPointer(3, 4, GL_FLOAT, FALSE, sizeof(tm5VERTEX),
                        (VOID *)(sizeof(VEC) * 2 + sizeof(VEC2)));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);

  glBindVertexArray(0);
}

/* End of 'rndprim.c' file */