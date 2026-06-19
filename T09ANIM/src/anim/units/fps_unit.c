/* FILE NAME: fps_unit.c
 * PROGRAMMER: TM5
 * DATE: 10.06.2026
 * PURPOSE: Implement animation system.
 */

#include <math.h>
#include <stdio.h>

#include "anim/units/units.h"

static VOID Init( tm5UNIT *Unit, tm5ANIM *Anim )
{
  //TM5_RndFntLoad("bin/fonts/font.g3dm");
}

static VOID Render( tm5UNIT *Unit, tm5ANIM *Anim )
{
  static CHAR Buffer[128];
  
  TM5_RndFntDraw3D(Buffer, VecSet3(0, 0, 0), sprintf(Buffer, "FPS: %3.1f", Anim->FPS));
}

tm5UNIT* TM5_UnitCreateFPS( VOID )
{
  tm5UNIT *NewUnit = TM5_AnimCreateUnit(sizeof(tm5UNIT));

  NewUnit->Init = Init;
  NewUnit->Render = Render;

  return (VOID *)NewUnit;
}

/* End of 'fps_unit.c' file */
