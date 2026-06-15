/* FILE NAME: rndres.h
 * PROGRAMMER: TM5
 * DATE: 09.06.2026
 * PURPOSE: Implement renderer.
 */
#ifndef __rndres_h_
#define __rndres_h_

#include "def.h"

#define TM5_MAX_SHADERS 8
#define TM5_STR_MAX 512

typedef struct tagtm5SHADER
{
  CHAR Name[TM5_STR_MAX];
  INT ProgId;
} tm5SHADER;

VOID TM5_RndResInit( VOID );
VOID TM5_RndResClose( VOID );

VOID TM5_RndShdInit( VOID ); 
VOID TM5_RndShdClose( VOID ); 
VOID TM5_RndShdUpdate( VOID );
INT TM5_RndShdAdd( CHAR *ShaderFileNamePrefix );

extern tm5SHADER TM5_RndShaders[TM5_MAX_SHADERS];
extern INT TM5_RndShadersSize;

/* END OF 'rndshd.c' FILE */

#endif

/* End of 'rndres.h' file */