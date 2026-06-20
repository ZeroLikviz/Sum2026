/* FILE NAME: fps_unit.c
 * PROGRAMMER: TM5
 * DATE: 10.06.2026
 * PURPOSE: Implement animation system.
 */

#include <math.h>
#include <stdio.h>

#include "anim/units/units.h"

static FLT RealY, ShowY;

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
  RealY = Anim->Camera.Pos.Y;
  ShowY = Anim->Camera.Pos.Y;
}

static VOID Response( tm5UNIT *Unit, tm5ANIM *Anim )
{
  tm5UNIT_HEIGHT_MAP *HMap = (tm5UNIT_HEIGHT_MAP*)(Anim->Units[Anim->HeightMapId]);
  INT X = (INT)((Anim->Camera.Pos.X + HMap->Size / 2) / HMap->Size * HMap->Grid.W);
  INT Y = (INT)((Anim->Camera.Pos.Z + HMap->Size / 2) / HMap->Size * HMap->Grid.H);
  X = Clamp(X, 0, HMap->Grid.W - 1);
  Y = Clamp(Y, 0, HMap->Grid.H - 1);

  Anim->Camera.HasNew = TRUE;
  Anim->Camera.NewPos = Anim->Camera.Pos;
  RealY = HMap->Grid.Vertices[X + Y * HMap->Grid.W].Vec.Y + 0.6;
  ShowY = Mix(ShowY, RealY, Anim->DeltaTime * 4);
  Anim->Camera.NewPos = VecMix(Anim->Camera.NewPos, VecAddVec(Anim->Camera.NewPos, TM5_RndUnitForestCollision(Anim->Camera.NewPos)), Anim->DeltaTime * 8);
  Anim->Camera.NewPos.Y = ShowY;
}

tm5UNIT* TM5_UnitCreateCamsmooth( VOID )
{
  tm5UNIT *NewUnit = TM5_AnimCreateUnit(sizeof(tm5UNIT));

  NewUnit->Init = Init;
  NewUnit->Response = Response;

  return (VOID *)NewUnit;
}

/* End of 'fps_unit.c' file */
