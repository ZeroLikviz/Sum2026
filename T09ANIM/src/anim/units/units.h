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
tm5UNIT* TM5_UnitCreateHeightMap( CHAR *Filename, FLT Size );
tm5UNIT* TM5_UnitCreateTextureRect( CHAR *Filename );
tm5UNIT* TM5_UnitCreateG3DM( CHAR *Filename, BOOL Normalize );
tm5UNIT* TM5_UnitCreateForest( INT Trees, FLT Size );
tm5UNIT* TM5_UnitCreateCustards( INT Custards, FLT Size );
tm5UNIT* TM5_UnitCreateTinky( VOID );
tm5UNIT* TM5_UnitCreateCamsmooth( VOID );

VEC TM5_RndUnitForestCollision( VEC Pos );

/* End of 'units.h' file */
