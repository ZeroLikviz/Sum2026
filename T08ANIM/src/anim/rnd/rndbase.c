/* FILE NAME: rndbase.c
 * PROGRAMMER: TM5
 * DATE: 09.06.2026
 * PURPOSE: Implement renderer.
 */
#include "anim/rnd/rnd.h"
#include <stdlib.h>
#include <string.h>

/***  ////////
 ***  Renderer
 ***/ ////////

VOID TM5_RndInit( HWND hWnd )
{
  HDC hDC = GetDC(hWnd);
  TM5_hRndWnd = hWnd;
  
  TM5_RndFrameW = 0;
  TM5_RndFrameH = 0;
  TM5_hRndBmFrame = NULL;
  TM5_hRndDCFrame = CreateCompatibleDC(hDC);
  TM5_RndCamSet(VecSet3(0, 0, 4), VecSet3(0, 0, 0), VecSet3(0, 1, 0));

  ReleaseDC(hWnd, hDC);
}

VOID TM5_RndClose( VOID )
{
  DeleteObject(&TM5_hRndBmFrame);
  DeleteDC(TM5_hRndDCFrame);
}

VOID TM5_RndResize( INT W, INT H )
{
  HDC hDC = GetDC(TM5_hRndWnd);

  if (TM5_hRndBmFrame != NULL)
    DeleteObject(TM5_hRndBmFrame);
  TM5_hRndBmFrame = CreateCompatibleBitmap(hDC, W, H);
  ReleaseDC(TM5_hRndWnd, hDC);		

  SelectObject(TM5_hRndDCFrame, TM5_hRndBmFrame);

  TM5_RndFrameW = W;
  TM5_RndFrameH = H;

  TM5_RndProjSet();
}


VOID TM5_RndCopyFrame( HDC hDC )
{
  BitBlt(hDC, 0, 0, TM5_RndFrameW, TM5_RndFrameH, TM5_hRndDCFrame, 0, 0, SRCCOPY);
}

VOID TM5_RndStart( VOID )
{
  SelectObject(TM5_hRndDCFrame, GetStockObject(DC_BRUSH));
  SelectObject(TM5_hRndDCFrame, GetStockObject(DC_PEN));
  SetDCBrushColor(TM5_hRndDCFrame, RGB(128, 128, 128));
  SetDCPenColor(TM5_hRndDCFrame, RGB(128, 128, 128));

  Rectangle(TM5_hRndDCFrame, 0, 0, TM5_RndFrameW, TM5_RndFrameH);
  
  SetDCBrushColor(TM5_hRndDCFrame, RGB(255, 0, 0));
  SetDCPenColor(TM5_hRndDCFrame, RGB(0, 255, 0));
  Ellipse(TM5_hRndDCFrame, TM5_RndFrameW / 2 - 15, TM5_RndFrameH / 2 - 15,
          TM5_RndFrameW / 2 + 15, TM5_RndFrameH / 2 + 15);
}

VOID TM5_RndEnd( VOID )
{
}

/***  ///////////////////
 ***  Camera & Primitives
 ***/ ///////////////////

VOID TM5_RndProjSet( VOID )
{
  DBL rx, ry;

  rx = ry = TM5_RndProjSize;
  /* Correct aspect ratio */
  if (TM5_RndFrameW > TM5_RndFrameH)
    rx *= (DBL)TM5_RndFrameW / TM5_RndFrameH;
  else
    ry *= (DBL)TM5_RndFrameH / TM5_RndFrameW;
  TM5_RndMatrProj =
    MatrFrustum(-rx / 2, rx / 2, -ry / 2, ry / 2,
      TM5_RndProjDist, TM5_RndProjFarClip);
  TM5_RndMatrVP = MatrMulMatr(TM5_RndMatrView, TM5_RndMatrProj);
}


VOID TM5_RndCamSet( VEC3 Loc, VEC3 At, VEC3 Up )
{
  TM5_RndMatrView = MatrView(Loc, At, Up);
  TM5_RndMatrVP = MatrMulMatr(TM5_RndMatrView, TM5_RndMatrProj);
}

VOID TM5_RndPrimDraw( tm5PRIM *Primitive, MATR World )
{
  INT i;
  MATR wvp = MatrMulMatr(MatrMulMatr(Primitive->Transform, World), TM5_RndMatrVP);
  POINT *pnts;

  if ((pnts = malloc(sizeof(POINT) * Primitive->NumOfV)) == NULL)
    return;

  /* Build vertex projects */
  for (i = 0; i < Primitive->NumOfV; i++)
  {
    VEC3 P = VecMulMatr(Primitive->Vertices[i].Vec, wvp);

    pnts[i].x = (INT)((P.X + 1) * TM5_RndFrameW / 2);
    pnts[i].y = (INT)((-P.Y + 1) * TM5_RndFrameH / 2);
  }

  for (i = 0; i < Primitive->NumOfI; i += 3)
  {
    MoveToEx(TM5_hRndDCFrame, pnts[Primitive->Indexes[i]].x, pnts[Primitive->Indexes[i]].y, NULL); 
    LineTo(TM5_hRndDCFrame, pnts[Primitive->Indexes[i + 1]].x, pnts[Primitive->Indexes[i + 1]].y);
    LineTo(TM5_hRndDCFrame, pnts[Primitive->Indexes[i + 2]].x, pnts[Primitive->Indexes[i + 2]].y);
    LineTo(TM5_hRndDCFrame, pnts[Primitive->Indexes[i]].x, pnts[Primitive->Indexes[i]].y);
  }
  free(pnts);
}

VOID TM5_RndPrimFree( tm5PRIM *Primitive )
{
  free(Primitive->Vertices);
  memset(Primitive, 0, sizeof(tm5VERTEX));
}

BOOL TM5_RndPrimCreate( tm5PRIM *Primitive, INT NofV, INT NofI )
{
  INT size;

  memset(Primitive, 0, sizeof(tm5PRIM));
  size = sizeof(tm5VERTEX) * NofV + sizeof(INT) * NofI;

  if ((Primitive->Vertices = malloc(size)) == NULL)
    return FALSE;
  Primitive->Indexes = (INT *)(Primitive->Vertices + NofV);
  Primitive->NumOfV = NofV;
  Primitive->NumOfI = NofI;
  Primitive->Transform = MatrIdentity();
  memset(Primitive->Vertices, 0, size);
  return TRUE;
}

/* End of 'rndbase.c' file */