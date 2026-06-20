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
  
  tm5GRID Grid;
  CHAR Filename[512];
  FLT Size;
  tm5PRIM Model;
  VEC Pos;
};

static VOID Init( tm5UNIT *Unit, tm5ANIM *Anim )
{
  tm5UNIT_HEIGHT_MAP *rUnit = (tm5UNIT_HEIGHT_MAP*)(Unit);
  tm5MATERIAL Material = TM5_RndMtlGetStock("Silver");
  tm5GRID Grid;

  Material.ShaderNumber = TM5_RndShdAdd("snow");
  Material.Textures[0] = TM5_RndTexAdd("ground.bmp");
  memset(&Grid, 0, sizeof(tm5GRID));
  TM5_RndGridCreateHeightMap(&Grid, rUnit->Filename, rUnit->Size);
  TM5_RndPrimFromGrid(&(rUnit->Model), &Grid);
  TM5_RndGridCreateHeightMap(&Grid, rUnit->Filename, rUnit->Size);
  rUnit->Grid = Grid;
  rUnit->Model.MtlNumber = TM5_RndMtlAdd(&Material);
  Anim->HeightMapId = Anim->NumOfU - 1;
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

  TM5_RndShdAddonI[4] = TRUE;
  TM5_RndPrimDraw(&(rUnit->Model), MatrTranslate(rUnit->Pos));
  TM5_RndShdAddonI[4] = FALSE;
}

tm5UNIT* TM5_UnitCreateHeightMap( CHAR *Filename, FLT Size )
{
  tm5UNIT_HEIGHT_MAP *NewUnit = TM5_AnimCreateUnit(sizeof(tm5UNIT_HEIGHT_MAP));

  strcpy(NewUnit->Filename, Filename);
  NewUnit->Pos = VecSet3(0, 0, 0);
  NewUnit->Init = Init;
  NewUnit->Close = Close;
  NewUnit->Render = Render;
  NewUnit->Size = Size;

  return (VOID *)NewUnit;
}

/* End of 'height_map_unit.c' file */
