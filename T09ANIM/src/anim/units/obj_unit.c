/* FILE NAME: obj_unit.c
 * PROGRAMMER: TM5
 * DATE: 10.06.2026
 * PURPOSE: Implement animation system.
 */

#include "anim/units/units.h"

#define MaxPathLen 2048

/* Obj Unit */
typedef struct tagtm5UNIT_OBJ tm5UNIT_OBJ;
struct tagtm5UNIT_OBJ
{
  VOID (*Init)( tm5UNIT *Unit, tm5ANIM *Anim );
  VOID (*Close)( tm5UNIT *Unit, tm5ANIM *Anim );
  VOID (*Response)( tm5UNIT *Unit, tm5ANIM *Anim );
  VOID (*Render)( tm5UNIT *Unit, tm5ANIM *Anim );
  
  CHAR ModelPath[MaxPathLen];
  tm5PRIM Model;
  VEC Pos;
};

static VOID Init( tm5UNIT *Unit, tm5ANIM *Anim )
{
  tm5UNIT_OBJ *rUnit = (tm5UNIT_OBJ*)(Unit);
  
  TM5_RndPrimLoad(&(rUnit->Model), rUnit->ModelPath);
}

static VOID Close( tm5UNIT *Unit, tm5ANIM *Anim )
{
  tm5UNIT_OBJ *rUnit = (tm5UNIT_OBJ*)(Unit);
  
  TM5_RndPrimFree(&(rUnit->Model));
  memset(rUnit, 0, sizeof(tm5UNIT_OBJ));
}

static VOID Render( tm5UNIT *Unit, tm5ANIM *Anim )
{
  tm5UNIT_OBJ *rUnit = (tm5UNIT_OBJ*)(Unit);

  TM5_RndPrimDraw(&(rUnit->Model), MatrTranslate(rUnit->Pos));
}

tm5UNIT* TM5_UnitCreateObj( CHAR *Path, VEC Pos )
{
  tm5UNIT_OBJ *NewUnit = TM5_AnimCreateUnit(sizeof(tm5UNIT_OBJ));
  tm5MATERIAL Material = TM5_RndMtlGetDef();

  strcpy(NewUnit->ModelPath, Path);
  NewUnit->Pos = Pos;
  NewUnit->Init = Init;
  NewUnit->Close = Close;
  NewUnit->Render = Render;
  NewUnit->Model.MtlNumber = TM5_RndMtlAdd(&Material);

  return (VOID *)NewUnit;
}

/* End of 'obj_unit.c' file */
