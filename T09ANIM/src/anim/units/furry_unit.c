/* FILE NAME: furry_unit.c
 * PROGRAMMER: TM5
 * DATE: 10.06.2026
 * PURPOSE: Implement animation system.
 */

#include "anim/units/units.h"

/* Furry Unit */
typedef struct tagtm5UNIT_FURRY tm5UNIT_FURRY;
struct tagtm5UNIT_FURRY
{
  VOID (*Init)( tm5UNIT *Unit, tm5ANIM *Anim );
  VOID (*Close)( tm5UNIT *Unit, tm5ANIM *Anim );
  VOID (*Response)( tm5UNIT *Unit, tm5ANIM *Anim );
  VOID (*Render)( tm5UNIT *Unit, tm5ANIM *Anim );
  
  tm5PRIM Model;
  VEC Pos;
};

static VOID Init( tm5UNIT *Unit, tm5ANIM *Anim )
{
  tm5UNIT_FURRY *rUnit = (tm5UNIT_FURRY*)(Unit);
  
  TM5_RndPrimLoad(&(rUnit->Model), "bin/models/real_furry.obj");
  
  rUnit->Pos = VecSet3(0, 0, 0);
}

static VOID Close( tm5UNIT *Unit, tm5ANIM *Anim )
{
  tm5UNIT_FURRY *rUnit = (tm5UNIT_FURRY*)(Unit);
  
  TM5_RndPrimFree(&(rUnit->Model));
  memset(rUnit, 0, sizeof(tm5UNIT_FURRY));
}

static VOID Response( tm5UNIT *Unit, tm5ANIM *Anim )
{
  tm5UNIT_FURRY *rUnit = (tm5UNIT_FURRY*)(Unit);
}

static VOID Render( tm5UNIT *Unit, tm5ANIM *Anim )
{
  tm5UNIT_FURRY *rUnit = (tm5UNIT_FURRY*)(Unit);
  
  SelectObject(Anim->hDC, GetStockObject(DC_BRUSH));
  SelectObject(Anim->hDC, GetStockObject(DC_PEN));
  SetDCBrushColor(Anim->hDC, RGB(255, 128, 32));
  SetDCPenColor(Anim->hDC, RGB(255, 128, 32));

  TM5_RndPrimDraw(&(rUnit->Model), MatrTranslate(rUnit->Pos));
}

tm5UNIT* TM5_UnitCreateFurry( VOID )
{
  tm5UNIT_FURRY *NewUnit = TM5_AnimCreateUnit(sizeof(tm5UNIT_FURRY));
  tm5MATERIAL Material = TM5_RndMtlGetDef();

  NewUnit->Init = Init;
  NewUnit->Close = Close;
  NewUnit->Response = Response;
  NewUnit->Render = Render;
  NewUnit->Model.MtlNumber = TM5_RndMtlAdd(&Material);

  return (VOID *)NewUnit;
}

/* End of 'furry_unit.c' file */
