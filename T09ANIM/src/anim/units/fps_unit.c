/* FILE NAME: fps_unit.c
 * PROGRAMMER: TM5
 * DATE: 10.06.2026
 * PURPOSE: Implement animation system.
 */

#include <math.h>
#include <stdio.h>

#include "anim/units/units.h"

static VOID Render( tm5UNIT *Unit, tm5ANIM *Anim )
{
  static CHAR Buffer[128];
  
  sprintf(Buffer, "FPS: %3.1f", Anim->FPS);
  SetWindowText(TM5_hRndWnd, Buffer);
}

tm5UNIT* TM5_UnitCreateFPS( VOID )
{
  tm5UNIT *NewUnit = TM5_AnimCreateUnit(sizeof(tm5UNIT));

  NewUnit->Render = Render;

  return (VOID *)NewUnit;
}

/* End of 'fps_unit.c' file */
