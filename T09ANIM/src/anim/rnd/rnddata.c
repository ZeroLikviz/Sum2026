/* FILE NAME: rnddata.c
 * PROGRAMMER: TM5
 * DATE: 09.06.2026
 * PURPOSE: Hold renderer data.
 */
#include "anim/rnd/rnd.h"
/* Renderer Data */
HWND TM5_hRndWnd;                 /* Work window handle */
HDC TM5_hRndDC;                   /* Work window memory device context  */
HGLRC TM5_hRndGLRC;               /* GL window context */
INT TM5_RndFrameW, TM5_RndFrameH; /* Work window size */

/* Camera Data */
DBL TM5_RndProjSize = 0.1;     /* Project plane fit square */
DBL TM5_RndProjDist = 0.1;     /* Distance to project plane from viewer (near) */
DBL TM5_RndProjFarClip = 2048;  /* Distance to project far clip plane (far) */

MATR TM5_RndMatrView; /* View coordinate system matrix */
MATR TM5_RndMatrProj; /* Projection coordinate system matrix */
MATR TM5_RndMatrVP;   /* Stored (View * Proj) matrix */

INT TM5_RndShdAddonI[8] = {0};
FLT TM5_RndShdAddonF[8] = {0};
VEC TM5_RndShdAddonV[8] = {0};
/* End of 'rnddata.c' file */