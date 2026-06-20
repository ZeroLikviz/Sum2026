/* FILE NAME: forest_unit.c
 * PROGRAMMER: TM5
 * DATE: 10.06.2026
 * PURPOSE: Implement animation system.
 */

#include <stdio.h>
#include "anim/units/units.h"

/*  Unit */
typedef struct tagtm5UNIT_CUSTARDS tm5UNIT_CUSTARDS;
struct tagtm5UNIT_CUSTARDS
{
  VOID (*Init)( tm5UNIT *Unit, tm5ANIM *Anim );
  VOID (*Close)( tm5UNIT *Unit, tm5ANIM *Anim );
  VOID (*Response)( tm5UNIT *Unit, tm5ANIM *Anim );
  VOID (*Render)( tm5UNIT *Unit, tm5ANIM *Anim );
  
  tm5PRIMS Model;
  INT Custards;
  VEC *CustardsPosBuf;
  BOOL *CustardsStateBuf;
  FLT Size;
};

/* SHhhh */
typedef struct tagtm5UNIT_HEIGHT_MAP tm5UNIT_HEIGHT_MAP;
struct tagtm5UNIT_HEIGHT_MAP
{
  VOID (*Init)( tm5UNIT *Unit, tm5ANIM *Anim );
  VOID (*Close)( tm5UNIT *Unit, tm5ANIM *Anim );
  VOID (*Response)( tm5UNIT *Unit, tm5ANIM *Anim );
  VOID (*Render)( tm5UNIT *Unit, tm5ANIM *Anim );
  
  tm5GRID Grid;
  CHAR Filename[512];
  FLT Size;
  tm5PRIM Model;
  VEC Pos;
};

static VOID Init( tm5UNIT *Unit, tm5ANIM *Anim )
{
  INT i;
  tm5UNIT_CUSTARDS *rUnit = (tm5UNIT_CUSTARDS*)(Unit);
  
  rUnit->CustardsPosBuf = malloc(sizeof(VEC) * rUnit->Custards);
  rUnit->CustardsStateBuf = malloc(sizeof(BOOL) * rUnit->Custards);
  TM5_RndPrimsLoadTransform = MatrScale1(0.1);
  TM5_RndPrimsLoad(&rUnit->Model, "bin/models/custard.g3dm", TRUE);
  TM5_RndMaterials[rUnit->Model.Primitives[0].MtlNumber].ShaderNumber = TM5_RndShdAdd("default");

  for (i = 0; i < rUnit->Custards; i++)
  {
    FLT NewX, NewY;
    INT X, Y;
    tm5UNIT_HEIGHT_MAP *HMap = (tm5UNIT_HEIGHT_MAP*)(Anim->Units[Anim->HeightMapId]);

    NewX = ((100 * rand()) % (INT)(rUnit->Size * 100 + 1)) / 100.0 - rUnit->Size / 2;
    NewY = ((100 * rand()) % (INT)(rUnit->Size * 100 + 1)) / 100.0 - rUnit->Size / 2;

    X = (INT)((NewX + HMap->Size / 2) / HMap->Size * HMap->Grid.W);
    Y = (INT)((NewY + HMap->Size / 2) / HMap->Size * HMap->Grid.H);
    X = Clamp(X, 0, HMap->Grid.W - 1);
    Y = Clamp(Y, 0, HMap->Grid.H - 1);

    rUnit->CustardsPosBuf[i].X = NewX;
    rUnit->CustardsPosBuf[i].Y = HMap->Grid.Vertices[X + Y * HMap->Grid.W].Vec.Y + 0.04;
    rUnit->CustardsPosBuf[i].Z = NewY;
    rUnit->CustardsStateBuf[i] = 1;
  }
}

static VOID Close( tm5UNIT *Unit, tm5ANIM *Anim )
{
  tm5UNIT_CUSTARDS *rUnit = (tm5UNIT_CUSTARDS*)(Unit);
  
  TM5_RndPrimsFree(&rUnit->Model);
}

static VOID Render( tm5UNIT *Unit, tm5ANIM *Anim )
{
  INT i;
  tm5UNIT_CUSTARDS *rUnit = (tm5UNIT_CUSTARDS*)(Unit);

  for (i = 0; i < rUnit->Custards; i++)
  {
    INT State = rUnit->CustardsStateBuf[i];
    if (State)
      TM5_RndPrimsDraw(&rUnit->Model, MatrTranslate(rUnit->CustardsPosBuf[i]));
  }
}

tm5UNIT* TM5_UnitCreateCustards( INT Custards, FLT Size )
{
  tm5UNIT_CUSTARDS *NewUnit = TM5_AnimCreateUnit(sizeof(tm5UNIT_CUSTARDS));
  
  NewUnit->Init = Init;
  NewUnit->Close = Close;
  NewUnit->Render = Render;
  NewUnit->Size = Size;
  NewUnit->Custards = Custards;

  return (VOID *)NewUnit;
}

/* End of 'forest_unit.c' file */
