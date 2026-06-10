/* FILE NAME: anim.c
 * PROGRAMMER: TM5
 * DATE: 10.06.2026
 * PURPOSE: Implement animation system.
 */

#include <string.h>

#include "anim/rnd/rnd.h"
#include "anim/anim.h"

HDC TM5_hAnimDC;
static tm5ANIM TM5_Animation;

VOID TM5_AnimInit( HWND hWnd )
{
  TM5_RndInit(hWnd);
  TM5_hAnimDC = TM5_hRndDCFrame;

  memset(&TM5_Animation, 0, sizeof(tm5ANIM));
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
}

VOID TM5_AnimCopyFrame( HDC hDC )
{
  TM5_RndCopyFrame(hDC);
}

VOID TM5_AnimRender( VOID )
{
  INT i;

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

/* End of 'anim.c' file */