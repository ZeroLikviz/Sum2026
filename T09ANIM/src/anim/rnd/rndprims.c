/* FILE NAME: rndprims.c
 * PROGRAMMER: TM5
 * DATE: 09.06.2026
 * PURPOSE: Implement renderer.
 */

#include <stdio.h>
#include "anim/rnd/rnd.h"

MATR TM5_RndPrimsLoadTransform =
{
  {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
  }
};

BOOL TM5_RndPrimsCreate( tm5PRIMS *Prs, INT NumOfPrims )
{
  memset(Prs, 0, sizeof(tm5PRIMS));
  if ((Prs->Primitives = malloc(sizeof(tm5PRIM) * NumOfPrims)) == NULL)
    return FALSE;
  memset(Prs->Primitives, 0, sizeof(tm5PRIM) * NumOfPrims);
  Prs->NumOfPrims = NumOfPrims;
  Prs->Transparency = MatrIdentity();
  return TRUE;
} /* End of 'TM5_RndPrimsCreate' function */

VOID TM5_RndPrimsFree( tm5PRIMS *Prs )
{
  INT i;

  if (Prs->Primitives != NULL)
  {
    for (i = 0; i < Prs->NumOfPrims; i++)
      TM5_RndPrimFree(&Prs->Primitives[i]);
    free(Prs->Primitives);
  }
  memset(Prs, 0, sizeof(tm5PRIMS));
} /* End of 'TM5_RndPrimsFree' function */

VOID TM5_RndPrimsDraw( tm5PRIMS *Prs, MATR World )
{
  INT i;
  MATR m = MatrMulMatr(Prs->Transparency, World);

  TM5_RndShdAddonI[0] = Prs->NumOfPrims;
  /* Draw all nontransparent primitives */ 
  for (i = 0; i < Prs->NumOfPrims; i++)
    if (TM5_RndMtlGet(Prs->Primitives[i].MtlNumber)->Transparency == 1)
    {
      TM5_RndShdAddonI[1] = i;
      TM5_RndPrimDraw(&Prs->Primitives[i], m);
    }

  /* Draw all transparent primitives */ 
  glEnable(GL_CULL_FACE);

  /* Draw all front-face-culling */ 
  glCullFace(GL_FRONT);
  for (i = 0; i < Prs->NumOfPrims; i++)
    if (TM5_RndMtlGet(Prs->Primitives[i].MtlNumber)->Transparency != 1)
    {
      TM5_RndShdAddonI[1] = i;
      TM5_RndPrimDraw(&Prs->Primitives[i], m);
    }

  /* Draw all back-face-culling */ 
  glCullFace(GL_BACK);
  for (i = 0; i < Prs->NumOfPrims; i++)
    if (TM5_RndMtlGet(Prs->Primitives[i].MtlNumber)->Transparency != 1)
    {
      TM5_RndShdAddonI[1] = i;
      TM5_RndPrimDraw(&Prs->Primitives[i], m);
    }
  glDisable(GL_CULL_FACE);
} /* End of 'TM5_RndPrimsDraw' function */

