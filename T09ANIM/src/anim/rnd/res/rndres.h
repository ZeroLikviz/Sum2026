/* FILE NAME: rndres.h
 * PROGRAMMER: TM5
 * DATE: 09.06.2026
 * PURPOSE: Implement renderer.
 */
#ifndef __rndres_h_
#define __rndres_h_

#include "def.h"

VOID TM5_RndResInit( VOID );
VOID TM5_RndResClose( VOID );

VOID TM5_RndShdInit( VOID ); 
VOID TM5_RndShdClose( VOID ); 
VOID TM5_RndShdUpdate( VOID );

extern UINT TM5_RndProgId;

/* END OF 'rndshd.c' FILE */

#endif

/* End of 'rndres.h' file */