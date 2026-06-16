/* FILE NAME: height_map_unit.c
 * PROGRAMMER: TM5
 * DATE: 10.06.2026
 * PURPOSE: Implement animation system.
 */

#include "anim/units/units.h"

/*  Unit */
typedef struct tagtm5UNIT_HEIGHT_MAP tm5UNIT_HEIGHT_MAP;
struct tagtm5UNIT_HEIGHT_MAP
{
  VOID (*Init)( tm5UNIT *Unit, tm5ANIM *Anim );
  VOID (*Close)( tm5UNIT *Unit, tm5ANIM *Anim );
  VOID (*Response)( tm5UNIT *Unit, tm5ANIM *Anim );
  VOID (*Render)( tm5UNIT *Unit, tm5ANIM *Anim );
  
  CHAR Filename[512];
  tm5PRIM Model;
  VEC Pos;
};

static VOID Init( tm5UNIT *Unit, tm5ANIM *Anim )
{
  tm5UNIT_HEIGHT_MAP *rUnit = (tm5UNIT_HEIGHT_MAP*)(Unit);
  tm5GRID Grid;

  memset(&Grid, 0, sizeof(tm5GRID));
  TM5_RndGridCreateHeightMap(&Grid, rUnit->Filename);
  TM5_RndPrimFromGrid(&(rUnit->Model), &Grid);
}

static VOID Close( tm5UNIT *Unit, tm5ANIM *Anim )
{
  tm5UNIT_HEIGHT_MAP *rUnit = (tm5UNIT_HEIGHT_MAP*)(Unit);
  
  TM5_RndPrimFree(&(rUnit->Model));
  memset(rUnit, 0, sizeof(tm5UNIT_HEIGHT_MAP));
}

static VOID Render( tm5UNIT *Unit, tm5ANIM *Anim )
{
  tm5UNIT_HEIGHT_MAP *rUnit = (tm5UNIT_HEIGHT_MAP*)(Unit);

  TM5_RndPrimDraw(&(rUnit->Model), MatrTranslate(rUnit->Pos));
}

tm5UNIT* TM5_UnitCreateHeightMap( CHAR *Filename )
{
  tm5UNIT_HEIGHT_MAP *NewUnit = TM5_AnimCreateUnit(sizeof(tm5UNIT_HEIGHT_MAP));

  strcpy(NewUnit->Filename, Filename);
  NewUnit->Pos = VecSet3(0, 0, 0);
  NewUnit->Init = Init;
  NewUnit->Close = Close;
  NewUnit->Render = Render;
  NewUnit->Model.ProgId = TM5_RndShdAdd("default");

  return (VOID *)NewUnit;
}

/* End of 'height_map_unit.c' file */
