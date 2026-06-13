/* FILE NAME: rndbase.c
 * PROGRAMMER: TM5
 * DATE: 09.06.2026
 * PURPOSE: Implement renderer.
 */
#include <stdlib.h>
#include <string.h>

#include "anim/rnd/rnd.h"
#include "anim/rnd/res/rndres.h"
#include "anim/timer.h"

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

  glewInit();

  TM5_RndFrameW = 0;
  TM5_RndFrameH = 0;
  TM5_RndCamSet(VecSet3(0.0f, 0.0f, 0.001f),
                VecSet3(0.0f, 0.0f, 0.0f),
                VecSet3(0.0f, 1.0f, 0.0f));

  TM5_RndResInit();
}

VOID TM5_RndClose( VOID )
{
  TM5_RndResClose();
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
  VEC4 ClearColor = {0.2, 0.0, 0.0, 1};
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
  MATR wvp = MatrMulMatr3(Primitive->Transform, World, TM5_RndMatrVP);
  INT loc,
    prim_type =
    Primitive->DrawMode == TM5_RND_WIREFRAME ? GL_LINES :
    Primitive->DrawMode == TM5_RND_TRIANGLES ? GL_TRIANGLES :
    GL_POINTS;

  TM5_RndShdUpdate();
  glUseProgram(TM5_RndProgId);
  if ((loc = glGetUniformLocation(TM5_RndProgId, "MatrWVP")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, (VOID *)wvp.Values);
  if ((loc = glGetUniformLocation(TM5_RndProgId, "Time")) != -1)
    glUniform1f(loc, Time);

  glBindVertexArray(Primitive->VertexArrayId);
  if (Primitive->VertexBufferId == 0)
    glDrawArrays(prim_type, 0, Primitive->NumOfElements);
  else
    glDrawElements(prim_type, Primitive->NumOfElements, GL_UNSIGNED_INT, NULL);
  
  glBindVertexArray(0);
  glUseProgram(0);
}

/* End of 'rndbase.c' file */