/* FILE NAME: globe.h
 * PROGRAMMER: TM5
 * DATE: 06.06.2026
 * PURPOSE: Trying 3D.
 */
#ifndef __globe_h_
#define __globe_h_

#include <windows.h>
#define HALF_PI 1.5708
#define PI 3.1415926535
#define DPI 6.2831

VOID GLB_Init( DOUBLE R );
VOID GLB_Resize( INT W, INT H );
VOID GLB_Draw( HDC hDC );

#endif
/* END OF 'globe.h' FILE */
