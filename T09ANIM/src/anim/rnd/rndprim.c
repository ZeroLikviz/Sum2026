/* FILE NAME: rndprim.c
 * PROGRAMMER: TM5
 * DATE: 09.06.2026
 * PURPOSE: Implement renderer.
 */

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "anim/rnd/rnd.h"

VOID TM5_RndPrimCreateSphere( tm5PRIM *Primitive, DBL R, INT W, INT H )
{
  /*
  INT i, j, k;
  DBL theta, phi;
 
  if (!TM5_RndPrimCreate(Primitive, W * H, (H - 1) * (W - 1) * 2 * 3))
    return FALSE;
 
  for (k = 0, i = 0, theta = 0; i < H; i++, theta += PI / (H - 1))
    for (j = 0, phi = 0; j < W; j++, phi += 2 * PI / (W - 1))
      Primitive->Vertices[k++].Vec = VecSet3(R * sin(theta) * sin(phi),
                                             R * cos(theta),
                                             R * sin(theta) * cos(phi));

  for (k = 0, i = 0; i < H - 1; i++)
    for (j = 0; j < W - 1; j++)
    {
      Primitive->Indexes[k++] = i * W + j;
      Primitive->Indexes[k++] = i * W + j + 1;
      Primitive->Indexes[k++] = (i + 1) * W + j;
      
      Primitive->Indexes[k++] = (i + 1) * W + j;
      Primitive->Indexes[k++] = i * W + j + 1;
      Primitive->Indexes[k++] = (i + 1) * W + j + 1;
    }
  */
}

VOID TM5_RndPrimCreateCylinder( tm5PRIM *Primitive, DBL R, DBL H, INT W )
{
  /*
  INT i, sign, k;
  DBL alpha;
 
  if (!TM5_RndPrimCreate(Primitive, W * 2 + 2, (W - 1) * 4 * 3 ))
    return FALSE;
 
  Primitive->Vertices[0].Vec = VecSet3(0, -H / 2, 0);
  Primitive->Vertices[1].Vec = VecSet3(0, H / 2, 0);
  for (k = 2, sign = -1; sign < 2; sign += 2)
    for (i = 0, alpha = 0; i < W; i++, alpha += DPI / (W - 1))
      Primitive->Vertices[k++].Vec = VecSet3(R * cos(alpha),
                                             sign * H / 2,
                                             R * sin(alpha));
  for (k = 0, i = 0; i < W - 1; i++)
  {
    Primitive->Indexes[k++] = 0;
    Primitive->Indexes[k++] = i + 1 + 2;
    Primitive->Indexes[k++] = i + 2;
  }
  for (i = 0; i < W - 1; i++)
  {
    Primitive->Indexes[k++] = 1;
    Primitive->Indexes[k++] = i + W + 1 + 2;
    Primitive->Indexes[k++] = i + W + 2;
  }
  for (i = 0; i < W - 1; i++)
  {
    Primitive->Indexes[k++] = i + 2;
    Primitive->Indexes[k++] = i + W + 2;
    Primitive->Indexes[k++] = i + W + 1 + 2;

    Primitive->Indexes[k++] = i + W + 1 + 2;
    Primitive->Indexes[k++] = i + 1 + 2;
    Primitive->Indexes[k++] = i + 2;
  }
  */
}

VOID TM5_RndPrimCreateTorus( tm5PRIM *Primitive, DBL oR, DBL iR, INT tW, INT iW )
{
  /*
  INT i, j, k;
  DBL alpha, beta;
 
  if (!TM5_RndPrimCreate(Primitive, tW * iW, (tW - 1) * (iW - 1) * 2 * 3))
    return FALSE;
 
  for (i = 0, alpha = 0, k = 0; i < tW; i++, alpha += DPI / (tW - 1))
  {
    MATR MatrixY = MatrRotateY(R2D(alpha));
    for (j = 0, beta = 0; j < iW; j++, beta += DPI / (iW - 1))
      Primitive->Vertices[k++].Vec = VecMulMatr(VecSet3(oR + cos(beta) * iR +  iR,
                                                        sin(beta) * iR,
                                                        0),
                                     MatrixY);
  }

  for (i = 0, k = 0; i < tW - 1; i++)
    for (j = 0; j < iW - 1; j++)
    {
      Primitive->Indexes[k++] = i * iW + j;
      Primitive->Indexes[k++] = (i + 1) * iW + j;
      Primitive->Indexes[k++] = i * iW + j + 1;

      Primitive->Indexes[k++] = (i + 1) * iW + j;
      Primitive->Indexes[k++] = (i + 1) * iW + j + 1;
      Primitive->Indexes[k++] = i * iW + j + 1;
    }
  */
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

/* End of 'rndprim.c' file */