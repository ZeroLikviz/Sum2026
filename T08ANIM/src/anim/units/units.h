/* FILE NAME: units.h
 * PROGRAMMER: TM5
 * DATE: 10.06.2026
 * PURPOSE: Implement animation system.
 */

#include "anim/rnd/rnd.h"
#include "anim/anim.h"

VOID NumbInit( tm5UNIT *Unit, tm5ANIM *Anim );
VOID NumbClose( tm5UNIT *Unit, tm5ANIM *Anim );
VOID NumbResponse( tm5UNIT *Unit, tm5ANIM *Anim );
VOID NumbRender( tm5UNIT *Unit, tm5ANIM *Anim );

tm5UNIT* TM5_UnitCreateFurry( VOID );

/* End of 'units.h' file */
