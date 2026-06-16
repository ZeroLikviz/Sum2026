/* FILE NAME: rndprim.c
 * PROGRAMMER: TM5
 * DATE: 09.06.2026
 * PURPOSE: Implement renderer.
 */

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "anim/rnd/rnd.h"

VOID TM5_RndPrimCreateSquare( tm5PRIM *Primitive, DBL Size, VEC Color )
{
  tm5VERTEX Vertices[4] = {0};
  INT Indexes[6] = {0};
  INT i;
  
  Vertices[0].Vec = VecSet3(0, 0, 0);
  Vertices[1].Vec = VecSet3(Size, 0, 0);
  Vertices[2].Vec = VecSet3(Size, Size, 0);
  Vertices[3].Vec = VecSet3(0, Size, 0);

  Indexes[0] = 0;
  Indexes[1] = 1;
  Indexes[2] = 3;
  Indexes[3] = 3;
  Indexes[4] = 1;
  Indexes[5] = 2;

  for (i = 0; i < 4; i++)
    Vertices[i].Color = Vec3to4(Color);

  TM5_RndPrimCreate(Primitive, Vertices, 4, Indexes, 6, TM5_RND_TRIANGLES);
}

VOID TM5_RndPrimPermanentApply( tm5VERTEX *Vertices, INT NumOfV, MATR Matrix )
{
  INT i;

  for (i = 0; i < NumOfV; i++)
    Vertices[i].Vec = VecMulMatr(Vertices[i].Vec, Matrix);
}

VOID TM5_RndPrimRelocate( tm5VERTEX *Vertices, INT NumOfV )
{
  INT i;
  VEC Min = Vertices[0].Vec,
       Max = Vertices[0].Vec,
       Relocation;

  for (i = 0; i < NumOfV; i++)
  {
    Min.X = min(Vertices[i].Vec.X, Min.X);
    Min.Y = min(Vertices[i].Vec.Y, Min.Y);
    Min.Z = min(Vertices[i].Vec.Z, Min.Z);

    Max.X = max(Vertices[i].Vec.X, Max.X);
    Max.Y = max(Vertices[i].Vec.Y, Max.Y);
    Max.Z = max(Vertices[i].Vec.Z, Max.Z);
  }

  Relocation = VecScale(VecAddVec(Min, Max), -0.5);
  TM5_RndPrimPermanentApply(Vertices, NumOfV, MatrTranslate(Relocation));
}

VOID TM5_RndPrimResize( tm5VERTEX *Vertices, INT NumOfV )
{
  INT i;
  VEC Min = Vertices[0].Vec,
       Max = Vertices[0].Vec;
  DBL ResizeFactor;

  for (i = 0; i < NumOfV; i++)
  {
    Min.X = min(Vertices[i].Vec.X, Min.X);
    Min.Y = min(Vertices[i].Vec.Y, Min.Y);
    Min.Z = min(Vertices[i].Vec.Z, Min.Z);

    Max.X = max(Vertices[i].Vec.X, Max.X);
    Max.Y = max(Vertices[i].Vec.Y, Max.Y);
    Max.Z = max(Vertices[i].Vec.Z, Max.Z);
  }

  ResizeFactor = 1 / max(max(Max.X - Min.X, Max.Y - Min.Y), Max.Z - Min.Z);
  TM5_RndPrimPermanentApply(Vertices, NumOfV, MatrScale1(ResizeFactor));
}

VOID TM5_RndPrimStandartize( tm5VERTEX *Vertices, INT NumOfV )
{
  TM5_RndPrimRelocate(Vertices, NumOfV);
  TM5_RndPrimResize(Vertices, NumOfV);
}

VOID TM5_RndPrimCalculateNormals( tm5VERTEX *Vertices, INT NumOfV, INT *Indexes, INT NumOfI )
{
  INT i = 0;

  for (i = 0; i < NumOfV; i++)
    Vertices[i].Normal = VecSet3(0, 0, 0);

  for (i = 0; i < NumOfI; i += 3)
  {
    VEC Vec1 = Vertices[Indexes[i + 0]].Vec;
    VEC Vec2 = Vertices[Indexes[i + 1]].Vec;
    VEC Vec3 = Vertices[Indexes[i + 2]].Vec;
    VEC Cross = VecCross(VecSubVec(Vec1, Vec2), VecSubVec(Vec2, Vec3));
    Cross = VecNormalize(Cross);

    Vertices[Indexes[i + 0]].Normal = VecAddVec(Cross, Vertices[Indexes[i + 0]].Normal);
    Vertices[Indexes[i + 1]].Normal = VecAddVec(Cross, Vertices[Indexes[i + 1]].Normal);
    Vertices[Indexes[i + 2]].Normal = VecAddVec(Cross, Vertices[Indexes[i + 2]].Normal);
  }

  for (i = 0; i < NumOfV; i++)
    Vertices[i].Normal = VecNormalize(Vertices[i].Normal);
}

