/* FILE NAME: cylinder_unit.c
 * PROGRAMMER: TM5
 * DATE: 10.06.2026
 * PURPOSE: Implement animation system.
 */

#include <math.h>

#include "anim/units/units.h"

/* Furry Unit */
typedef struct tagtm5UNIT_CYLINDER tm5UNIT_CYLINDER;
struct tagtm5UNIT_CYLINDER
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
  tm5UNIT_CYLINDER *rUnit = (tm5UNIT_CYLINDER*)(Unit);
  
  TM5_RndPrimCreateCylinder(&(rUnit->Model), 1, 2, 12);
  rUnit->Pos = VecSet3(0, 0, 0);
}

static VOID Close( tm5UNIT *Unit, tm5ANIM *Anim )
{
  tm5UNIT_CYLINDER *rUnit = (tm5UNIT_CYLINDER*)(Unit);
  
  TM5_RndPrimFree(&(rUnit->Model));
  memset(rUnit, 0, sizeof(tm5UNIT_CYLINDER));
}

static VOID Render( tm5UNIT *Unit, tm5ANIM *Anim )
{
  tm5UNIT_CYLINDER *rUnit = (tm5UNIT_CYLINDER*)(Unit);
  
  SelectObject(Anim->hDC, GetStockObject(DC_BRUSH));
  SelectObject(Anim->hDC, GetStockObject(DC_PEN));
  SetDCBrushColor(Anim->hDC, RGB(255, 128, 32));
  SetDCPenColor(Anim->hDC, RGB(255, 128, 32));

  TM5_RndPrimDraw(&(rUnit->Model), MatrTranslate(rUnit->Pos));
}

tm5UNIT* TM5_UnitCreateCylinder( VOID )
{
  tm5UNIT_CYLINDER *NewUnit = TM5_AnimCreateUnit(sizeof(tm5UNIT_CYLINDER));

  NewUnit->Init = Init;
  NewUnit->Close = Close;
  NewUnit->Render = Render;

  return (VOID *)NewUnit;
}

/* End of 'cylinder_unit.c' file */