BOOL TM5_RndPrimsLoad( tm5PRIMS *Prs, CHAR *FileName, BOOL Normalize )
{
  FILE *F;
  INT flen;
  UINT p, m, t;
  BYTE *mem, *ptr, *ptr_vertices;
  DWORD Sign;
  DWORD NumOfPrims;
  DWORD NumOfMaterials;
  DWORD NumOfTextures;
  struct tagG3DM_MATERIAL
  {
    CHAR Name[300]; /* Material name */
    /* Illumination coefficients */
    VEC Ka, Kd, Ks;     /* Ambient, diffuse, specular coefficients */
    FLT Ph;             /* Phong power coefficient – shininess */
    FLT Transparency;          /* Transparency factor */
    DWORD Tex[8];       /* Texture references 
                         * (8 time: texture number in G3DM file, -1 if no texture) */
    /* Shader information */
    CHAR ShaderString[300]; /* Additional shader information */
    DWORD Shader;       /* Shader number (uses after load into memory) */
  } *mtls;
  MATR trans_m = TM5_RndPrimsLoadTransform, trans_minv = MatrTranspose(MatrInverse(trans_m));

  TM5_RndPrimsLoadTransform = MatrIdentity();

  /* Read whole file to memory */
  memset(Prs, 0, sizeof(tm5PRIMS));
  if ((F = fopen(FileName, "rb")) == NULL)
    return FALSE;
  fseek(F, 0, SEEK_END);
  flen = ftell(F);

  if ((mem = malloc(flen)) == NULL)
  {
    fclose(F);
    return FALSE;
  }
  rewind(F);
  fread(mem, 1, flen, F);
  fclose(F);

  ptr = mem;
  Sign = *(DWORD *)ptr;
  ptr += 4;
  if (Sign != *(DWORD *)"G3DM")
  {
    free(mem);
    fclose(F);
    return FALSE;
  }
  NumOfPrims = *(DWORD *)ptr;
  ptr += 4;
  NumOfMaterials = *(DWORD *)ptr;
  ptr += 4;
  NumOfTextures = *(DWORD *)ptr;
  ptr += 4;

  if (!TM5_RndPrimsCreate(Prs, NumOfPrims))
  {
    free(mem);
    fclose(F);
    return FALSE;
  }

  /* Primitives */
  ptr_vertices = ptr;

  if (Normalize)
  {
    for (p = 0; p < NumOfPrims; p++)
    {
      DWORD NumOfVertexes;
      DWORD NumOfFacetIndexes;
      DWORD MtlNo;
      tm5VERTEX *V;
      INT *Ind;

      NumOfVertexes = *(DWORD *)ptr;
      ptr += 4;
      NumOfFacetIndexes = *(DWORD *)ptr;
      ptr += 4;
      MtlNo = *(DWORD *)ptr;
      ptr += 4;
      V = (tm5VERTEX *)ptr;
      ptr += sizeof(tm5VERTEX) * NumOfVertexes;
      Ind = (INT *)ptr;
      ptr += sizeof(INT) * NumOfFacetIndexes;

      Prs->MinBB = VecMinVec(Prs->MinBB, TM5_RndVertexCalculateMinBB(V, NumOfVertexes));
      Prs->MaxBB = VecMaxVec(Prs->MaxBB, TM5_RndVertexCalculateMaxBB(V, NumOfVertexes));
    }

    trans_m = MatrMulMatr(trans_m, MatrDescale1(VecMaxValue(VecSubVec(Prs->MaxBB, Prs->MinBB))));
    trans_minv = MatrTranspose(MatrInverse(trans_m));
  }
  ptr = ptr_vertices;
  for (p = 0; p < NumOfPrims; p++)
  {
    DWORD NumOfVertexes;
    DWORD NumOfFacetIndexes;
    DWORD MtlNo;
    tm5VERTEX *V;
    INT *Ind;
    UINT i;

    NumOfVertexes = *(DWORD *)ptr;
    ptr += 4;
    NumOfFacetIndexes = *(DWORD *)ptr;
    ptr += 4;
    MtlNo = *(DWORD *)ptr;
    ptr += 4;
    V = (tm5VERTEX *)ptr;
    ptr += sizeof(tm5VERTEX) * NumOfVertexes;
    Ind = (INT *)ptr;
    ptr += sizeof(INT) * NumOfFacetIndexes;

    for (i = 0; i < NumOfVertexes; i++)
    {
      V[i].Vec = VecMulMatr(V[i].Vec, trans_m);
      V[i].Normal = VecNormalize(VecMulMatr(V[i].Normal, trans_minv));
    }

    TM5_RndPrimCreate(&Prs->Primitives[p], V, NumOfVertexes, Ind, NumOfFacetIndexes, TM5_RND_TRIANGLES);
    Prs->Primitives[p].MtlNumber = TM5_RndMaterialsSize + MtlNo;
  }
  

  /* Materials */
  mtls = (VOID *)ptr;
  ptr += sizeof(*mtls) * NumOfMaterials;
  for (m = 0; m < NumOfMaterials; m++)
  {
    tm5MATERIAL mtl = TM5_RndMtlGetDef();

    mtl.Ka = VecMinVec(mtls[m].Ka, VecSet3(0.1, 0.1, 0.1));
    mtl.Kd = mtls[m].Kd;
    mtl.Ks = mtls[m].Ks;
    mtl.Ph = mtls[m].Ph;
    mtl.Transparency = mtls[m].Transparency;
    for (t = 0; t < 8; t++)
      if (mtls[m].Tex[t] != -1 && mtls[m].Tex[t] >= 0 && mtls[m].Tex[t] < NumOfTextures)
        mtl.Textures[t] = TM5_RndTexturesSize + mtls[m].Tex[t];
    TM5_RndMtlAdd(&mtl);
  }

  /* Textures */
  for (t = 0; t < NumOfTextures; t++)
  {
    CHAR *Name = (CHAR *)ptr;
    DWORD W, H, C;

    ptr += 300;
    W = *(DWORD *)ptr;
    ptr += 4;
    H = *(DWORD *)ptr;
    ptr += 4;
    C = *(DWORD *)ptr;
    ptr += 4;
    TM5_RndTexAddImg(Name, W, H, C, ptr);
    ptr += W * H * C;
  }
  free(mem);
  return TRUE;
}

/* End of 'rndprims.c' file */