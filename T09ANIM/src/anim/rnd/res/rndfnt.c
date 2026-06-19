/* FILE NAME  : rndfnt.c
 * PROGRAMMER : TM5
 * LAST UPDATE: 19.06.2026
 * PURPOSE    : 3D animation project.
 *              Rendering resources implementation module.
 *              Resource implemetation module.
 *              Font handle module.
 */

#include <stdio.h>
#include <string.h>

#include "anim/rnd/rnd.h"

/***
 * Fonts handle functions
 ***/

/* Current font description */
/* Font data */
static tm5FONT TM5_RndFntFont;

/* Font characters primitives */
static tm5PRIM TM5_RndFntChars[256];

/* Font material */
static INT TM5_RndFntMtlNo;

/* Load font from .G3DF file function.
 * ARGUMENTS:
 *   - font file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL TM5_RndFntLoad( CHAR *FileName )
{
  DWORD Sign, W, H, *Tex;
  tm5VERTEX Chars[256][4];
  INT Indexes[4] = {0, 1, 2, 3};
  tm5MATERIAL mtl;
  FILE *F;
  INT i;

  TM5_RndFntClose();

  if ((F = fopen(FileName, "rb")) == NULL)
    return FALSE;

  if (fread(&Sign, 4, 1, F) != 1 || Sign != *(DWORD *)"G3DF")
  {
    fclose(F);
    return FALSE;
  }

  fread(&TM5_RndFntFont, sizeof(tm5FONT), 1, F);
  fread(Chars, sizeof(tm5VERTEX), 4 * 256, F);
  fread(&W, 4, 1, F);
  fread(&H, 4, 1, F);
  if ((Tex = malloc(W * H * 4)) == NULL)
  {
    memset(&TM5_RndFntFont, 0, sizeof(TM5_RndFntFont));
    memset(&TM5_RndFntFont, 0, sizeof(TM5_RndFntFont));
    fclose(F);
    return FALSE;
  }
  fread(Tex, 4, W * H, F);
  fclose(F);

  /* Material */
  mtl = TM5_RndMtlGetDef();
  strncpy(mtl.Name, FileName, TM5_STR_MAX - 1);
  mtl.Textures[0] = TM5_RndTexAddImg(FileName, W, H, 4, Tex);
  mtl.ShaderNumber = TM5_RndShdAdd("font");
  TM5_RndFntMtlNo = TM5_RndMtlAdd(&mtl);

  for (i = 0; i < 256; i++)
  {
    TM5_RndPrimCreate(&TM5_RndFntChars[i], Chars[i], 4, Indexes, 4, TM5_RND_TRISTRIP);
    TM5_RndFntChars[i].MtlNumber = TM5_RndFntMtlNo;
  }
  free(Tex);
  return TRUE;
} /* End of 'TM5_RndFntLoad' function */

/* Init font subsystem function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TM5_RndFntInit( VOID )
{
  TM5_RndFntLoad("bin/fonts/font.g3dm");
} /* End of 'TM5_RndFntInit' function */

/* Deinit font subsystem function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TM5_RndFntClose( VOID )
{
  INT i;

  for (i = 0; i < 256; i++)
    TM5_RndPrimFree(&TM5_RndFntChars[i]);
  memset(TM5_RndFntChars, 0, sizeof(TM5_RndFntChars));
  memset(&TM5_RndFntFont, 0, sizeof(TM5_RndFntFont));
} /* End of 'TM5_RndFntInit' function */

/* Draw screen space string function.
 * ARGUMENTS:
 *   - string to draw:
 *       CHAR *Str;
 *   - draw position:
 *       VEC Pos;
 *   - font size:
 *      FLT Size;
 * RETURNS: None.
 */

VOID TM5_RndFntDraw3D( CHAR *Str, VEC Pos, FLT Size )
{
  VEC Start = Pos;
 
  while (*Str != 0)
  {
    if (*Str == '\n')
      Pos.X = Start.X, Pos.Y -= Size;
    else if (TM5_RndFntFont.AdvanceX[(UCHAR)*Str] != 0)
    {
      TM5_RndPrimDraw(&TM5_RndFntChars[(UCHAR)*Str],
        MatrMulMatr(MatrScale1(Size), MatrTranslate(Pos)));
      Pos.X += TM5_RndFntFont.AdvanceX[(UCHAR)*Str] * Size;
    }
    Str++;
  }
}

VOID TM5_RndFntDraw( CHAR *Str, VEC Pos, FLT Size )
{
  VEC Start = Pos;

  if (Size <= 0)
    Size = TM5_RndFntFont.LineH;

  while (*Str != 0)
  {
    if (*Str == '\n')
      Pos.X = Start.X, Pos.Y -= Size;
    else if (TM5_RndFntFont.AdvanceX[(UCHAR)*Str] != 0)
    {
      TM5_RndPrimDraw(&TM5_RndFntChars[(UCHAR)*Str],
        MatrMulMatr(MatrScale1(Size), MatrTranslate(Pos)));
      Pos.X += TM5_RndFntFont.AdvanceX[(UCHAR)*Str] * Size;
    }
    Str++;
  }
} /* End of 'TM5_RndFntDraw' function */

/* END OF 'rndfnt.c' FILE */