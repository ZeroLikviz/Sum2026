/* FILE NAME: g3dm_unit.c
 * PROGRAMMER: TM5
 * DATE: 10.06.2026
 * PURPOSE: Implement animation system.
 */

#include <stdio.h>
#include "anim/units/units.h"

/*  Unit */
typedef struct tagtm5UNIT_G3DM tm5UNIT_G3DM;
struct tagtm5UNIT_G3DM
{
  VOID (*Init)( tm5UNIT *Unit, tm5ANIM *Anim );
  VOID (*Close)( tm5UNIT *Unit, tm5ANIM *Anim );
  VOID (*Response)( tm5UNIT *Unit, tm5ANIM *Anim );
  VOID (*Render)( tm5UNIT *Unit, tm5ANIM *Anim );
  
  CHAR Filename[512];
  tm5PRIMS Model;
  BOOL Normalized;
  VEC Pos;
};

static VOID Init( tm5UNIT *Unit, tm5ANIM *Anim )
{
  tm5UNIT_G3DM *rUnit = (tm5UNIT_G3DM*)(Unit);
  
  TM5_RndPrimsLoad(&rUnit->Model, rUnit->Filename, rUnit->Normalized);
}

static VOID Close( tm5UNIT *Unit, tm5ANIM *Anim )
{
  tm5UNIT_G3DM *rUnit = (tm5UNIT_G3DM*)(Unit);
  
  TM5_RndPrimsFree(&rUnit->Model);
}

static VOID Render( tm5UNIT *Unit, tm5ANIM *Anim )
{
  tm5UNIT_G3DM *rUnit = (tm5UNIT_G3DM*)(Unit);

  TM5_RndPrimsDraw(&(rUnit->Model), MatrTranslate(rUnit->Pos));
}

tm5UNIT* TM5_UnitCreateG3DM( CHAR *Filename, BOOL Normalize )
{
  tm5UNIT_G3DM *NewUnit = TM5_AnimCreateUnit(sizeof(tm5UNIT_G3DM));
  CHAR Buffer[512];

  sprintf(Buffer, "bin/models/%s", Filename);
  strcpy(NewUnit->Filename, Buffer);
  NewUnit->Normalized = Normalize;
  NewUnit->Pos = VecSet3(0, 0, 0);
  NewUnit->Init = Init;
  NewUnit->Close = Close;
  NewUnit->Render = Render;

  return (VOID *)NewUnit;
}

/* End of 'g3dm_unit.c' file */
