/* FILE NAME: sphere_unit.c
 * PROGRAMMER: TM5
 * DATE: 10.06.2026
 * PURPOSE: Implement animation system.
 */

#include <math.h>

#include "anim/units/units.h"

/* Furry Unit */
typedef struct tagtm5UNIT_SPHERE tm5UNIT_SPHERE;
struct tagtm5UNIT_SPHERE
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
  tm5UNIT_SPHERE *rUnit = (tm5UNIT_SPHERE*)(Unit);
  
  TM5_RndPrimCreateSphere(&(rUnit->Model), 1, 20, 20);
  rUnit->Pos = VecSet3(0, 0, 0);
}

static VOID Close( tm5UNIT *Unit, tm5ANIM *Anim )
{
  tm5UNIT_SPHERE *rUnit = (tm5UNIT_SPHERE*)(Unit);
  
  TM5_RndPrimFree(&(rUnit->Model));
  memset(rUnit, 0, sizeof(tm5UNIT_SPHERE));
}

static VOID Response( tm5UNIT *Unit, tm5ANIM *Anim )
{
  static DBL Ticks = 0;
  tm5UNIT_SPHERE *rUnit = (tm5UNIT_SPHERE*)(Unit);
  
  rUnit->Pos.Y = sin(Ticks);
  Ticks += 0.004;
}

static VOID Render( tm5UNIT *Unit, tm5ANIM *Anim )
{
  tm5UNIT_SPHERE *rUnit = (tm5UNIT_SPHERE*)(Unit);
  
  SelectObject(Anim->hDC, GetStockObject(DC_BRUSH));
  SelectObject(Anim->hDC, GetStockObject(DC_PEN));
  SetDCBrushColor(Anim->hDC, RGB(255, 128, 32));
  SetDCPenColor(Anim->hDC, RGB(255, 128, 32));

  TM5_RndPrimDraw(&(rUnit->Model), MatrTranslate(rUnit->Pos));
}

tm5UNIT* TM5_UnitCreateSphere( VOID )
{
  tm5UNIT_SPHERE *NewUnit = TM5_AnimCreateUnit(sizeof(tm5UNIT_SPHERE));

  NewUnit->Init = Init;
  NewUnit->Close = Close;
  NewUnit->Response = Response;
  NewUnit->Render = Render;

  return (VOID *)NewUnit;
}

/* End of 'sphere_unit.c' file */
