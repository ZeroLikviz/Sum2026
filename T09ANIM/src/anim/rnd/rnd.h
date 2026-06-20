/* FILE NAME: rnd.h
 * PROGRAMMER: TM5
 * DATE: 09.06.2026
 * PURPOSE: Implement renderer.
 */
#ifndef __rnd_h_
#define __rnd_h_
#define GLEW_STATIC
#include <glew.h>
#include <windows.h>

#include "anim/rnd/res/rndres.h"

typedef enum tagtm5DRAW_MODE
{
  TM5_RND_TRIANGLES,
  TM5_RND_WIREFRAME,
  TM5_RND_TRISTRIP,
  TM5_RND_POINTS
} tm5DRAW_MODE;

/* Primitive Structure */
typedef struct tagtm5VERTEX
{
  VEC Vec;        /* Position */
  VEC2 Texture;   /* Texture Coordinates */
  VEC Normal;     /* Normal vector */
  VEC4 Color;     /* Vertex color */
} tm5VERTEX;

typedef struct tagtm5PRIM
{
  INT VertexArrayId;
  INT VertexBufferId;
  INT IndexBufferId;
  INT NumOfElements;

  tm5DRAW_MODE DrawMode;

  MATR Transform;      /* Additional transformation matrix */

  INT MtlNumber;

  VEC MaxBB;
  VEC MinBB;

} tm5PRIM;

typedef struct tagtm5GRID
{
  INT W, H;      /* Grid size (in vertices) */
  tm5VERTEX *Vertices;  /* Array (2D) of vertex */
} tm5GRID;

/* Primitive collection data type */
typedef struct tagtm5PRIMS
{
  INT NumOfPrims;      /* Number of primitives in array */  
  tm5PRIM *Primitives; /* Array of primitives */
  MATR Transparency;   /* Common transformation matrix */

  VEC MaxBB;
  VEC MinBB;
} tm5PRIMS;

/* Renderer Data */
extern HWND TM5_hRndWnd;                 /* Work window handle */
extern HDC TM5_hRndDC;                   /* Work window memory device context  */
extern HGLRC TM5_hRndGLRC;               /* GL window context */
extern INT TM5_RndFrameW, TM5_RndFrameH; /* Work window size */

/* Camera Data */
extern DBL TM5_RndProjSize;     /* Project plane fit square */
extern DBL TM5_RndProjDist;     /* Distance to project plane from viewer (near) */
extern DBL TM5_RndProjFarClip;  /* Distance to project far clip plane (far) */

extern MATR TM5_RndMatrView; /* View coordinate system matrix */
extern MATR TM5_RndMatrProj; /* Projection coordinate system matrix */
extern MATR TM5_RndMatrVP;   /* Stored (View * Proj) matrix */

/* Prims Data */
extern MATR TM5_RndPrimsLoadTransform;
extern INT TM5_RndShdAddonI[8];
extern FLT TM5_RndShdAddonF[8];
extern VEC TM5_RndShdAddonV[8];

/* Renderer functions */
VOID TM5_RndInit( HWND hWnd );
VOID TM5_RndClose( VOID );
VOID TM5_RndResize( INT W, INT H );
VOID TM5_RndCopyFrame( VOID );
VOID TM5_RndStart( VOID );
VOID TM5_RndEnd( VOID );

/* Camera & Primitive functions */
VOID TM5_RndProjSet( VOID );
VOID TM5_RndCamSet( VEC Loc, VEC At, VEC Up );
VOID TM5_RndPrimDraw( tm5PRIM *Pr, MATR World );
VOID TM5_RndPrimFree( tm5PRIM *Primitive );
VOID TM5_RndPrimCreate( tm5PRIM *Primitive, tm5VERTEX *Vertices, INT NofV, INT *Indexes, INT NofI, tm5DRAW_MODE DrawMode );

VOID TM5_RndPrimCreateSquare( tm5PRIM *Primitive, DBL Size, VEC Color );
VOID TM5_RndPrimPermanentApply( tm5VERTEX *Vertices, INT NumOfV, MATR Matrix );
VOID TM5_RndPrimRelocate( tm5VERTEX *Vertices, INT NumOfV );
VOID TM5_RndPrimResize( tm5VERTEX *Vertices, INT NumOfV );
VOID TM5_RndPrimStandartize( tm5VERTEX *Vertices, INT NumOfV );
VOID TM5_RndPrimCalculateNormals( tm5VERTEX *Vertices, INT NumOfV, INT *Indexes, INT NumOfI );
VOID TM5_RndPrimApplySun( tm5VERTEX *Vertices, INT NumOfV, VEC SunPos );
VOID TM5_RndPrimScale( tm5VERTEX *Vertices, INT NumOfV, FLT Factor );
VEC TM5_RndVertexCalculateMinBB( tm5VERTEX *Vertices, INT Size );
VEC TM5_RndVertexCalculateMaxBB( tm5VERTEX *Vertices, INT Size );

/* Grid Functions */
INT TM5_RndGridCreate( tm5GRID *Grid, INT W, INT H );
VOID TM5_RndGridFree( tm5GRID *Grid );
VOID TM5_RndPrimFromGrid( tm5PRIM *Primitive, tm5GRID *Grid );
VOID TM5_RndGridAutoNormals( tm5GRID *Grid );
VOID TM5_RndGridCreateSphere( tm5GRID *Grid, DBL R, INT W, INT H );
VOID TM5_RndGridCreateHeightMap( tm5GRID *Grid, CHAR *Filename, FLT Size );

/* Prims Functions */

BOOL TM5_RndPrimsCreate( tm5PRIMS *Prs, INT NumOfPrims );
VOID TM5_RndPrimsFree( tm5PRIMS *Prs );
VOID TM5_RndPrimsDraw( tm5PRIMS *Prs, MATR World );
BOOL TM5_RndPrimsLoad( tm5PRIMS *Prs, CHAR *FileName, BOOL Normalize );

#endif

/* End of 'rnd.h' file */