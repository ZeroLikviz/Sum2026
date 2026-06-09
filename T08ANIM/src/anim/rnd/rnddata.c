/* FILE NAME: rnddata.c
 * PROGRAMMER: TM5
 * DATE: 09.06.2026
 * PURPOSE: Hold renderer data.
 */
#include "anim/rnd/rnd.h"
/* Renderer Data */
HWND TM5_hRndWnd;                 /* Work window handle */
HDC TM5_hRndDCFrame;              /* Work window memory device context  */
HBITMAP TM5_hRndBmFrame;          /* Work window background bitmap handle */
INT TM5_RndFrameW, TM5_RndFrameH; /* Work window size */

/* Camera Data */
DBL TM5_RndProjSize = 0.1;     /* Project plane fit square */
DBL TM5_RndProjDist = 0.1;     /* Distance to project plane from viewer (near) */
DBL TM5_RndProjFarClip = 300;  /* Distance to project far clip plane (far) */

MATR TM5_RndMatrView; /* View coordinate system matrix */
MATR TM5_RndMatrProj; /* Projection coordinate system matrix */
MATR TM5_RndMatrVP;   /* Stored (View * Proj) matrix */
/* End of 'rnddata.c' file */