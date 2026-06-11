/* FILE NAME: anim.h
 * PROGRAMMER: TM5
 * DATE: 10.06.2026
 * PURPOSE: Implement animation system.
 */

#ifndef __anim_h_
#define __anim_h_
#include <windows.h>

#include "def.h"

#define MAX_UNITS 1024      

/* Type Declarations */
typedef struct tagtm5ANIM tm5ANIM;
typedef struct tagtm5UNIT tm5UNIT;

/* Anim Structure */
struct tagtm5ANIM
{
  INT NumOfU;
  tm5UNIT *Units[MAX_UNITS];
  HWND hWnd;
  HDC hDC;
  INT W, H;
  BOOL IsActive;

  DOUBLE
    GlobalTime,
    GlobalDeltaTime,
    Time,
    DeltaTime,
    FPS;
  BOOL IsPause;

  BYTE Keys[256];
  BYTE KeysClick[256];
  BYTE KeysOld[256];
  BOOL ProcessKeyboard;

  INT Mx, My, Mz, Mdx, Mdy, Mdz;

  BYTE JBut[32], JButOld[32], JButClick[32];
  INT JPov;
  DBL JX, JY, JZ, JR;
};

/* Unit Structure */
struct tagtm5UNIT
{
  VOID (*Init)( tm5UNIT *Unit, tm5ANIM *Anim );
  VOID (*Close)( tm5UNIT *Unit, tm5ANIM *Anim );
  VOID (*Response)( tm5UNIT *Unit, tm5ANIM *Anim );
  VOID (*Render)( tm5UNIT *Unit, tm5ANIM *Anim );
};

VOID TM5_AnimInit( HWND hWnd );
VOID TM5_AnimClose( VOID );
VOID TM5_AnimResize( INT W, INT H );
VOID TM5_AnimCopyFrame( HDC hDC );
VOID TM5_AnimRender( VOID );
VOID TM5_AnimUnitAdd( tm5UNIT *Unit );
VOID* TM5_AnimCreateUnit( INT Size );
VOID TM5_AnimExit();
VOID TM5_AnimFlipFullScreen( VOID );

extern tm5ANIM TM5_Animation;

#endif /* __anim_h_ */

/* End of 'anim.h' file */