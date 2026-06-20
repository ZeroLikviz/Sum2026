/* FILE NAME: rndgrid.c
 * PROGRAMMER: TM5
 * DATE: 09.06.2026
 * PURPOSE: 3D Animation Project.
 */

#include <stdio.h>
#include "anim/rnd/rnd.h"

INT TM5_RndGridCreate( tm5GRID *Grid, INT W, INT H )
{
  INT i, j;
  
  Grid->W = W;
  Grid->H = H;
  Grid->Vertices = malloc(sizeof(tm5VERTEX) * W * H);
  if(Grid->Vertices == NULL)
    return 0;

  for (j = 0; j < H; j++)
    for (i = 0; i < W; i++)
    {
      Grid->Vertices[j * W + i].Normal = VecSet3(0, 1, 0);
      Grid->Vertices[j * W + i].Color = VecSet4(0.5, 0.4, 0.3, 1.0);
      Grid->Vertices[j * W + i].Texture = VecSet2((DBL)i / (W - 1), (DBL)j / (H - 1));
      Grid->Vertices[j * W + i].Vec = VecSet3((DBL)i / (W - 1), 0, (DBL)j / (H - 1));
    }

  return 1;
}

VOID TM5_RndGridFree( tm5GRID *Grid )
{
  free(Grid->Vertices);
  memset(Grid, 0, sizeof(tm5GRID));
}

VOID TM5_RndPrimFromGrid( tm5PRIM *Primitive, tm5GRID *Grid )
{
  INT i, j, k;
  INT IndexesSize = (Grid->H - 1) * (Grid->W * 2 + 1) - 1;
  INT *Indexes = malloc(sizeof(INT) * IndexesSize);

  for (k = 0, j = 0; j < Grid->H - 1; j++)
  {
    for (i = 0; i < Grid->W; i++)
    {
      Indexes[k++] = (j + 1) * Grid->W + i;
      Indexes[k++] = j * Grid->W + i;
    }
    if (j != Grid->H - 2)
      Indexes[k++] = -1;
  }

  TM5_RndPrimCreate(Primitive, Grid->Vertices, Grid->W * Grid->H, Indexes, IndexesSize, TM5_RND_TRISTRIP);
  free(Indexes);
  TM5_RndGridFree(Grid);
}

INT GridPos( tm5GRID *Grid, INT X, INT Y )
{
  X = (X + Grid->W) % Grid->W;
  Y = (Y + Grid->H) % Grid->H;

  return Y * Grid->W + X;
}

VOID TM5_RndGridAutoNormals( tm5GRID *Grid )
{
  INT i, j;

  for (i = 0; i < Grid->W * Grid->H; i++)
    Grid->Vertices[i].Normal = VecSet3(0, 0, 0);

  for (j = 0; j < Grid->H - 1; j++)
    for (i = 0; i < Grid->W - 1; i++)
    {
      VEC Vec1 = VecSubVec(Grid->Vertices[GridPos(Grid, i, j)].Vec, Grid->Vertices[GridPos(Grid, i, j + 1)].Vec);
      VEC Vec2 = VecSubVec(Grid->Vertices[GridPos(Grid, i, j + 1)].Vec, Grid->Vertices[GridPos(Grid, i + 1, j + 1)].Vec);
      VEC Cross = VecNormalize(VecCross(Vec1, Vec2));

      Grid->Vertices[GridPos(Grid, i,     j)].Normal = VecAddVec(Grid->Vertices[GridPos(Grid, i,     j)].Normal, Cross);
      Grid->Vertices[GridPos(Grid, i + 1, j)].Normal = VecAddVec(Grid->Vertices[GridPos(Grid, i + 1, j)].Normal, Cross);
      Grid->Vertices[GridPos(Grid, i + 1, j + 1)].Normal = VecAddVec(Grid->Vertices[GridPos(Grid, i + 1, j + 1)].Normal, Cross);
      Grid->Vertices[GridPos(Grid, i,     j + 1)].Normal = VecAddVec(Grid->Vertices[GridPos(Grid, i    , j + 1)].Normal, Cross);
    }

  for (i = 0; i < Grid->W * Grid->H; i++)
    Grid->Vertices[i].Normal = VecNormalize(Grid->Vertices[i].Normal);
}

VOID TM5_RndGridCreateSphere( tm5GRID *Grid, DBL R, INT W, INT H )
{
  INT i, j;

  TM5_RndGridFree(Grid);
  TM5_RndGridCreate(Grid, W, H);

  for (j = 0; j < Grid->H; j++)
    for (i = 0; i < Grid->W; i++)
    {
      DBL Alpha = (DBL)i / (Grid->W - 1) * DPI;
      DBL Beta = (DBL)j / (Grid->H - 1) * PI;
      Grid->Vertices[j * Grid->W + i].Vec = VecSet3(R * sin(Alpha) * sin(Beta), R * cos(Beta), R * cos(Alpha) * sin(Beta));
      Grid->Vertices[j * Grid->W + i].Normal = VecNormalize(Grid->Vertices[j * Grid->W + i].Vec);
    }
}

VOID TM5_RndGridCreateHeightMap( tm5GRID *Grid, CHAR *Filename, FLT Size )
{
  INT w, h, x, y;
  CHAR Buffer[512];
  HBITMAP hBm;
  BITMAP bm;

  sprintf(Buffer, "bin/height_maps/%s", Filename);
  if ((hBm = LoadImage(NULL, Buffer, IMAGE_BITMAP, 0, 0,
                       LR_LOADFROMFILE | LR_CREATEDIBSECTION)) != NULL)
  {
    GetObject(hBm, sizeof(bm), &bm);
    w = bm.bmWidth;
    h = bm.bmHeight;
    if (TM5_RndGridCreate(Grid, w, h))
    {
      BYTE *Bits = bm.bmBits;

      for (y = 0; y < h; y++)
        for (x = 0; x < w; x++)
        {
          INT hgt = Bits[(h - 1 - y) * bm.bmWidthBytes + x];

          Grid->Vertices[y * w + x].Vec = VecScale(VecSet3(2.0 * x / (w - 1.0) - 1,
                                                           hgt / 255.0 / 12,
                                                           2.0 * y / (h - 1.0) - 1), Size / 2);
        }
      TM5_RndGridAutoNormals(Grid);
    }
  }
}
/* End of 'rndgrid.c' file */