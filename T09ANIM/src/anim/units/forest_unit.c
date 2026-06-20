/* FILE NAME: forest_unit.c
 * PROGRAMMER: TM5
 * DATE: 10.06.2026
 * PURPOSE: Implement animation system.
 */

#include <stdio.h>
#include "anim/units/units.h"

/*  Unit */
typedef struct tagtm5UNIT_FOREST tm5UNIT_FOREST;
struct tagtm5UNIT_FOREST
{
  VOID (*Init)( tm5UNIT *Unit, tm5ANIM *Anim );
  VOID (*Close)( tm5UNIT *Unit, tm5ANIM *Anim );
  VOID (*Response)( tm5UNIT *Unit, tm5ANIM *Anim );
  VOID (*Render)( tm5UNIT *Unit, tm5ANIM *Anim );
  
  tm5PRIMS Model;
  INT Trees;
  VEC2 *TreesPosBuf;
  INT *TreesTypeBuf;
  FLT Size;
};

static tm5UNIT_FOREST LastUnit;

static VOID Init( tm5UNIT *Unit, tm5ANIM *Anim )
{
  INT i;
  tm5UNIT_FOREST *rUnit = (tm5UNIT_FOREST*)(Unit);
  
  rUnit->TreesPosBuf = malloc(sizeof(VEC2) * rUnit->Trees);
  rUnit->TreesTypeBuf = malloc(sizeof(INT) * rUnit->Trees);
  TM5_RndPrimsLoad(&rUnit->Model, "bin/models/trees.g3dm", FALSE);
  TM5_RndMaterials[rUnit->Model.Primitives[0].MtlNumber].ShaderNumber = TM5_RndShdAdd("snow");

  for (i = 0; i < rUnit->Trees; i++)
  {
    FLT X, Y;
    X = ((100 * rand()) % (INT)(rUnit->Size * 100 + 1)) / 100.0 - rUnit->Size / 2;
    Y = ((100 * rand()) % (INT)(rUnit->Size * 100 + 1)) / 100.0 - rUnit->Size / 2;
    rUnit->TreesPosBuf[i].X = X;
    rUnit->TreesPosBuf[i].Y = Y;
    rUnit->TreesTypeBuf[i] = rand() % 3;
    if (rUnit->TreesTypeBuf[i] == 0)
      rUnit->TreesPosBuf[i].X -= 10;
    if (rUnit->TreesTypeBuf[i] == 2)
      rUnit->TreesPosBuf[i].X += 10;
  }

  LastUnit = *rUnit;
}

VEC TM5_RndUnitForestCollision( VEC Pos )
{
  INT i;
  tm5UNIT_FOREST *rUnit = &LastUnit;
  VEC Out = VecSet3(0, 0, 0);

  Pos.Y = 0;
  for (i = 0; i < rUnit->Trees; i++)
  {
    FLT X = rUnit->TreesPosBuf[i].X;
    FLT Y = rUnit->TreesPosBuf[i].Y;
    VEC Diff;
    FLT Len;

    if (rUnit->TreesTypeBuf[i] == 0)
      X += 10;
    if (rUnit->TreesTypeBuf[i] == 2)
      X -= 10;

    Diff = VecSubVec(Pos, VecSet3(X, 0, Y));
    Len = VecLen(Diff);
    if (Len < 1.2)
      Out = VecAddVec(VecScale(VecNormalize(Diff), 1.2 - Len), Out);
  }

  return Out;
}

static VOID Close( tm5UNIT *Unit, tm5ANIM *Anim )
{
  tm5UNIT_FOREST *rUnit = (tm5UNIT_FOREST*)(Unit);
  
  TM5_RndPrimsFree(&rUnit->Model);
}

static VOID Render( tm5UNIT *Unit, tm5ANIM *Anim )
{
  INT i;
  tm5UNIT_FOREST *rUnit = (tm5UNIT_FOREST*)(Unit);

  for (i = 0; i < rUnit->Trees; i++)
  {
    INT Type = rUnit->TreesTypeBuf[i];
    tm5PRIM *Primitive = &rUnit->Model.Primitives[Type];
    VEC Pos = VecSet3(rUnit->TreesPosBuf[i].X, 0, rUnit->TreesPosBuf[i].Y);

    TM5_RndPrimDraw(Primitive, MatrTranslate(Pos));
  }
}

tm5UNIT* TM5_UnitCreateForest( INT Trees, FLT Size )
{
  tm5UNIT_FOREST *NewUnit = TM5_AnimCreateUnit(sizeof(tm5UNIT_FOREST));
  
  NewUnit->Init = Init;
  NewUnit->Close = Close;
  NewUnit->Render = Render;
  NewUnit->Size = Size;
  NewUnit->Trees = Trees;

  return (VOID *)NewUnit;
}

/* End of 'forest_unit.c' file */
