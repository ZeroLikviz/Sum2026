/* FILE NAME: rndbase.c
 * PROGRAMMER: TM5
 * DATE: 09.06.2026
 * PURPOSE: Implement renderer.
 */
#include "anim/rnd/rnd.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
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
  TM5_RndCamSet(VecSet3(0, 0, 0.001), VecSet3(0, 0, 0), VecSet3(0, 1, 0));

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


VOID TM5_RndCamSet( VEC Loc, VEC At, VEC Up )
{
  TM5_RndMatrView = MatrView(Loc, At, Up);
  TM5_RndMatrVP = MatrMulMatr(TM5_RndMatrView, TM5_RndMatrProj);
}

VOID TM5_RndPrimDraw( tm5PRIM *Primitive, MATR World )
{
  INT i;
  MATR wvp = MatrMulMatr(MatrMulMatr(Primitive->Transform, World), TM5_RndMatrVP);

  for (i = 0; i < Primitive->NumOfV; i++)
  {
    VEC Vec = VecMulMatr(Primitive->Vertices[i].Vec, wvp);

    Primitive->Projections[i].x = (INT)((Vec.X + 1) * TM5_RndFrameW / 2);
    Primitive->Projections[i].y = (INT)((-Vec.Y + 1) * TM5_RndFrameH / 2);
  }

  for (i = 0; i < Primitive->NumOfI; i += 3)
  {
    if ((i / 3) % 1 != 0)
      continue;
    MoveToEx(TM5_hRndDCFrame, Primitive->Projections[Primitive->Indexes[i]].x, Primitive->Projections[Primitive->Indexes[i]].y, NULL); 
    LineTo(TM5_hRndDCFrame, Primitive->Projections[Primitive->Indexes[i + 1]].x, Primitive->Projections[Primitive->Indexes[i + 1]].y);
    LineTo(TM5_hRndDCFrame, Primitive->Projections[Primitive->Indexes[i + 2]].x, Primitive->Projections[Primitive->Indexes[i + 2]].y);
    LineTo(TM5_hRndDCFrame, Primitive->Projections[Primitive->Indexes[i]].x, Primitive->Projections[Primitive->Indexes[i]].y);
  }
}

VOID TM5_RndPrimFree( tm5PRIM *Primitive )
{
  free(Primitive->Vertices);
  memset(Primitive, 0, sizeof(tm5PRIM));
}

BOOL TM5_RndPrimCreate( tm5PRIM *Primitive, INT NofV, INT NofI )
{
  INT size;

  memset(Primitive, 0, sizeof(tm5PRIM));
  size = sizeof(tm5VERTEX) * NofV + sizeof(INT) * NofI + sizeof(POINT) * NofV;

  if ((Primitive->Vertices = malloc(size)) == NULL)
    return FALSE;
  Primitive->Indexes = (INT *)(Primitive->Vertices + NofV);
  Primitive->Projections = (POINT *)(Primitive->Indexes + NofI);
  Primitive->NumOfV = NofV;
  Primitive->NumOfI = NofI;
  Primitive->Transform = MatrIdentity();
  memset(Primitive->Vertices, 0, size);
  return TRUE;
}

INT CountVertexIndexes( CHAR *String )
{
  INT Count = -1;
  INT i = 0;

  while (String[i] && i < 1024)
  {
    while (String[i] && String[i] == ' ' && i < 1024)
      i++;

    if (String[i] != ' ' && String[i])
      Count++;

    while (String[i] && String[i] != ' ' && i < 1024)
      i++;
  }

  return Count;
}

BOOL TM5_RndPrimLoad( tm5PRIM *Primitive, CHAR *FileName )
{
  FILE *F;
  INT nv = 0, nf = 0;
  static CHAR Buffer[3000];

  memset(Primitive, 0, sizeof(tm5PRIM));

  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;

  /* Count vertices and indices */
  while (fgets(Buffer, sizeof(Buffer) - 1, F) != NULL)
  {
    if (Buffer[0] == 'v' && Buffer[1] == ' ')
      nv++;
    else if (Buffer[0] == 'f' && Buffer[1] == ' ')
    {
      INT n = 0;
      CHAR *ptr = Buffer + 2, oldc = ' ';

      while (*ptr != 0)
      {
        if (*ptr != ' ' && oldc == ' ')
          n++;
        oldc = *ptr++;
      }

      nf += n - 2;
    }
  }

  if (!TM5_RndPrimCreate(Primitive, nv, nf * 3))
  {
    fclose(F);
    return FALSE;
  }

  /* Load model */
  rewind(F);
  nv = 0;
  nf = 0;
  while (fgets(Buffer, sizeof(Buffer) - 1, F) != NULL)
  {
    if (Buffer[0] == 'v' && Buffer[1] == ' ')
    {
      DBL x, y, z;

      sscanf(Buffer + 2, "%lf%lf%lf", &x, &y, &z);
      Primitive->Vertices[nv++].Vec = VecSet3(x, y, z);
    }
    else if (Buffer[0] == 'f' && Buffer[1] == ' ')
    {
      INT n, n1, n2, n3;
      INT fvn = 0;
      CHAR *ptr = Buffer + 2, oldc = ' ';

      while (*ptr != 0)
      {
        if (*ptr != ' ' && oldc == ' ')
        {
          sscanf(ptr, "%d", &n);
          if (n > 0)
            n--;
          else
            if (n < 0)
              n += nv;

          if (fvn == 0)
            n1 = n;
          else if (fvn == 1)
            n2 = n;
          else
          {
            n3 = n;

            Primitive->Indexes[nf++] = n1;
            Primitive->Indexes[nf++] = n2;
            Primitive->Indexes[nf++] = n3;

            n2 = n3;
          }
          fvn++;
        }
        oldc = *ptr++;
      }
    }
  }
  fclose(F);
  return TRUE;
}

/* End of 'rndbase.c' file */