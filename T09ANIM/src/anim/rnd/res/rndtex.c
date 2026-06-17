/* FILE NAME: rndtex.c
 * PROGRAMMER: TM5
 * DATE: 09.06.2026
 * PURPOSE: 3D Animation Project.
 */

#include <stdio.h>
#include "anim/rnd/rnd.h"

tm5TEXTURE TM5_RndTextures[TM5_MAX_TEXTURES]; /* Array of textures */
INT TM5_RndTexturesSize; 

VOID TM5_RndTexInit( VOID );
VOID TM5_RndTexClose( VOID );
INT TM5_RndTexAddImg( CHAR *Name, INT W, INT H, INT C, VOID *Bits );
INT TM5_RndTexAdd( CHAR *FileName );

/* End of 'rndtex.c' file */