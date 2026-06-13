/* FILE NAME: rndres.c
 * PROGRAMMER: TM5
 * DATE: 09.06.2026
 * PURPOSE: Implement renderer.
 */

#include "anim/rnd/res/rndres.h"

VOID TM5_RndResInit( VOID )
{
  TM5_RndShdInit();
}

VOID TM5_RndResClose( VOID )
{
  TM5_RndShdClose();
}

/* End of 'rndres.c' file */