/* FILE NAME: rnd.h
 * PROGRAMMER: TM5
 * DATE: 09.06.2026
 * PURPOSE: Implement renderer.
 */
#ifndef __rnd_h_
#define __rnd_h_
#include <windows.h>
#include "def.h"

/* Primitive Structure */
typedef struct tagtm5VERTEX
{
  VEC3 Vec;
} tm5VERTEX;

typedef struct tagtm5PRIM
{
  tm5VERTEX *Vertices; /* Vertex attributes array */
  INT NumOfV;          /* Number of vertices */

  INT *Indexes;        /* Index array (for trimesh – by 3 ones) */
  INT NumOfI;          /* Number of indices */
  
  POINT *Projections;

  MATR Transform;      /* Additional transformation matrix */
} tm5PRIM;

/* Renderer Data */
extern HWND TM5_hRndWnd;                 /* Work window handle */
extern HDC TM5_hRndDCFrame;              /* Work window memory device context  */
extern HBITMAP TM5_hRndBmFrame;          /* Work window background bitmap handle */
extern INT TM5_RndFrameW, TM5_RndFrameH; /* Work window size */

/* Camera Data */
extern DBL TM5_RndProjSize;     /* Project plane fit square */
extern DBL TM5_RndProjDist;     /* Distance to project plane from viewer (near) */
extern DBL TM5_RndProjFarClip;  /* Distance to project far clip plane (far) */

extern MATR TM5_RndMatrView; /* View coordinate system matrix */
extern MATR TM5_RndMatrProj; /* Projection coordinate system matrix */
extern MATR TM5_RndMatrVP;   /* Stored (View * Proj) matrix */

/* Renderer functions */
VOID TM5_RndInit( HWND hWnd );
VOID TM5_RndClose( VOID );
VOID TM5_RndResize( INT W, INT H );
VOID TM5_RndCopyFrame( HDC hDC );
VOID TM5_RndStart( VOID );
VOID TM5_RndEnd( VOID );

/* Camera & Primitive functions */
VOID TM5_RndProjSet( VOID );
VOID TM5_RndCamSet( VEC3 Loc, VEC3 At, VEC3 Up );
VOID TM5_RndPrimDraw( tm5PRIM *Pr, MATR World );
VOID TM5_RndPrimFree( tm5PRIM *Primitive );
BOOL TM5_RndPrimCreate( tm5PRIM *Primitive, INT NofV, INT NofI );
BOOL TM5_RndPrimLoad( tm5PRIM *Primitive, CHAR *Filename );

BOOL TM5_RndPrimCreateSphere( tm5PRIM *Primitive, DBL R, INT W, INT H );
BOOL TM5_RndPrimCreateCylinder( tm5PRIM *Primitive, DBL R, DBL H, INT W);
BOOL TM5_RndPrimCreateTorus( tm5PRIM *Primitive, DBL oR, DBL iR, INT tW, INT iW);
VOID TM5_RndPrimPermanentApply( tm5PRIM *Primitive, MATR Matrix );
VOID TM5_RndPrimRelocate( tm5PRIM *Primitive );
VOID TM5_RndPrimRelocateMass( tm5PRIM *Primitive );
VOID TM5_RndPrimResizeTo1( tm5PRIM *Primitive );
VOID TM5_RndPrimStandartize( tm5PRIM *Primitive );
#endif

/* End of 'rnd.h' file */