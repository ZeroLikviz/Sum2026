/* FILE NAME: texture_rect_unit.c
 * PROGRAMMER: TM5
 * DATE: 10.06.2026
 * PURPOSE: Implement animation system.
 */

#include "anim/units/units.h"

/* Sphere Unit */
typedef struct tagtm5UNIT_TEXTURE_RECT tm5UNIT_TEXTURE_RECT;
struct tagtm5UNIT_TEXTURE_RECT
{
  VOID (*Init)( tm5UNIT *Unit, tm5ANIM *Anim );
  VOID (*Close)( tm5UNIT *Unit, tm5ANIM *Anim );
  VOID (*Response)( tm5UNIT *Unit, tm5ANIM *Anim );
  VOID (*Render)( tm5UNIT *Unit, tm5ANIM *Anim );
  
  CHAR TextureName[TM5_STR_MAX];
  tm5PRIM Model;
  VEC Pos;
};

static VOID Init( tm5UNIT *Unit, tm5ANIM *Anim )
{
  tm5UNIT_TEXTURE_RECT *rUnit = (tm5UNIT_TEXTURE_RECT*)(Unit);
  tm5MATERIAL Material = TM5_RndMtlGetDef();

  TM5_RndPrimCreateSquare(&rUnit->Model, 2, VecSet3(1, 0, 1));
  Material.ShaderNumber = TM5_RndShdAdd("texture");
  rUnit->Model.MtlNumber = TM5_RndMtlAdd(&Material);
  TM5_RndResBindTexture(rUnit->Model.MtlNumber, TM5_RndTexAdd(rUnit->TextureName));
}

static VOID Close( tm5UNIT *Unit, tm5ANIM *Anim )
{
  tm5UNIT_TEXTURE_RECT *rUnit = (tm5UNIT_TEXTURE_RECT*)(Unit);
  
  TM5_RndPrimFree(&(rUnit->Model));
  memset(rUnit, 0, sizeof(tm5UNIT_TEXTURE_RECT));
}

static VOID Render( tm5UNIT *Unit, tm5ANIM *Anim )
{
  tm5UNIT_TEXTURE_RECT *rUnit = (tm5UNIT_TEXTURE_RECT*)(Unit);

  TM5_RndPrimDraw(&(rUnit->Model), MatrTranslate(rUnit->Pos));
}

tm5UNIT* TM5_UnitCreateTextureRect( CHAR *Filename )
{
  tm5UNIT_TEXTURE_RECT *NewUnit = TM5_AnimCreateUnit(sizeof(tm5UNIT_TEXTURE_RECT));

  NewUnit->Pos = VecSet3(0, 0, 0);
  NewUnit->Init = Init;
  NewUnit->Close = Close;
  NewUnit->Render = Render;
  strcpy(NewUnit->TextureName, Filename);

  return (VOID *)NewUnit;
}

/* End of 'texture_rect_unit.c' file */
