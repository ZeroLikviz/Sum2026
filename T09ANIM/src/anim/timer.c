/* FILE NAME: timer.c
 * PROGRAMMER: TM5
 * DATE: 08.06.2026
 * PURPOSE: Syncronise time.
 */

#include <windows.h>
#include "timer.h"

typedef unsigned long long UINT64;
/* Local values */
static UINT64
    StartTime,    /* Start program time */
    OldTime,      /* Previous frame time */
    OldFPSTime,   /* Old time FPS measurement */
    PauseTime,    /* Time during pause period */
    TimePerSec,   /* Timer resolution */
    FrameCount;   /* Frames counter */

/* Global values */
DOUBLE
    GlobalTime,       /* Global time */
    GlobalDeltaTime,  /* Interframe interval */
    Time,             /* Time with pause */
    DeltaTime,        /* Interframe interval with pause */     
    FPS;              /* Frames per second value */
BOOL
    IsPause;          /* Pause flag */

VOID TimerInit( VOID )
{
  LARGE_INTEGER t;

  QueryPerformanceFrequency(&t);
  TimePerSec = t.QuadPart;
  QueryPerformanceCounter(&t);
  StartTime = OldTime = OldFPSTime = t.QuadPart;

  PauseTime = 0;
  FrameCount = 0;
  IsPause = FALSE;
  GlobalTime = GlobalDeltaTime = 0;
  FPS = 0;
}  

VOID TimerResponse( VOID )
{
  LARGE_INTEGER t;

  QueryPerformanceCounter(&t);

  /* Global time */
  GlobalTime = (DOUBLE)(t.QuadPart - StartTime) / TimePerSec;
  GlobalDeltaTime = (DOUBLE)(t.QuadPart - OldTime) / TimePerSec;
  /* Time with pause */
  if (!IsPause)
  {
    Time = (DOUBLE)(t.QuadPart - PauseTime - StartTime) / TimePerSec;
    DeltaTime = GlobalDeltaTime;
  }
  else
  {
    DeltaTime = 0;
    PauseTime += t.QuadPart - OldTime;
  }

  /* FPS */
  FrameCount++;
  if (t.QuadPart - OldFPSTime > 3 * TimePerSec)
  {
    FPS = FrameCount * TimePerSec / (DOUBLE)(t.QuadPart - OldFPSTime);
    OldFPSTime = t.QuadPart;
    FrameCount = 0;
  }
  OldTime = t.QuadPart;
}
/* End of 'timer.c' file */