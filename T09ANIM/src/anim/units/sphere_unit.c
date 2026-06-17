/* FILE NAME: sphere_unit.c
 * PROGRAMMER: TM5
 * DATE: 10.06.2026
 * PURPOSE: Implement animation system.
 */

#include "anim/units/units.h"

/* Sphere Unit */
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
  tm5GRID SphereGrid;

  memset(&SphereGrid, 0, sizeof(tm5GRID));
  TM5_RndGridCreateSphere(&SphereGrid, 1, 20, 20);
  TM5_RndPrimFromGrid(&(rUnit->Model), &SphereGrid);
}

static VOID Close( tm5UNIT *Unit, tm5ANIM *Anim )
{
  tm5UNIT_SPHERE *rUnit = (tm5UNIT_SPHERE*)(Unit);
  
  TM5_RndPrimFree(&(rUnit->Model));
  memset(rUnit, 0, sizeof(tm5UNIT_SPHERE));
}

static VOID Render( tm5UNIT *Unit, tm5ANIM *Anim )
{
  tm5UNIT_SPHERE *rUnit = (tm5UNIT_SPHERE*)(Unit);

  TM5_RndPrimDraw(&(rUnit->Model), MatrTranslate(rUnit->Pos));
}

tm5UNIT* TM5_UnitCreateSphere( VOID )
{
  tm5UNIT_SPHERE *NewUnit = TM5_AnimCreateUnit(sizeof(tm5UNIT_SPHERE));
  tm5MATERIAL Material = TM5_RndMtlGetDef();

  NewUnit->Pos = VecSet3(0, 0, 0);
  NewUnit->Init = Init;
  NewUnit->Close = Close;
  NewUnit->Render = Render;
  NewUnit->Model.MtlNumber = TM5_RndMtlAdd(&Material);

  return (VOID *)NewUnit;
}

/* End of 'sphere_unit.c' file */
