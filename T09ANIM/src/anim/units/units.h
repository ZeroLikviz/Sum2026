/* FILE NAME: units.h
 * PROGRAMMER: TM5
 * DATE: 10.06.2026
 * PURPOSE: Implement animation system.
 */

#include "anim/rnd/rnd.h"
#include "anim/anim.h"
#include "def.h"

tm5UNIT* TM5_UnitCreateFPS( VOID );
tm5UNIT* TM5_UnitCreateControl( VOID );
tm5UNIT* TM5_UnitCreateGame( VOID );
tm5UNIT* TM5_UnitCreateSphere( VOID );
tm5UNIT* TM5_UnitCreateHeightMap( CHAR *Filename );
tm5UNIT* TM5_UnitCreateTextureRect( CHAR *Filename );
tm5UNIT* TM5_UnitCreateG3DM( CHAR *Filename, BOOL Normalize );

/* End of 'units.h' file */
