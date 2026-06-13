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
typedef struct tagtm5MOUSE tm5MOUSE;
typedef struct tagtm5ANIM tm5ANIM;
typedef struct tagtm5UNIT tm5UNIT;

#include "anim/rnd/rnd.h"
#include "anim/units/units.h"
#include "timer.h"

/* Mouse Structure */
struct tagtm5MOUSE
{
  INT X, Y, dX, dY;
  INT oldX, oldY;
  BOOL Capture;
  BOOL Visible;
};

/* Animation Structure */
struct tagtm5ANIM
{
  /* Context */
  HWND hWnd;                 /* Main window handler */
  HDC hDC;                   /* DC handler to draw to */
  INT W, H;                  /* Width and heigth parameters of window */

  /* Units */
  INT NumOfU;                /* Number of units */
  tm5UNIT *Units[MAX_UNITS]; /* Pointers to units */

  /* Time */
  DOUBLE GlobalTime;         /* Time from the start of the program */
  DOUBLE GlobalDeltaTime;    /* Interframe time */
  DOUBLE Time;               /* Time with pauses */
  DOUBLE DeltaTime;          /* Interframe time with pauses */
  DOUBLE FPS;                /* Frames per second */

  /* Keyboard*/
  BYTE Keys[256];            /* Keyboard keys */
  BYTE KeysClick[256];       /* Keyboard just pressed keys */
  BYTE KeysRelease[256];     /* Keyboard just released keys */
  BYTE KeysOld[256];         /* Keyboard pressed keys */

  /* Mouse */
  tm5MOUSE Mouse;

  /* Joystick */
  BYTE JBut[32];
  BYTE JButClick[32];
  BYTE JButRelease[32];
  BYTE JButOld[32];
  
  INT JPov;
  DBL JX, JY, JZ, JR;

  /* States */
  BOOL ProcessInput;         /* Bool whether input should be processed at all */
  BOOL ProcessKeyboard;      /* Bool whether keyboard input should be processed */
  BOOL ProcessMouse;         /* Bool whether mouse input should be processed */
  BOOL ProcessJoystick;      /* Bool whether joystick input should be processed */
  BOOL IsPause;              /* Is timer paused */
  BOOL IsActive;             /* Bool whether to do anything */
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
VOID TM5_AnimCopyFrame( VOID );
VOID TM5_AnimRender( VOID );
VOID TM5_AnimUnitAdd( tm5UNIT *Unit );
VOID* TM5_AnimCreateUnit( INT Size );
VOID TM5_AnimExit();
VOID TM5_AnimFlipFullScreen( VOID );

extern tm5ANIM TM5_Animation;

#endif /* __anim_h_ */

/* End of 'anim.h' file */