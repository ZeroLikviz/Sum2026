/* FILE NAME: rndbase.c
 * PROGRAMMER: TM5
 * DATE: 09.06.2026
 * PURPOSE: Implement renderer.
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "anim/rnd/rnd.h"
#include <wglew.h>
#include <gl/wglext.h>

#pragma comment(lib, "opengl32")

/***  ////////
 ***  Renderer
 ***/ ////////

VOID TM5_RndInit( HWND hWnd )
{
  INT pfdID;
  PIXELFORMATDESCRIPTOR pfd = {0};

  TM5_hRndDC = GetDC(hWnd);
  TM5_hRndWnd = hWnd;

  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  pfdID = ChoosePixelFormat(TM5_hRndDC, &pfd);
 
  DescribePixelFormat(TM5_hRndDC, pfdID, sizeof(pfd), &pfd);
  SetPixelFormat(TM5_hRndDC, pfdID, &pfd);
 
  TM5_hRndGLRC = wglCreateContext(TM5_hRndDC);
  wglMakeCurrent(TM5_hRndDC, TM5_hRndGLRC);
  glEnable(GL_DEPTH_TEST);
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  glewInit();

  TM5_RndFrameW = 0;
  TM5_RndFrameH = 0;
  TM5_RndCamSet(VecSet3(0.0f, 0.0f, 0.001f),
                VecSet3(0.0f, 0.0f, 0.0f),
                VecSet3(0.0f, 1.0f, 0.0f));
}

VOID TM5_RndClose( VOID )
{
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(TM5_hRndGLRC);
  ReleaseDC(TM5_hRndWnd, TM5_hRndDC);
  DeleteDC(TM5_hRndDC);
}

VOID TM5_RndResize( INT W, INT H )
{
  glViewport(0, 0, W, H);
  
  TM5_RndFrameW = W;
  TM5_RndFrameH = H;
  TM5_RndProjSet();
}


VOID TM5_RndCopyFrame( VOID )
{
   SwapBuffers(TM5_hRndDC);
}

VOID TM5_RndStart( VOID )
{
  VEC4 ClearColor = {0.2, 0.2, 0.2, 1};
  FLT DepthClearValue = 1;

  glClearBufferfv(GL_COLOR, 0, &ClearColor.X);
  glClearBufferfv(GL_DEPTH, 0, &DepthClearValue);
}

VOID TM5_RndEnd( VOID )
{
  glFinish();
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
  MATR wvp = MatrMulMatr3(Primitive->Transform, World, TM5_RndMatrVP);
  glLoadMatrixf(wvp.Values[0]);
  
  glBegin(GL_TRIANGLES);
  for (i = 0; i < Primitive->NumOfI; i++)
  {
    glColor4fv((VOID *)&Primitive->Vertices[Primitive->Indexes[i]].Color.X);
    glVertex3fv(&Primitive->Vertices[Primitive->Indexes[i]].Vec.X);
  }
  glEnd();
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