VOID TM5_RndPrimApplySun( tm5VERTEX *Vertices, INT NumOfV, VEC SunPos )
{
  INT i = 0;

  SunPos = VecNormalize(SunPos);
  for (i = 0; i < NumOfV; i++)
  {
    FLT Factor = (VecDot(VecScale(SunPos, -1), Vertices[i].Normal) + 1) / 2;
    if (Factor < 0.5)
      Factor = 0.5;
    Vertices[i].Color = Vec3to4(VecScale(Vec4to3(Vertices[i].Color), Factor));
  }
}

VOID TM5_RndPrimScale( tm5VERTEX *Vertices, INT NumOfV, FLT Factor )
{
  INT i = 0;

  for (i = 0; i < NumOfV; i++)
    Vertices[i].Vec = VecScale(Vertices[i].Vec, Factor);
}

VOID TM5_RndPrimFree( tm5PRIM *Primitive )
{
  glBindVertexArray(Primitive->VertexArrayId);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &Primitive->VertexBufferId);
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &Primitive->VertexArrayId);
  memset(Primitive, 0, sizeof(tm5PRIM));
}

VOID TM5_RndPrimCreate( tm5PRIM *Primitive, tm5VERTEX *Vertices, INT NofV, INT *Indexes, INT NofI, tm5DRAW_MODE DrawMode )
{
  memset(Primitive, 0, sizeof(tm5PRIM));
  
  Primitive->DrawMode = DrawMode;
  Primitive->Transform = MatrIdentity();
  Primitive->NumOfElements = NofI;
  glGenBuffers(1, &Primitive->VertexBufferId);
  glGenBuffers(1, &Primitive->IndexBufferId);
  glGenVertexArrays(1, &Primitive->VertexArrayId);

  glBindVertexArray(Primitive->VertexArrayId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Primitive->IndexBufferId);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT) * NofI, Indexes, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, Primitive->VertexBufferId);
  glBufferData(GL_ARRAY_BUFFER, sizeof(tm5VERTEX) * NofV, Vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(tm5VERTEX),
                        (VOID *)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(tm5VERTEX),
                        (VOID *)sizeof(VEC));
  glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(tm5VERTEX),
                        (VOID *)(sizeof(VEC) + sizeof(VEC2)));
  glVertexAttribPointer(3, 4, GL_FLOAT, FALSE, sizeof(tm5VERTEX),
                        (VOID *)(sizeof(VEC) * 2 + sizeof(VEC2)));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);

  glBindVertexArray(0);
}

VOID TM5_RndPrimLoad( tm5PRIM *Primitive, CHAR *FileName )
{
  FILE *F;
  INT nv = 0, nf = 0;
  tm5VERTEX *Vertices = NULL;
  INT *Indexes = NULL;
  static CHAR Buffer[2048];

  memset(Primitive, 0, sizeof(tm5PRIM));

  if ((F = fopen(FileName, "r")) == NULL)
    return;

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

  rewind(F);
  Vertices = malloc(nv * sizeof(tm5VERTEX));
  Indexes = malloc(nf * 3 * sizeof(INT));
  if (Vertices == NULL || Indexes == NULL)
  {
    fclose(F);
    free(Vertices);
    free(Indexes);
    return;
  }

  nv = 0;
  nf = 0;
  while (fgets(Buffer, sizeof(Buffer) - 1, F) != NULL)
  {
    if (Buffer[0] == 'v' && Buffer[1] == ' ')
    {
      DBL x, y, z;

      sscanf(Buffer + 2, "%lf%lf%lf", &x, &y, &z);
      Vertices[nv].Color = VecSet4(1, 0, 0, 1); 
      Vertices[nv++].Vec = VecSet3(x, y, z);
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

            Indexes[nf++] = n1;
            Indexes[nf++] = n2;
            Indexes[nf++] = n3;

            n2 = n3;
          }
          fvn++;
        }
        oldc = *ptr++;
      }
    }
  }

  TM5_RndPrimCalculateNormals(Vertices, nv, Indexes, nf);
  TM5_RndPrimApplySun(Vertices, nv, VecSet3(4, 6, 3));
  //TM5_RndPrimScale(Vertices, nv, 0.01);
  //TM5_RndPrimStandartize(Vertices, nv);
  TM5_RndPrimCreate(Primitive, Vertices, nv, Indexes, nf, TM5_RND_TRIANGLES);

  fclose(F);
  free(Vertices);
  free(Indexes);

  return;
}

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
    }
  TM5_RndGridAutoNormals(Grid);
}

VOID TM5_RndGridCreateHeightMap( tm5GRID *Grid, CHAR *Filename )
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

          Grid->Vertices[y * w + x].Vec = VecScale(VecSet3(x / (w - 1.0), hgt / 255.0, 1 - y / (h - 1.0)), 1);
        }
      TM5_RndGridAutoNormals(Grid);
    }
  }
}
/* End of 'rndprim.c' file */