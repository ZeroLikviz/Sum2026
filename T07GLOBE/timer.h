/* FILE NAME: timer.h
 * PROGRAMMER: TM5
 * DATE: 08.06.2026
 * PURPOSE: Syncronise time.
 */

#ifndef __timer_h_
#define __timer_h_
#include <windows.h>

extern DOUBLE
    GlobalTime,       /* Global time */
    GlobalDeltaTime,  /* Interframe interval */
    Time,             /* Time with pause */
    DeltaTime,        /* Interframe interval with pause */     
    FPS;              /* Frames per second value */
extern BOOL
    IsPause;          /* Pause flag */

VOID TimerInit( VOID );
VOID TimerResponse( VOID );
#endif