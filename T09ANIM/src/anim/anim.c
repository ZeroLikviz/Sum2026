/* FILE NAME: anim.c
 * PROGRAMMER: TM5
 * DATE: 10.06.2026
 * PURPOSE: Implement animation system.
 */

#include <string.h>

#include "anim/anim.h"


tm5ANIM TM5_Animation;

VOID InputInit( VOID )
{
  TM5_AnimUnitAdd(TM5_UnitCreateControl());
}

VOID TM5_AnimInit( HWND hWnd )
{
  memset(&TM5_Animation, 0, sizeof(tm5ANIM));

  TM5_RndInit(hWnd);
  TM5_Animation.hWnd = hWnd;
  TM5_Animation.hDC = TM5_hRndDC;
  
  TM5_Animation.ProcessInput    = TRUE;
  TM5_Animation.ProcessKeyboard = TRUE;
  TM5_Animation.ProcessMouse    = TRUE;
  TM5_Animation.ProcessJoystick = TRUE;
  TM5_Animation.IsActive        = TRUE;
  TM5_Animation.IsPause         = FALSE;
  
  TM5_Animation.Mouse.Capture = FALSE;
  TM5_Animation.Mouse.Visible = TRUE;
  InputInit();

  TimerInit();
}

VOID TM5_AnimClose( VOID )
{
  INT i;

  for (i = 0; i < TM5_Animation.NumOfU; i++)
    TM5_Animation.Units[i]->Close(TM5_Animation.Units[i], &TM5_Animation);
  memset(&TM5_Animation, 0, sizeof(tm5ANIM));

  TM5_RndClose();
}

VOID TM5_AnimResize( INT W, INT H )
{
  TM5_RndResize(W, H);
  TM5_Animation.W = W;
  TM5_Animation.H = H;
}

VOID TM5_AnimCopyFrame( VOID )
{
  TM5_RndCopyFrame();
}

VOID TM5_UpdateAnimTimer( VOID )
{
  IsPause = TM5_Animation.IsPause && TM5_Animation.IsActive;

  TimerResponse();

  TM5_Animation.GlobalTime = GlobalTime;
  TM5_Animation.GlobalDeltaTime = GlobalDeltaTime;
  TM5_Animation.Time = Time;
  TM5_Animation.DeltaTime = DeltaTime;
  TM5_Animation.FPS = FPS;
}

VOID TM5_AnimRender( VOID )
{
  INT i;

  TM5_UpdateAnimTimer();
  if (!TM5_Animation.IsActive)
    return;

  for (i = 0; i < TM5_Animation.NumOfU; i++)
    TM5_Animation.Units[i]->Response(TM5_Animation.Units[i], &TM5_Animation);

  TM5_RndStart();
  for (i = 0; i < TM5_Animation.NumOfU; i++)
    TM5_Animation.Units[i]->Render(TM5_Animation.Units[i], &TM5_Animation);
  TM5_RndEnd();
}

VOID TM5_AnimUnitAdd( tm5UNIT *Unit )
{
  if (TM5_Animation.NumOfU < MAX_UNITS)
  {
    TM5_Animation.Units[TM5_Animation.NumOfU++] = Unit;
    Unit->Init(Unit, &TM5_Animation);
  }
}

VOID TM5_AnimExit()
{
  static BOOL IsExiting = FALSE;

  if (IsExiting)
    return;

  IsExiting = TRUE;
  TM5_Animation.ProcessInput = FALSE;
  TM5_Animation.IsPause = TRUE;
  if (MessageBox(TM5_Animation.hWnd, "Are you sure?", "Exit", MB_YESNO) == IDYES)
    PostMessage(TM5_Animation.hWnd, WM_QUIT, 0, 0);
  TM5_Animation.IsPause = FALSE;
  TM5_Animation.ProcessInput = TRUE;
  IsExiting = FALSE;
}

VOID TM5_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRect;

  if (!IsFullScreen)
  {
    HMONITOR hmon;
    MONITORINFO mi;
    RECT rc;

    /* Save old window size and position */
    GetWindowRect(TM5_Animation.hWnd, &SaveRect);
    
    /* Obtain nearest monitor */
    hmon = MonitorFromWindow(TM5_Animation.hWnd, MONITOR_DEFAULTTONEAREST);
    mi.cbSize = sizeof(mi);
    GetMonitorInfo(hmon, &mi);
    
    /* Go to full screen mode */
    rc = mi.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(TM5_Animation.hWnd, GWL_STYLE), FALSE);

    /* Expand window */
    SetWindowPos(TM5_Animation.hWnd, HWND_TOP,
      rc.left, rc.top,
      rc.right - rc.left,
      rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
  }
  else
    /* Restore from full screen mode */
    SetWindowPos(TM5_Animation.hWnd, HWND_TOP,
      SaveRect.left, SaveRect.top,
      SaveRect.right - SaveRect.left,
      SaveRect.bottom - SaveRect.top,
      SWP_NOOWNERZORDER);
  IsFullScreen = !IsFullScreen;
}

/* End of 'anim.c' file */