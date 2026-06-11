/* FILE NAME: torus_unit.c
 * PROGRAMMER: TM5
 * DATE: 10.06.2026
 * PURPOSE: Implement animation system.
 */

#include <math.h>

#include "anim/units/units.h"

/* Furry Unit */
typedef struct tagtm5UNIT_TORUS tm5UNIT_TORUS;
struct tagtm5UNIT_TORUS
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
  tm5UNIT_TORUS *rUnit = (tm5UNIT_TORUS*)(Unit);
  
  TM5_RndPrimCreateTorus(&(rUnit->Model), 0.7, 0.04, 40, 20);
  rUnit->Pos = VecSet3(0, 0, 0);
}

static VOID Close( tm5UNIT *Unit, tm5ANIM *Anim )
{
  tm5UNIT_TORUS *rUnit = (tm5UNIT_TORUS*)(Unit);
  
  TM5_RndPrimFree(&(rUnit->Model));
  memset(rUnit, 0, sizeof(tm5UNIT_TORUS));
}

static VOID Response( tm5UNIT *Unit, tm5ANIM *Anim )
{
  tm5UNIT_TORUS *rUnit = (tm5UNIT_TORUS*)(Unit);

  rUnit->Model.Transform = MatrMulMatr(rUnit->Model.Transform, MatrRotateX(10 * Anim->DeltaTime));
}

static VOID Render( tm5UNIT *Unit, tm5ANIM *Anim )
{
  tm5UNIT_TORUS *rUnit = (tm5UNIT_TORUS*)(Unit);
  
  SelectObject(Anim->hDC, GetStockObject(DC_BRUSH));
  SelectObject(Anim->hDC, GetStockObject(DC_PEN));
  SetDCBrushColor(Anim->hDC, RGB(255, 128, 32));
  SetDCPenColor(Anim->hDC, RGB(255, 128, 32));

  TM5_RndPrimDraw(&(rUnit->Model), MatrTranslate(rUnit->Pos));
}

tm5UNIT* TM5_UnitCreateTorus( VOID )
{
  tm5UNIT_TORUS *NewUnit = TM5_AnimCreateUnit(sizeof(tm5UNIT_TORUS));

  NewUnit->Init = Init;
  NewUnit->Close = Close;
  NewUnit->Response = Response;
  NewUnit->Render = Render;

  return (VOID *)NewUnit;
}

/* End of 'torus_unit.c' file */
