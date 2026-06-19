/* FILE NAME: rndres.c
 * PROGRAMMER: TM5
 * DATE: 09.06.2026
 * PURPOSE: Implement renderer.
 */

#include "anim/rnd/res/rndres.h"

VOID TM5_RndResInit( VOID )
{
  TM5_RndShdInit();
  TM5_RndTexInit();
  TM5_RndMtlInit();
  TM5_RndFntInit();
}

VOID TM5_RndResClose( VOID )
{
  TM5_RndFntClose();
  TM5_RndMtlClose();
  TM5_RndTexClose();
  TM5_RndShdClose();
}

VOID TM5_RndResBindTexture( INT MtlNum, INT TexNum )
{
  INT i;

  for (i = 0; i < 8; i++)
    if (TM5_RndMaterials[MtlNum].Textures[i] == -1)
    {
      TM5_RndMaterials[MtlNum].Textures[i] = TexNum;
      break;
    }
}

/* End of 'rndres.c' file */