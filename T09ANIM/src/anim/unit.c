/* FILE NAME: unit.c
 * PROGRAMMER: TM5
 * DATE: 10.06.2026
 * PURPOSE: Implement animation system.
 */

#include "anim/units/units.h"
#include <string.h>

static VOID NumbInit( tm5UNIT *Unit, tm5ANIM *Anim )
{
}

static VOID NumbClose( tm5UNIT *Unit, tm5ANIM *Anim )
{
}

static VOID NumbResponse( tm5UNIT *Unit, tm5ANIM *Anim )
{
}

static VOID NumbRender( tm5UNIT *Unit, tm5ANIM *Anim )
{
}

VOID* TM5_AnimCreateUnit( INT Size )
{
  tm5UNIT* NewUnit = malloc(Size);

  if (NewUnit != NULL)
  {
    memset(NewUnit, 0, Size);

    NewUnit->Close = NumbClose;
    NewUnit->Init = NumbInit;
    NewUnit->Render = NumbRender;
    NewUnit->Response = NumbResponse;
  }

  return NewUnit;
}

/* End of 'unit.c' file */